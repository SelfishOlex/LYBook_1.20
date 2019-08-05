#include "LocalClientComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Math/Transform.h>
#include <ISystem.h>
#include <INetwork.h>
#include <AzFramework/Network/NetBindingHandlerBus.h>
#include <MultiplayerCharacter/MultiplayerCharacterBus.h>
#include <MultiplayerCharacter/LocalPlayerBodyNotificationBus.h>

using namespace AZ;
using namespace AzFramework;
using namespace GridMate;
using namespace MultiplayerCharacter;

void LocalClientComponent::Reflect(AZ::ReflectContext* context)
{
    if (auto sc = azrtti_cast<SerializeContext*>(context))
    {
        using Self = LocalClientComponent; // to shorten lines
        sc->Class<Self, Component>()
          ->Version(1)
          ->Field("Camera Offset", &Self::m_cameraOffset);

        if (EditContext* ec = sc->GetEditContext())
        {
            ec->Class<Self>("Local Client", "[description]")
              ->ClassElement(
                  Edit::ClassElements::EditorData, "")
              ->Attribute(Edit::Attributes::Category,
                          "Multiplayer Character")
              ->Attribute(Edit::Attributes::
                  AppearsInAddComponentMenu, AZ_CRC("Game"))
              ->DataElement(nullptr, &Self::m_cameraOffset,
                        "Camera Offset", "");
        }
    }
}

void LocalClientComponent::Activate()
{
    LocalClientBus::Handler::BusConnect();
}

void LocalClientComponent::Deactivate()
{
    if (LocalClientBus::Handler::BusIsConnected())
    {
        LocalClientBus::Handler::BusDisconnect();
    }
}

void LocalClientComponent::AttachToBody(
    GridMate::MemberIDCompact playerId,
    const AZ::EntityId& body)
{
    MemberIDCompact self;
    MultiplayerCharacterRequestBus::BroadcastResult(self,
        &MultiplayerCharacterRequestBus::Events::GetLocal);

    AZ_Printf("Book", "AttachToBody %u local %u",
        playerId, self);

    if (self == 0 || self != playerId) return;

    TransformBus::Event(GetEntityId(),
        &TransformBus::Events::SetParentRelative, body);

    Transform t = Transform::CreateTranslation(m_cameraOffset);
    TransformBus::Event(GetEntityId(),
        &TransformBus::Events::SetLocalTM, t);
        
    AZ_Printf("Book", "Relay notified %llu", (AZ::u64)body);
    LocalPlayerBodyNotificationBus::Event(body,
        &LocalPlayerBodyNotificationBus::Events::
            OnClientInputConnected);
}