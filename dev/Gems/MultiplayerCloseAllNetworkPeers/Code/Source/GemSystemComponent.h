
#pragma once

#include <AzCore/Component/Component.h>

#include <MultiplayerCloseAllNetworkPeers/CloseNetworkPeersBus.h>
#include <AzCore/Component/EntityBus.h>
#include <AzFramework/Entity/GameEntityContextBus.h>
#include <GridMate/Session/Session.h>
#include <CrySystemBus.h>

namespace MultiplayerCloseAllNetworkPeers
{
    class GemSystemComponent
        : public AZ::Component
        , protected CloseNetworkPeersRequestBus::Handler
        , public AZ::EntityBus::MultiHandler
        , AzFramework::GameEntityContextEventBus::Handler
        , public GridMate::SessionEventBus::Handler
        , public CrySystemEventBus::Handler
    {
    public:
        AZ_COMPONENT(GemSystemComponent, "{30168C7B-5DCE-4404-83DC-6349CB07A60F}");

        static void Reflect(AZ::ReflectContext* context);

    protected:
        // CloseNetworkPeersRequestBus interface implementation
        void CloseAll() override;

        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        // GameEntityContextEventBus
        void OnGameEntitiesStarted() override;
        void OnGameEntitiesReset() override;

        // SessionEventBus
        void OnSessionHosted(GridMate::GridSession* session) override;
        void OnSessionDelete(GridMate::GridSession* session) override;
        void OnSessionEnd(GridMate::GridSession* session) override;

        // CrySystemEventBus
        void OnCrySystemInitialized(ISystem&, const SSystemInitParams&) override;

        void CreateWorkerEntity();

    private:
        AZ::Entity* m_workerEntity = nullptr;
        bool m_isHost = false;
        bool m_isMapLoaded = false;
    };
}
