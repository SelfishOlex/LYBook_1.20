#pragma once
#include <AzCore/Component/Component.h>
#include <AzFramework/Network/NetBindable.h>
#include <GridMate/Replica/Interest/ProximityInterestHandler.h>
#include <AzCore/Component/TransformBus.h>
#include <MultiplayerCharacter/ServerPlayerBodyBus.h>

namespace MyProject
{
    // Player's Area of Interest for GridMate's Interest Manager
    // Player will only see object's with AoI within theirs
    class PlayerAoIComponent : public AZ::Component
        , public AzFramework::NetBindable
        , public AZ::TransformNotificationBus::Handler
        , public MultiplayerCharacter::ServerPlayerBodyBus::Handler
    {
    public:
        AZ_COMPONENT(PlayerAoIComponent,
            "{D51D6573-BEE7-42DF-A219-1D54DAFC09D1}",
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

        // ServerPlayerBodyBus overrides
        void SetAssociatedPlayerId(
            GridMate::MemberIDCompact player) override;

        // NetBindable overrides
        GridMate::ReplicaChunkPtr GetNetworkBinding() override;
        void SetNetworkBinding(
            GridMate::ReplicaChunkPtr chunk) override;
        void UnbindFromNetwork() override;

    private:
        class Chunk;
        GridMate::ReplicaChunkPtr m_chunk = nullptr;
        GridMate::ProximityInterestRule::Ptr m_aoi = nullptr;
        GridMate::MemberIDCompact m_playerId = 0;

        AZ::Vector3 m_lastPosition = AZ::Vector3::CreateZero();
        void UpdateAreaOfInterest(const AZ::Vector3& pos);

        float m_interestRange = 20.f;
    };
}