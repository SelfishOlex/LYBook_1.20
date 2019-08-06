#pragma once
#include <AzCore/Component/Component.h>
#include <AzFramework/Network/NetBindable.h>
#include <AzCore/Component/TickBus.h>

namespace MultiplayerCloseAllNetworkPeers
{
    enum class CloseResult
    {
        CloseImmediately,
        ClosingViaWorkerComponent,
    };

    class CloseAllRequestInterface
        : public AZ::EBusTraits
    {
    public:
        virtual ~CloseAllRequestInterface() = default;
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        virtual CloseResult CloseAll() = 0;
    };

    using CloseAllRequestBus = AZ::EBus<CloseAllRequestInterface>;

    class WorkerComponent
        : public AZ::Component
        , public CloseAllRequestBus::Handler
        , public AzFramework::NetBindable
        , public AZ::TickBus::Handler
    {
    public:
        AZ_COMPONENT(WorkerComponent, "{33C9FC43-8756-48F3-A925-C04A5BD4FF6A}", NetBindable);

        static void Reflect(AZ::ReflectContext* context);

    protected:
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        // CloseAllRequestBus
        CloseResult CloseAll() override;

        // NetBindable
        GridMate::ReplicaChunkPtr GetNetworkBinding() override;
        void SetNetworkBinding(
            GridMate::ReplicaChunkPtr chunk) override;
        void UnbindFromNetwork() override;

        // TickBus
        void OnTick(float, AZ::ScriptTimePoint time) override;

        // RPC callback
        bool OnCloseAll(const GridMate::RpcContext& rc);

    private:
        class Chunk;
        GridMate::ReplicaChunkPtr m_chunk;

        bool m_isShuttinDown = false;
        AZ::u32 m_shutdownCountdown = 0;
        AZ::u32 m_ticksBeforeShutdown = 20;
    };
}
