#include "PointOfInterestComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <GridMate/Replica/ReplicaFunctions.h>
#include <AzCore/Component/TransformBus.h>
#include <ISystem.h>
#include <AzFramework/Network/InterestManagerComponent.h>

using namespace AZ;
using namespace GridMate;
using namespace MyProject;

class PointOfInterestComponent::Chunk : public ReplicaChunkBase
{
public:
    AZ_CLASS_ALLOCATOR(Chunk, AZ::SystemAllocator, 0);
    Chunk() = default;
    static const char* GetChunkName()
    {
        return "PointOfInterestComponent::Chunk";
    }
    bool IsReplicaMigratable() override { return true; }

#if defined(DEDICATED_SERVER)
    void OnReplicaActivate(const ReplicaContext&) override
    {
        PointOfInterestComponent* handler =
            static_cast<PointOfInterestComponent*>(GetHandler());
        if (handler)
        {
            handler->InitInterestAttribute(GetReplicaId());
        }
    }
#endif
};

void PointOfInterestComponent::Activate()
{
#if defined(DEDICATED_SERVER)
    TransformNotificationBus::Handler::BusConnect(GetEntityId());
#endif
}

void PointOfInterestComponent::Deactivate()
{
#if defined(DEDICATED_SERVER)
    TransformNotificationBus::Handler::BusDisconnect();
#endif
}

void PointOfInterestComponent::OnTransformChanged(
    const AZ::Transform&, const AZ::Transform& world)
{
    UpdatePointOfInterest(world.GetTranslation());
}

void PointOfInterestComponent::InitInterestAttribute(
    GridMate::ReplicaId id)
{
    ProximityInterestHandler* handler = nullptr;
    using Interest = AzFramework::InterestManagerRequestsBus;
    Interest::BroadcastResult(handler,
        &Interest::Events::GetProximityInterest);
    if (handler)
    {
        m_attr = handler->CreateAttribute(m_chunk->GetReplicaId());

        AZ::Vector3 position;
        AZ::TransformBus::EventResult(position, GetEntityId(),
            &AZ::TransformBus::Events::GetWorldTranslation);

        UpdatePointOfInterest(position);
    }
}

void PointOfInterestComponent::UpdatePointOfInterest(
            const AZ::Vector3& pos)
{
    if (!m_attr || pos.GetDistanceSq(m_lastPosition) < 1) return;
    m_lastPosition = pos;

    const AZ::Aabb box = AZ::Aabb::CreateCenterRadius(pos, 0.f);
    m_attr->Set(box);
}

void PointOfInterestComponent::Reflect(AZ::ReflectContext* rc)
{
    auto& descTable = ReplicaChunkDescriptorTable::Get();
    if (!descTable.FindReplicaChunkDescriptor(
        ReplicaChunkClassId(Chunk::GetChunkName())))
    {
        descTable.RegisterChunkType<Chunk>();
    }

    auto sc = azrtti_cast<AZ::SerializeContext*>(rc);
    if (!sc) return;

    sc->Class<PointOfInterestComponent, Component>()
        ->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    // reflect this component for Lumberyard Editor
    ec->Class<PointOfInterestComponent>("Point of Interest",
        "[tells GridMate when a player should see this entity]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("Game", 0x232b318c))
        ->Attribute(Category, "My Project");
}

ReplicaChunkPtr PointOfInterestComponent::GetNetworkBinding()
{
    m_chunk = GridMate::CreateReplicaChunk<Chunk>();
    m_chunk->SetHandler(this);
    return m_chunk;
}

void PointOfInterestComponent::SetNetworkBinding(
            ReplicaChunkPtr chunk)
{
    chunk->SetHandler(this);
    m_chunk = chunk;
}

void PointOfInterestComponent::UnbindFromNetwork()
{
    m_chunk = nullptr;
    m_attr = nullptr;
}