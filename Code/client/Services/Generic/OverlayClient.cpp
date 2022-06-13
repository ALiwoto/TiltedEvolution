#include <TiltedOnlinePCH.h>

#include <OverlayRenderHandler.hpp>

#include <Services/OverlayClient.h>
#include <Services/TransportService.h>

#include <Events/CommandEvent.h>

#include <Messages/SendChatMessageRequest.h>
#include <Messages/TeleportRequest.h>

#include <World.h>

OverlayClient::OverlayClient(TransportService& aTransport, TiltedPhoques::OverlayRenderHandler* apHandler)
    : TiltedPhoques::OverlayClient(apHandler), m_transport(aTransport)
{
}

OverlayClient::~OverlayClient() noexcept
{
}

bool OverlayClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                             CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
    if (message->GetName() == "ui-event")
    {
        auto pArguments = message->GetArgumentList();

        auto eventName = pArguments->GetString(0).ToString();
        auto eventArgs = pArguments->GetList(1);

        spdlog::info(eventName);
        spdlog::info(eventArgs->GetString(0).ToString());
        spdlog::info(std::to_string(eventArgs->GetInt(1)));
        spdlog::info(eventArgs->GetString(2).ToString());

#ifndef PUBLIC_BUILD
        LOG(INFO) << "event=ui_event name=" << eventName;
#endif

        if (eventName == "connect")
            ProcessConnectMessage(eventArgs);
        else if (eventName == "disconnect")
            ProcessDisconnectMessage();
        else if (eventName == "sendMessage")
            ProcessChatMessage(eventArgs);
        else if (eventName == "launchParty")
            World::Get().GetPartyService().CreateParty();
        else if (eventName == "leaveParty")
            World::Get().GetPartyService().LeaveParty();
        else if (eventName == "createPartyInvite")
        {
            uint32_t aPlayerId = eventArgs->GetInt(0);
            World::Get().GetPartyService().CreateInvite(aPlayerId);
        }
        else if (eventName == "acceptPartyInvite")
        {
            uint32_t aInviterId = eventArgs->GetInt(0);
            World::Get().GetPartyService().AcceptInvite(aInviterId);
        }
        else if (eventName == "kickPartyMember")
        {
            uint32_t aPlayerId = eventArgs->GetInt(0);
            World::Get().GetPartyService().KickPartyMember(aPlayerId);
        }
        else if (eventName == "changePartyLeader")
        {
            uint32_t aPlayerId = eventArgs->GetInt(0);
            World::Get().GetPartyService().ChangePartyLeader(aPlayerId);
        }
        else if (eventName == "teleportToPlayer")
            ProcessTeleportMessage(eventArgs);

        return true;
    }

    return false;
}

void OverlayClient::ProcessConnectMessage(CefRefPtr<CefListValue> aEventArgs)
{
    std::string baseIp = aEventArgs->GetString(0);
    if (baseIp == "localhost")
    {
        baseIp = "127.0.0.1";
    }

    uint16_t port = aEventArgs->GetInt(1) ? aEventArgs->GetInt(1) : 10578;
    // iAmAToken = aEventArgs->GeString(2);

    std::string endpoint = baseIp + ":" + std::to_string(port);

    World::Get().GetRunner().Queue([endpoint] { 
        World::Get().GetTransport().Connect(endpoint);
    });
}

void OverlayClient::ProcessDisconnectMessage()
{
    World::Get().GetRunner().Queue([]() { World::Get().GetTransport().Close(); });
}

void OverlayClient::ProcessChatMessage(CefRefPtr<CefListValue> aEventArgs)
{
    std::string contents = aEventArgs->GetString(0).ToString();
    if (!contents.empty())
    {
        if (contents[0] == '/')
        {
            World::Get().GetRunner().Trigger(CommandEvent(std::move(String(contents))));
        }
        else
        {
            SendChatMessageRequest messageRequest;
            messageRequest.ChatMessage = aEventArgs->GetString(0).ToString();
            spdlog::debug("Received Message from UI and will send it to server: " + messageRequest.ChatMessage);
            m_transport.Send(messageRequest);
        }
    }
}

void OverlayClient::ProcessTeleportMessage(CefRefPtr<CefListValue> aEventArgs)
{
    TeleportRequest request{};
    request.PlayerId = aEventArgs->GetInt(0);

    m_transport.Send(request);
}
