#pragma once
#include <AzCore/Component/Component.h>
#include <AzFramework/Network/NetBindable.h>
#include <MultiplayerCharacter/PebbleSpawnerComponentBus.h>

namespace MultiplayerCharacter
{
    class PebbleSpawnerComponent
        : public AZ::Component
        , public PebbleSpawnerComponentBus::Handler
        , public AzFramework::NetBindable
    {
    public:
        AZ_COMPONENT(PebbleSpawnerComponent,
            "{A80FAC35-650C-42A9-8CC0-A3F1645AA7EA}",
            NetBindable);
        static void Reflect(AZ::ReflectContext* context);

        static void GetRequiredServices(
            AZ::ComponentDescriptor::DependencyArrayType& req)
        {
            req.push_back(AZ_CRC("SpawnerService"));
        }

    protected:
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        // NetBindable interface implementation
        GridMate::ReplicaChunkPtr GetNetworkBinding()
            override;
        void SetNetworkBinding(
            GridMate::ReplicaChunkPtr chunk) override;
        void UnbindFromNetwork() override;

        // RPC
        bool OnPebbleSpawn(
            AZ::Transform tm, const GridMate::RpcContext& rc);

        // PebbleSpawnerComponentBus
        void SpawnPebbleAt(const AZ::Transform& tm) override;

    private:
        class Chunk;
        GridMate::ReplicaChunkPtr m_chunk;
    };
}