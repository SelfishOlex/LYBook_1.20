#include "PlayerAoIComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <GridMate/Replica/ReplicaFunctions.h>
#include <ISystem.h>
#include <AzFramework/Network/InterestManagerComponent.h>

using namespace GridMate;
using namespace MyProject;
using namespace MultiplayerCharacter;

class PlayerAoIComponent::Chunk : public ReplicaChunkBase
{
public:
    AZ_CLASS_ALLOCATOR(Chunk, AZ::SystemAllocator, 0);
    Chunk() = default;
    static const char* GetChunkName()
    {
        return "PlayerAoIComponent::Chunk";
    }
    bool IsReplicaMigratable() override { return true; }
};

void PlayerAoIComponent::Activate()
{
#if defined(DEDICATED_SERVER)
    const AZ::EntityId self = GetEntityId();
    ServerPlayerBodyBus::Handler::BusConnect(self);
    AZ::TransformNotificationBus::Handler::BusConnect(self);
#endif
}

void PlayerAoIComponent::Deactivate()
{
#if defined(DEDICATED_SERVER)
    MultiplayerCharacter::
            ServerPlayerBodyBus::Handler::BusDisconnect();
    AZ::TransformNotificationBus::Handler::BusDisconnect();
#endif
}

void PlayerAoIComponent::SetAssociatedPlayerId(
    GridMate::MemberIDCompact player)
{
    m_playerId = player;
    ProximityInterestHandler* proximityInterest = nullptr;
    using Interest = AzFramework::InterestManagerRequestsBus;
    Interest::BroadcastResult(proximityInterest,
        &Interest::Events::GetProximityInterest);
    if (proximityInterest)
    {
        m_aoi = proximityInterest->CreateRule(m_playerId);

        AZ::Vector3 position;
        AZ::TransformBus::EventResult(position, GetEntityId(),
            &AZ::TransformBus::Events::GetWorldTranslation);
        UpdateAreaOfInterest(position);
    }
}

void PlayerAoIComponent::OnTransformChanged(
    const AZ::Transform&, const AZ::Transform& world)
{
    UpdateAreaOfInterest(world.GetPosition());
}

void PlayerAoIComponent::UpdateAreaOfInterest(
        const AZ::Vector3& pos)
{
    if (!m_aoi || pos == m_lastPosition) return;
    m_lastPosition = pos;

    m_aoi->Set(AZ::Aabb::CreateCenterRadius(pos, m_interestRange));
}

void PlayerAoIComponent::Reflect(AZ::ReflectContext* rc)
{
    auto& descTable = ReplicaChunkDescriptorTable::Get();
    if (!descTable.FindReplicaChunkDescriptor(
        ReplicaChunkClassId(Chunk::GetChunkName())))
    {
        descTable.RegisterChunkType<Chunk>();
    }

    auto sc = azrtti_cast<AZ::SerializeContext*>(rc);
    if (!sc) return;

    sc->Class<PlayerAoIComponent, Component>()
        ->Version(1)
        ->Field("Interest Range",
            &PlayerAoIComponent::m_interestRange);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    // reflection of this component for Lumberyard Editor
    ec->Class<PlayerAoIComponent>("Player Area of Interest",
        "[tells GridMate about player's area of interest]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("Game", 0x232b318c))
        ->Attribute(Category, "My Project")
        ->DataElement(0, &PlayerAoIComponent::m_interestRange,
            "Interest Range", "[How far player's vision extends]");
}

ReplicaChunkPtr PlayerAoIComponent::GetNetworkBinding()
{
    m_chunk = GridMate::CreateReplicaChunk<Chunk>();
    m_chunk->SetHandler(this);
    return m_chunk;
}

void PlayerAoIComponent::SetNetworkBinding(ReplicaChunkPtr chunk)
{
    chunk->SetHandler(this);
    m_chunk = chunk;
}

void PlayerAoIComponent::UnbindFromNetwork()
{
    m_chunk = nullptr;
    m_aoi = nullptr;
}