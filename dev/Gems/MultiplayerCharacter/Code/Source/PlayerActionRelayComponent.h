#pragma once
#include <AzCore/Component/Component.h>
#include <MultiplayerCharacter/PlayerControlsRequestBus.h>
#include <AzFramework/Network/NetBindable.h>
#include <MultiplayerCharacter/LocalPlayerBodyNotificationBus.h>

namespace MultiplayerCharacter
{
    // Sends local client player input to the server
    class PlayerActionRelayComponent
        : public AZ::Component
        , public PlayerControlsRequestBus::Handler
        , public AzFramework::NetBindable
        , public LocalPlayerBodyNotificationBus::Handler
    {
    public:
        AZ_COMPONENT(PlayerActionRelayComponent,
            "{07CB07C7-4B69-4FA4-837B-070D780EF3DA}",
            NetBindable);

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* ref);

        static void GetProvidedServices(
            AZ::ComponentDescriptor::DependencyArrayType& prov);

        // LocalPlayerBodyNotificationBus implementation
        void OnClientInputConnected() override;

        // NetBindable overrides
        GridMate::ReplicaChunkPtr GetNetworkBinding() override;
        void SetNetworkBinding(
            GridMate::ReplicaChunkPtr chunk) override;
        void UnbindFromNetwork() override;

    protected:
        // PlayerControlsRequestBus interface
        void MoveForward(ActionState state) override;
        void MoveBackward(ActionState state) override;
        void StrafeLeft(ActionState state) override;
        void StrafeRight(ActionState state) override;
        void Turn(float amount) override;

    private:
        bool MoveForwardRpc(ActionState state,
            const GridMate::RpcContext& rc);
        bool MoveBackwardRpc(ActionState state,
            const GridMate::RpcContext& rc);
        bool StrafeLeftRpc(ActionState state,
            const GridMate::RpcContext& rc);
        bool StrafeRightRpc(ActionState state,
            const GridMate::RpcContext& rc);
        bool TurnRpc(float amount,
            const GridMate::RpcContext& rc);

        class Chunk;
        GridMate::ReplicaChunkPtr m_chunk;
    };
}