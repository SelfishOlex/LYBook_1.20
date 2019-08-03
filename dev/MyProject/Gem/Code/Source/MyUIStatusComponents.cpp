#include "MyUIStatusComponents.h"
#include <AzCore/Serialization/EditContext.h>
#include <LyShine/Bus/UiInteractableBus.h>

using namespace MyProject;
using namespace MultiplayerCharacter;

void MyUIForwardComponent::Reflect(AZ::ReflectContext* rc)
{
    auto sc = azrtti_cast<AZ::SerializeContext*>(rc);
    if (!sc) return;

    sc->Class<MyUIForwardComponent, Component>()->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    // reflection of this component for Lumberyard Editor
    ec->Class<MyUIForwardComponent>("My Forward Status",
        "[interacts with a button to show forward key]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("UI", 0x27ff46b0))
        ->Attribute(Category, "My Project");
}

void MyUIForwardComponent::MoveForward(ActionState state)
{
    UiInteractableBus::Event(GetEntityId(),
        &UiInteractableBus::Events::SetIsHandlingEvents,
        state != ActionState::Started);
}

void MyUIBackwardComponent::Reflect(AZ::ReflectContext* rc)
{
    auto sc = azrtti_cast<AZ::SerializeContext*>(rc);
    if (!sc) return;

    sc->Class<MyUIBackwardComponent, Component>()->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    // reflection of this component for Lumberyard Editor
    ec->Class<MyUIBackwardComponent>("My Backward Status",
        "[interacts with a button to show backward key]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("UI", 0x27ff46b0))
        ->Attribute(Category, "My Project");
}

void MyUIBackwardComponent::MoveBackward(ActionState state)
{
    UiInteractableBus::Event(GetEntityId(),
        &UiInteractableBus::Events::SetIsHandlingEvents,
        state != ActionState::Started);
}

void MyUIStrafeLeftComponent::Reflect(AZ::ReflectContext* rc)
{
    auto sc = azrtti_cast<AZ::SerializeContext*>(rc);
    if (!sc) return;

    sc->Class<MyUIStrafeLeftComponent, Component>()->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    // reflection of this component for Lumberyard Editor
    ec->Class<MyUIStrafeLeftComponent>("My Strafe Left Status",
        "[interacts with a button to show strafe left key]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("UI", 0x27ff46b0))
        ->Attribute(Category, "My Project");
}

void MyUIStrafeLeftComponent::StrafeLeft(ActionState state)
{
    UiInteractableBus::Event(GetEntityId(),
        &UiInteractableBus::Events::SetIsHandlingEvents,
        state != ActionState::Started);
}

void MyUIStrafeRightComponent::Reflect(AZ::ReflectContext* rc)
{
    auto sc = azrtti_cast<AZ::SerializeContext*>(rc);
    if (!sc) return;

    sc->Class<MyUIStrafeRightComponent, Component>()->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    // reflection of this component for Lumberyard Editor
    ec->Class<MyUIStrafeRightComponent>("My Strafe Right Status",
        "[interacts with a button to show strafe right key]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("UI", 0x27ff46b0))
        ->Attribute(Category, "My Project");
}

void MyUIStrafeRightComponent::StrafeRight(ActionState state)
{
    UiInteractableBus::Event(GetEntityId(),
        &UiInteractableBus::Events::SetIsHandlingEvents,
        state != ActionState::Started);
}