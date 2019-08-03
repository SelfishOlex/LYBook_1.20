#include "CameraControlsComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/Component/TransformBus.h>

using namespace AZ;
using namespace MultiplayerCharacter;

void CameraControlsComponent::Activate()
{
    PlayerControlsRequestBus::Handler::BusConnect(GetEntityId());
}

void CameraControlsComponent::Deactivate()
{
    PlayerControlsRequestBus::Handler::BusDisconnect();
}

void CameraControlsComponent::Reflect(AZ::ReflectContext* ref)
{
    auto sc = azrtti_cast<AZ::SerializeContext*>(ref);
    if (!sc) return;

    // to shorten lines
    using Self = CameraControlsComponent;
    sc->Class<CameraControlsComponent, Component>()
        ->Field("LookUp Speed", &Self::m_lookUpSpeed)
        ->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    using namespace AZ::Edit::UIHandlers;
    // reflection of this component for Lumberyard Editor
    ec->Class<CameraControlsComponent>("Camera Controls",
        "[Implements various player camera controls]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("Game", 0x232b318c))
        ->Attribute(Category, "Multiplayer Character")
        ->DataElement(Default, &Self::m_lookUpSpeed,
            "LookUp Speed", "");
}

void CameraControlsComponent::LookUpOrDown(float amount)
{
    m_rotY = amount * m_lookUpSpeed;
    SetRotation();
}

void CameraControlsComponent::SetRotation()
{
    AZ::EntityId parent;
    AZ::TransformBus::EventResult(parent, GetEntityId(),
        &AZ::TransformBus::Events::GetParentId);

    TransformBus::Event(parent,
        &TransformBus::Events::SetLocalRotationQuaternion,
        Quaternion::CreateRotationX(m_rotY));
}