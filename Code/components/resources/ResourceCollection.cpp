// Copyright (C) 2022 TiltedPhoques SRL.
// For licensing information see LICENSE at the root of this distribution.

#include "ResourceCollection.h"
#include "SemanticVersion2.h"

#include <base/Check.h>
#include <base/simpleini/SimpleIni.h>
#include <regex>

namespace Resources
{
namespace
{
constexpr char kResourceFolderName[] = "resources";
constexpr char kResourceManifestExt[] = ".manifest";
static const SemanticVersion kApiSet(1, 0, 0);

TiltedPhoques::Vector<TiltedPhoques::String> ParseQuotedTokens(const TiltedPhoques::String& aString)
{
    TiltedPhoques::Vector<TiltedPhoques::String> result;
    size_t start = aString.find_first_of('"');
    while (start != std::string::npos)
    {
        size_t end = aString.find_first_of('"', start + 1);
        if (end == std::string::npos)
        {
            break;
        }
        result.push_back(aString.substr(start + 1, end - start - 1));
        start = aString.find_first_of('"', end + 1);
        while (start != std::string::npos && isspace(aString[start]))
        {
            ++start;
        }
    }
    return result;
}

Manifest001::DependencyTuple SplitDependencyString(const TiltedPhoques::String& aString)
{
    size_t atPos = aString.find_first_of('@');
    if (atPos == TiltedPhoques::String::npos)
    {
        return {aString, {0, 0, 0}};
    }
    auto name = aString.substr(0, atPos);
    auto versionString = aString.substr(atPos + 1);

    SemanticVersion version(versionString.c_str());
    return {name, version};
}

TiltedPhoques::String UnescapeAndStrip(const std::string_view& aString)
{
    TiltedPhoques::String result;
    for (char c : aString)
    {
        if (c == '\\')
        {
            continue;
        }
        else if (c == '"')
        {
            continue;
        }
        else if (c == '\n')
        {
            continue;
        }
        result += c;
    }
    return result;
}

// Returns true if the given string is a valid semantic version.
bool IsValidSemanticVersion(const std::string& acVersion)
{
    // Use a regular expression to match the semantic version format:
    // major.minor.patch
    // where major, minor, and patch are non-negative integers.
    std::regex pattern("^\\d+\\.\\d+\\.\\d+$");
    return std::regex_match(acVersion, pattern);
}
} // namespace

ResourceCollection::ResourceCollection()
{
    CollectResources();
}

ResourceCollection::~ResourceCollection()
{
}

bool ResourceCollection::LoadManifestData(const std::filesystem::path& aPath)
{
    // example:
    // name = "my-resource"
    // resource_version = 1.0.0
    // apiset = 1.0.0
    // description = "A resource that does stuff"
    // keywords = ["selfmade", "resource"]
    // license = "MIT"
    // repository = ""
    // homepage = ""
    // entrypoint = "test.lua"
    // dependencies = ["dependency-a@1.0.0", "dependency-b@1.0.0"]

    CSimpleIni ini;
    {
        auto buf = TiltedPhoques::LoadFile(aPath);
        if (ini.LoadData(buf.c_str()) != SI_Error::SI_OK)
        {
            spdlog::error("Failed to load manifest file {}", aPath.string());
            return false;
        }
    }

    auto manifest = TiltedPhoques::MakeUnique<Resources::Manifest001>();
    // version data
    auto readSemVer = [&](const char* apName) -> SemanticVersion {
        auto stringRep = ini.GetValue("Resource", apName, "0.0.0");
        if (!IsValidSemanticVersion(stringRep))
        {
            spdlog::error("Invalid semantic version for {} in {}", apName, aPath.string());
            return {0, 0, 0};
        }
        return SemanticVersion(stringRep);
    };

    manifest->apiSet = readSemVer("apiset");
    manifest->resourceVersion = readSemVer("version");
    if (!manifest->apiSet || !manifest->resourceVersion)
    {
        return false;
    }

    if (manifest->apiSet > Resources::kApiSet)
    {
        spdlog::error("Resource {} requires a newer API set than the current one", aPath.string());
        return false;
    }

    auto readString = [&](const char* apName) -> TiltedPhoques::String {
        auto value = ini.GetValue("Manifest", apName, nullptr);
        if (value == nullptr)
        {
            spdlog::error("Missing {} in {}", apName, aPath.string());
            return "";
        }
        return UnescapeAndStrip(value);
    };

    // read must haves
    manifest->name = readString("name");
    manifest->description = readString("description");
    manifest->entryPoint = readString("entrypoint");
    if (manifest->name.empty() || manifest->description.empty() || manifest->entryPoint.empty())
    {
        return false;
    }

    // optional entries
    manifest->license = readString("license");
    manifest->repository = readString("repository");
    manifest->homepage = readString("homepage");

    // lists of strings
    auto readStringList = [&ini](const char* apName) -> TiltedPhoques::Vector<TiltedPhoques::String> {
        return ParseQuotedTokens(ini.GetValue("Resource", apName, ""));
    };
    for (const auto& dep : readStringList("dependencies"))
    {
        manifest->dependencies.push_back(SplitDependencyString(dep));
    }

    m_manifests.push_back(std::move(manifest));

    return true;
}

void ResourceCollection::ResolveDependencies()
{
    for (auto& manifest : m_manifests)
    {
        for (const auto& dep : manifest->dependencies)
        {
            for (auto& it2 : m_manifests)
            {
                if (it2->name == dep.first)
                {
                    if (!dep.second)
                    {
                        spdlog::error("Dependency {} version is invalid", dep.first);
                        manifest->isTombstone = true;
                        continue;
                    }

                    if (dep.second && dep.second != it2->resourceVersion)
                    {
                        spdlog::error("Dependency {} has version {} but {} is required", dep.first,
                                      SemVerToString(it2->resourceVersion), SemVerToString(dep.second));
                        manifest->isTombstone = true;
                    }
                    // let the loop continue to list all other conflicts immedeatly.
                }
            }
        }
    }
}

void ResourceCollection::CollectResources()
{
    m_resourcePath = std::filesystem::current_path() / kResourceFolderName;

    std::vector<std::filesystem::path> manifestCanidates;
    for (auto const& dir : std::filesystem::recursive_directory_iterator{m_resourcePath})
    {
        if (dir.path().extension() == kResourceManifestExt)
        {
            manifestCanidates.push_back(dir.path());
        }
    }

    uint32_t failedCount = 0;
    for (const auto& path : manifestCanidates)
    {
        if (!LoadManifestData(path))
        {
            failedCount++;
        }
    }

    if (failedCount > 0)
    {
        spdlog::warn("{} manifests failed to load", failedCount);
    }

    ResolveDependencies();

    failedCount = 0;
    for (auto iter = m_manifests.begin(); iter != m_manifests.end();)
    {
        if ((*iter)->isTombstone)
        {
            failedCount++;
            iter = m_manifests.erase(iter);
        }
        else
            ++iter;
    }

    if (failedCount > 0)
    {
        spdlog::warn("{} resources failed to load", failedCount);
    }
}
} // namespace Resources