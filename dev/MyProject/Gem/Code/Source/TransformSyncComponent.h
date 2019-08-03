#pragma once
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TransformBus.h>
#include <AzFramework/Network/NetBindable.h>

namespace MyProject
{
    // An example of syncing TransformComponent
    class TransformSyncComponent
        : public AZ::Component
        , public AZ::TransformNotificationBus::Handler
        , public AzFramework::NetBindable
    {
    public:
        AZ_COMPONENT(TransformSyncComponent,
            "{0EC069DF-A5B1-4EC7-8779-59AD3D24C005}",
            NetBindable);

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* reflect);

        // TransformNotificationBus implementation
        void OnTransformChanged(
            const AZ::Transform& local,
            const AZ::Transform& world) override;
        void OnParentChanged(AZ::EntityId oldParent,
            AZ::EntityId newParent) override;

        // NetBindable implementation
        GridMate::ReplicaChunkPtr GetNetworkBinding() override;
        void SetNetworkBinding(
            GridMate::ReplicaChunkPtr chunk) override;
        void UnbindFromNetwork() override;

    protected:
        // GridMate callbacks
        void OnNewTransform(const AZ::Transform& t,
            const GridMate::TimeContext& tc);
        void OnNewParent(const AZ::EntityId& parent,
            const GridMate::TimeContext& tc);

    private:
        bool m_isAuthoritative = false;

        class Chunk;
        GridMate::ReplicaChunkPtr m_chunk;
    };
}