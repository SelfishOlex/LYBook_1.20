#pragma once
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <GridMate/Replica/ReplicaCommon.h>
#include <AzFramework/Network/NetBindable.h>
#include <MultiplayerCharacter/ServerPlayerBodyBus.h>

namespace MultiplayerCharacter
{
    class ServerAuthPlayerComponent
        : public AZ::Component
        , public AzFramework::NetBindable
        , public ServerPlayerBodyBus::Handler
    {
    public:
        AZ_COMPONENT(ServerAuthPlayerComponent,
            "{3097BBE1-CCAA-46B5-B027-121A4588C036}",
            NetBindable);
        static void Reflect(AZ::ReflectContext* context);
        
        void GetRequiredServices(
            AZ::ComponentDescriptor::DependencyArrayType& req)
        {
            req.push_back(AZ_CRC("PlayerActionRelayService"));
        }

        GridMate::ReplicaChunkPtr GetNetworkBinding() override;
        void SetNetworkBinding(
            GridMate::ReplicaChunkPtr chunk) override;
        void UnbindFromNetwork() override;

    protected:
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        // ServerPlayerBodyBus interface
        void SetAssociatedPlayerId(
            GridMate::MemberIDCompact player) override;

    private:
        class Chunk;
        GridMate::ReplicaChunkPtr m_chunk;

        void BroadcastNewBody();

#if defined(DEDICATED_SERVER)
        constexpr bool IsDedicated() const { return true; }
#else
        constexpr bool IsDedicated() const { return false; }
#endif
    };
}