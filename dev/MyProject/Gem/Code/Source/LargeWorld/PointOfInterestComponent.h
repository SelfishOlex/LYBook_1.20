#pragma once
#include <AzCore/Component/Component.h>
#include <AzFramework/Network/NetBindable.h>
#include <GridMate/Replica/Interest/ProximityInterestHandler.h>
#include <AzCore/Component/TransformBus.h>
#include <AzFramework/Network/InterestManagerComponent.h>

namespace MyProject
{
    // Adds a network Area of Interest for GridMate
    // An object whose area of interest is not within
    // a player's area of interest will not appear for them
    class PointOfInterestComponent : public AZ::Component
        , public AzFramework::NetBindable
        , public AZ::TransformNotificationBus::Handler
    {
    public:
        AZ_COMPONENT(PointOfInterestComponent,
            "{3E6559BE-58F4-4325-A3ED-2125CFA77A19}",
            NetBindable);

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* rc);

    protected:
        // TransformNotificationBus overrides
        void OnTransformChanged(
            const AZ::Transform& local,
            const AZ::Transform& world) override;

        // NetBindable overrides
        GridMate::ReplicaChunkPtr GetNetworkBinding() override;
        void SetNetworkBinding(
            GridMate::ReplicaChunkPtr chunk) override;
        void UnbindFromNetwork() override;

    private:
        class Chunk;
        GridMate::ReplicaChunkPtr m_chunk = nullptr;
        GridMate::ProximityInterestAttribute::Ptr m_attr = nullptr;

        void InitInterestAttribute(GridMate::ReplicaId id);
        AZ::Vector3 m_lastPosition = AZ::Vector3::CreateZero();
        void UpdatePointOfInterest(const AZ::Vector3& pos);
    };
}