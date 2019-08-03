#include "TransformSyncComponent.h"
#include <AzCore/Serialization/EditContext.h>
// For GridMate::CreateReplicaChunk
#include <GridMate/Replica/ReplicaFunctions.h>
// NetQuery::IsEntityAuthoritative
#include <AzFramework/Network/NetBindingHandlerBus.h>
// For AZ::EntityId GridMate marshaler
#include <AzFramework/Network/EntityIdMarshaler.h>
// For AZ::Transform GridMate marshaler
#include <GridMate/Serialize/MathMarshal.h>

using namespace AZ;
using namespace GridMate;
using namespace MyProject;

class TransformSyncComponent::Chunk
    : public GridMate::ReplicaChunkBase
{
public:
    AZ_CLASS_ALLOCATOR(Chunk, GridMate::GridMateAllocator, 0);

    Chunk() : m_transform("Transform")
            , m_parent("Parent") {}

    bool IsReplicaMigratable() override { return true; }
    static const char* GetChunkName()
    {
        return "TransformSyncComponent::Chunk";
    }

    GridMate::DataSet<AZ::Transform>::BindInterface<
        TransformSyncComponent,
        &TransformSyncComponent::OnNewTransform> m_transform;
    GridMate::DataSet<AZ::EntityId>::BindInterface<
        TransformSyncComponent,
        &TransformSyncComponent::OnNewParent> m_parent;
};

void TransformSyncComponent::Activate()
{
    using namespace AzFramework;
    const AZ::EntityId self = GetEntityId();
    // figure out if we are server or client
    m_isAuthoritative = NetQuery::IsEntityAuthoritative(self);

    if (m_isAuthoritative)
        TransformNotificationBus::Handler::BusConnect(self);
}

void TransformSyncComponent::Deactivate()
{
    if (m_isAuthoritative)
        TransformNotificationBus::Handler::BusDisconnect();
}

void TransformSyncComponent::OnTransformChanged(
    const AZ::Transform& local, const AZ::Transform& world)
{
    AZ_UNUSED(local);
    if (auto chunk = static_cast<Chunk*>(m_chunk.get()))
    {
        chunk->m_transform.Set(world);
    }
}

void TransformSyncComponent::OnParentChanged(
    AZ::EntityId oldParent, AZ::EntityId newParent)
{
    AZ_UNUSED(oldParent);
    if (auto chunk = static_cast<Chunk*>(m_chunk.get()))
    {
        chunk->m_parent.Set(newParent);
    }
}

void TransformSyncComponent::OnNewTransform(
    const AZ::Transform& t, const GridMate::TimeContext&)
{
    TransformBus::Event(GetEntityId(),
        &TransformBus::Events::SetWorldTM, t);
}

void TransformSyncComponent::OnNewParent(
    const AZ::EntityId& parent, const GridMate::TimeContext&)
{
    TransformBus::Event(GetEntityId(),
        &TransformBus::Events::SetParent, parent);
}

void TransformSyncComponent::Reflect(AZ::ReflectContext* reflect)
{
    // We need to register the chunk type,
    // and this would be a good time to do so.
    auto& descTable = ReplicaChunkDescriptorTable::Get();
    if (!descTable.FindReplicaChunkDescriptor(
        ReplicaChunkClassId(Chunk::GetChunkName())))
    {
        descTable.RegisterChunkType<Chunk>();
    }

    auto sc = azrtti_cast<AZ::SerializeContext*>(reflect);
    if (!sc) return;

    sc->Class<TransformSyncComponent, Component, NetBindable>()
        ->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    // reflection of this component for Lumberyard Editor
    ec->Class<TransformSyncComponent>("Transform Sync",
        "[Replicates TransformComponent over network]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("Game", 0x232b318c))
        ->Attribute(Category, "My Project");
}

GridMate::ReplicaChunkPtr
    TransformSyncComponent::GetNetworkBinding()
{
    m_chunk = GridMate::CreateReplicaChunk<Chunk>();
    m_chunk->SetHandler(this);
    return m_chunk;
}

void TransformSyncComponent::SetNetworkBinding(
    GridMate::ReplicaChunkPtr chunk)
{
    if (chunk != nullptr)
    {
        chunk->SetHandler(this);
        m_chunk = chunk;
    }
}

void TransformSyncComponent::UnbindFromNetwork()
{
    if (m_chunk)
    {
        m_chunk->SetHandler(nullptr);
        m_chunk = nullptr;
    }
}