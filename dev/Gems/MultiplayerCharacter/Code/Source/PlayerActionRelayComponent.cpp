#include "PlayerActionRelayComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <GridMate/Replica/ReplicaFunctions.h>

using namespace AZ;
using namespace GridMate;
using namespace MultiplayerCharacter;

class PlayerActionRelayComponent::Chunk : public ReplicaChunkBase
{
public:
    GM_CLASS_ALLOCATOR(Chunk);
    Chunk() = default;

    static const char* GetChunkName()
    {
        return "PlayerActionRelayComponent::Chunk";
    }
    bool IsReplicaMigratable() override { return true; }

    GridMate::Rpc<RpcArg<ActionState>>::BindInterface<
        PlayerActionRelayComponent,
        &PlayerActionRelayComponent::MoveForwardRpc>
    m_forwardRpc = {"Forward"};

    GridMate::Rpc<RpcArg<ActionState>>::BindInterface<
        PlayerActionRelayComponent,
        &PlayerActionRelayComponent::MoveBackwardRpc>
    m_backwardRpc = {"Backward"};

    GridMate::Rpc<RpcArg<ActionState>>::BindInterface<
        PlayerActionRelayComponent,
        &PlayerActionRelayComponent::StrafeLeftRpc>
    m_strafeLeftRpc = {"StrafeLeft"};

    GridMate::Rpc<RpcArg<ActionState>>::BindInterface<
        PlayerActionRelayComponent,
        &PlayerActionRelayComponent::StrafeRightRpc>
    m_strafeRightRpc = {"StrafeRight"};

    GridMate::Rpc<RpcArg<float>>::BindInterface<
        PlayerActionRelayComponent,
        &PlayerActionRelayComponent::TurnRpc>
    m_turnRpc = {"Turn"};
};

void PlayerActionRelayComponent::Activate()
{
#if !defined(DEDICATED_SERVER)
    LocalPlayerBodyNotificationBus::Handler::BusConnect(
        GetEntityId());
    AZ_Printf("Book", "Relay bus connected %llu",
        (AZ::u64)GetEntityId());
#endif // DEDICATED_SERVER
}

void PlayerActionRelayComponent::Deactivate()
{
#if !defined(DEDICATED_SERVER)
    LocalPlayerBodyNotificationBus::Handler::BusDisconnect();
    if (PlayerControlsRequestBus::Handler::BusIsConnected())
        PlayerControlsRequestBus::Handler::BusDisconnect();
#endif // DEDICATED_SERVER
}

void PlayerActionRelayComponent::Reflect(AZ::ReflectContext* ref)
{
    auto& descTable = ReplicaChunkDescriptorTable::Get();
    if (!descTable.FindReplicaChunkDescriptor(
        ReplicaChunkClassId(Chunk::GetChunkName())))
    {
        descTable.RegisterChunkType<Chunk>();
    }

    auto sc = azrtti_cast<AZ::SerializeContext*>(ref);
    if (!sc) return;

    using Self = PlayerActionRelayComponent; // to shorten lines
    sc->Class<PlayerActionRelayComponent, Component>()
        ->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    using namespace AZ::Edit::UIHandlers;
    // reflection of this component for Lumberyard Editor
    ec->Class<PlayerActionRelayComponent>("Player Action Relay",
        "[Relays client commands to the server]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("Game", 0x232b318c))
        ->Attribute(Category, "Multiplayer Character");
}

void PlayerActionRelayComponent::GetProvidedServices(
    AZ::ComponentDescriptor::DependencyArrayType& prov)
{
    prov.push_back(AZ_CRC("PlayerActionRelayService"));
}

void PlayerActionRelayComponent::OnClientInputConnected()
{
    PlayerControlsRequestBus::Handler::BusConnect(GetEntityId());
    AZ_Printf("Book", "Relay connected");
}

ReplicaChunkPtr PlayerActionRelayComponent::GetNetworkBinding()
{
    m_chunk = GridMate::CreateReplicaChunk<Chunk>();
    m_chunk->SetHandler(this);
    return m_chunk;
}

void PlayerActionRelayComponent::SetNetworkBinding(
    GridMate::ReplicaChunkPtr chunk)
{
    m_chunk = chunk;
    m_chunk->SetHandler(this);
}

void PlayerActionRelayComponent::UnbindFromNetwork()
{
    if (m_chunk)
    {
        m_chunk->SetHandler(nullptr);
        m_chunk = nullptr;
    }
}

void PlayerActionRelayComponent::MoveForward(ActionState state)
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
        c->m_forwardRpc(state);
}

void PlayerActionRelayComponent::MoveBackward(ActionState state)
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
        c->m_backwardRpc(state);
}

void PlayerActionRelayComponent::StrafeLeft(ActionState state)
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
        c->m_strafeLeftRpc(state);
}

void PlayerActionRelayComponent::StrafeRight(ActionState state)
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
        c->m_strafeRightRpc(state);
}

void PlayerActionRelayComponent::Turn(float amount)
{
    if (Chunk* c = static_cast<Chunk*>(m_chunk.get()))
        c->m_turnRpc(amount);
}

bool PlayerActionRelayComponent::MoveForwardRpc(
    ActionState state, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsRequestBus::Event(GetEntityId(),
        &ServerPlayerControlsRequestBus::Events::MoveForward,
        state);
    return false; // don't pass back to clients
}

bool PlayerActionRelayComponent::MoveBackwardRpc(
    ActionState state, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsRequestBus::Event(GetEntityId(),
        &ServerPlayerControlsRequestBus::Events::MoveBackward,
        state);
    return false; // don't pass back to clients
}

bool PlayerActionRelayComponent::StrafeLeftRpc(
    ActionState state, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsRequestBus::Event(GetEntityId(),
        &ServerPlayerControlsRequestBus::Events::StrafeLeft,
        state);
    return false; // don't pass back to clients
}

bool PlayerActionRelayComponent::StrafeRightRpc(
    ActionState state, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsRequestBus::Event(GetEntityId(),
        &ServerPlayerControlsRequestBus::Events::StrafeRight,
        state);
    return false; // don't pass back to clients
}

bool PlayerActionRelayComponent::TurnRpc(
    float amount, const GridMate::RpcContext& rc)
{
    ServerPlayerControlsRequestBus::Event(GetEntityId(),
        &ServerPlayerControlsRequestBus::Events::Turn, amount);
    return false; // don't pass back to clients
}