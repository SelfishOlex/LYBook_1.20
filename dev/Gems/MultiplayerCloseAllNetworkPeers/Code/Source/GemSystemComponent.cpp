#include "MultiplayerCloseAllNetworkPeers_precompiled.h"
#include <AzCore/Serialization/EditContext.h>
#include "GemSystemComponent.h"
#include <INetwork.h>
#include <AzFramework/Network/NetBindingComponent.h>
#include "WorkerComponent.h"
#include "AzFramework/Entity/GameEntityContextBus.h"
#include <MultiplayerCloseAllNetworkPeers/ShutdownApplication.h>

using namespace AZ;
using namespace AzFramework;
using namespace GridMate;

namespace MultiplayerCloseAllNetworkPeers
{
    void GemSystemComponent::Reflect(ReflectContext* context)
    {
        if (auto sc = azrtti_cast<SerializeContext*>(context))
        {
            sc->Class<GemSystemComponent, Component>()
                ->Version(1);

            if (auto ec = sc->GetEditContext())
            {
                ec->Class<GemSystemComponent>("Close All Network Peers",
                    "[Closes all connected network peers]")
                ->ClassElement(Edit::ClassElements::EditorData, "")
                ->Attribute(Edit::Attributes::AppearsInAddComponentMenu,AZ_CRC("System"))
                ->Attribute(Edit::Attributes::AutoExpand, true);
            }
        }
    }

    void GemSystemComponent::CloseAll()
    {
        CloseResult result = CloseResult::CloseImmediately;
        CloseAllRequestBus::BroadcastResult(result, &CloseAllRequestBus::Events::CloseAll);

        if (result == CloseResult::CloseImmediately)
        {
            ShutdownApplication();
        }
    }

    void GemSystemComponent::Activate()
    {
        CloseNetworkPeersRequestBus::Handler::BusConnect();
        GameEntityContextEventBus::Handler::BusConnect();
        CrySystemEventBus::Handler::BusConnect();
    }

    void GemSystemComponent::Deactivate()
    {
        CloseNetworkPeersRequestBus::Handler::BusDisconnect();
        GameEntityContextEventBus::Handler::BusDisconnect();
        CrySystemEventBus::Handler::BusDisconnect();
    }

    void GemSystemComponent::OnGameEntitiesStarted()
    {
        AZ_TracePrintf("CloseAll", "OnGameEntitiesStarted");
        m_isMapLoaded = true;

        if (m_isHost && m_workerEntity == nullptr)
        {
            AZ_TracePrintf("CloseAll", "OnGameEntitiesStarted - Entity");
            CreateWorkerEntity();
        }
    }

    void GemSystemComponent::OnGameEntitiesReset()
    {
        m_workerEntity = nullptr;
        m_isMapLoaded = false;
    }

    void GemSystemComponent::OnSessionHosted(GridMate::GridSession* session)
    {
        AZ_TracePrintf("CloseAll", "OnSessionHosted");
        m_isHost = true;

        if (m_isMapLoaded && m_workerEntity == nullptr)
        {
            AZ_TracePrintf("CloseAll", "OnGameEntitiesStarted - Entity");
            CreateWorkerEntity();
        }
    }

    void GemSystemComponent::OnSessionDelete(
        GridMate::GridSession*)
    {
        m_isHost = false;
    }

    void GemSystemComponent::OnSessionEnd(
        GridMate::GridSession*)
    {
        m_isHost = false;
    }

    void GemSystemComponent::OnCrySystemInitialized(
        ISystem& system, const SSystemInitParams&)
    {
        AZ_TracePrintf("CloseAll", "OnCrySystemInitialized");

        SessionEventBus::Handler::BusConnect(
            system.GetINetwork()->GetGridMate());
    }

    void GemSystemComponent::CreateWorkerEntity()
    {
        m_workerEntity = aznew AZ::Entity("CloseAll Entity");
        if (m_workerEntity)
        {
            m_workerEntity->CreateComponent<NetBindingComponent>();
            m_workerEntity->CreateComponent<WorkerComponent>();
            m_workerEntity->Init();
            m_workerEntity->Activate();

            AZ::EntityBus::MultiHandler::BusConnect(
                m_workerEntity->GetId());
        }

        AZ_Assert(m_workerEntity != nullptr,
            "Could not create worker Entity.");
        AZ_Assert(m_workerEntity->GetState() ==
            AZ::Entity::ES_ACTIVE,
            "Worker entity could not be activated.");
    }
}
