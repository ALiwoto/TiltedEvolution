#include <GameServer.h>

#include <Services/OverlayService.h>

#include <Messages/NotifyChatMessageBroadcast.h>
#include <Messages/SendChatMessageRequest.h>
#include <Messages/PlayerDialogueRequest.h>
#include <Messages/NotifyPlayerDialogue.h>

#include "Game/Player.h"

#include <regex>

OverlayService::OverlayService(World& aWorld, entt::dispatcher& aDispatcher)
    : m_world(aWorld)
{
    m_chatMessageConnection = aDispatcher.sink<PacketEvent<SendChatMessageRequest>>().connect<&OverlayService::HandleChatMessage>(this);
    m_playerDialogueConnection = aDispatcher.sink<PacketEvent<PlayerDialogueRequest>>().connect<&OverlayService::OnPlayerDialogue>(this);
}

void OverlayService::HandleChatMessage(const PacketEvent<SendChatMessageRequest>& acMessage) const noexcept
{
    NotifyChatMessageBroadcast notifyMessage;
    notifyMessage.PlayerName = acMessage.pPlayer->GetUsername();

    // TODO: std regex is slow
    std::regex escapeHtml{"<[^>]+>\\s+(?=<)|<[^>]+>"};
    notifyMessage.ChatMessage = std::regex_replace(acMessage.Packet.ChatMessage, escapeHtml, "");

    GameServer::Get()->SendToPlayers(notifyMessage);
}

void OverlayService::OnPlayerDialogue(const PacketEvent<PlayerDialogueRequest>& acMessage) const noexcept
{
    auto& message = acMessage.Packet;

    NotifyPlayerDialogue notify;
    notify.Text = acMessage.pPlayer->GetUsername() + ": " + message.Text;

    auto& party = acMessage.pPlayer->GetParty();

    // TODO(cosideci): exclude player?
    GameServer::Get()->SendToParty(notify, party);
}

#if 0
#include <Components.h>
#include <Events/PlayerEnterWorldEvent.h>

void OverlayService::HandlePlayerJoin(const PlayerEnterWorldEvent& acEvent) const noexcept
{
    const Script::Player cPlayer(acEvent.Entity, m_world);

    auto& playerComponent = m_world.get<PlayerComponent>(cPlayer.GetEntityHandle());

    spdlog::info("[SERVER] PlayerId: {} - ConnectionId: {}", cPlayer.GetId(), playerComponent.ConnectionId);
    if (playerComponent.Character)
    {
        spdlog::info("[SERVER] CharacterId: {}", playerComponent.Character.value());
    }

    //TODO 
    //Send netid, username and default level to all client except to the one that just joined
}
#endif
