#include "InputCaptureComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <MultiplayerCharacter/PlayerControlsRequestBus.h>
#include <AzFramework/Input/Devices/Keyboard/InputDeviceKeyboard.h>
#include <AzFramework/Input/Devices/Mouse/InputDeviceMouse.h>

using namespace AZ;
using namespace AzFramework;
using namespace MultiplayerCharacter;

void InputCaptureComponent::Activate()
{
    // Connect to receive input events
    InputChannelEventListener::Connect();
}

void InputCaptureComponent::Deactivate()
{
    // Disconnect as a good practice
    InputChannelEventListener::Disconnect();
}

void InputCaptureComponent::Reflect(AZ::ReflectContext* ref)
{
    auto sc = azrtti_cast<AZ::SerializeContext*>(ref);
    if (!sc) return;

    sc->Class<InputCaptureComponent, Component>()
        ->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    // reflection of this component for Lumberyard Editor
    ec->Class<InputCaptureComponent>("Player Input Capture",
        "[Captures input the C++ way]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("Game", 0x232b318c))
        ->Attribute(Category, "Multiplayer Character");
}

bool InputCaptureComponent::OnInputChannelEventFiltered(
    const AzFramework::InputChannel& inputChannel)
{
    const AzFramework::InputDeviceId deviceId =
        inputChannel.GetInputDevice().GetInputDeviceId();

    if (deviceId == AzFramework::InputDeviceKeyboard::Id)
        return OnKeyboardEvent(inputChannel);

    if (deviceId == AzFramework::InputDeviceMouse::Id)
        return OnMouseEvent(inputChannel);

    return false; // key not consumed
}

bool InputCaptureComponent::OnKeyboardEvent(
    const InputChannel& inputChannel)
{
    const InputChannelId inputType = inputChannel.
        GetInputChannelId();

    if (inputType == InputDeviceKeyboard::Key::AlphanumericW)
    {
        const bool pressed = !!inputChannel.GetValue();
        CheckAndUpdateForward(pressed);
        return true; // key consumed
    }

    if (inputType == InputDeviceKeyboard::Key::AlphanumericS)
    {
        const bool pressed = !!inputChannel.GetValue();
        CheckAndUpdateBackward(pressed);
        return true; // key consumed
    }

    if (inputType == InputDeviceKeyboard::Key::AlphanumericA)
    {
        const bool pressed = !!inputChannel.GetValue();
        CheckAndUpdateStrafeLeft(pressed);
        return true; // key consumed
    }

    if (inputType == InputDeviceKeyboard::Key::AlphanumericD)
    {
        const bool pressed = !!inputChannel.GetValue();
        CheckAndUpdateStrafeRight(pressed);
        return true; // key consumed
    }

    return false; // key not consumed
}

void InputCaptureComponent::CheckAndUpdateForward(bool pressed)
{
    if (m_isForwardPressed == pressed) return;

    PlayerControlsRequestBus::Broadcast(
        &PlayerControlsRequestBus::Events::MoveForward,
        pressed ? ActionState::Started : ActionState::Stopped);

    m_isForwardPressed = pressed;
}

void InputCaptureComponent::CheckAndUpdateBackward(bool pressed)
{
    if (m_isBackwardPressed == pressed) return;

    PlayerControlsRequestBus::Broadcast(
        &PlayerControlsRequestBus::Events::MoveBackward,
        pressed ? ActionState::Started : ActionState::Stopped);

    m_isBackwardPressed = pressed;
}

void InputCaptureComponent::CheckAndUpdateStrafeLeft(bool press)
{
    if (m_isStrafingLeftPressed == press) return;

    PlayerControlsRequestBus::Broadcast(
        &PlayerControlsRequestBus::Events::StrafeLeft,
        press ? ActionState::Started : ActionState::Stopped);

    m_isStrafingLeftPressed = press;
}

void InputCaptureComponent::CheckAndUpdateStrafeRight(bool press)
{
    if (m_isStrafingRightPressed == press) return;

    PlayerControlsRequestBus::Broadcast(
        &PlayerControlsRequestBus::Events::StrafeRight,
        press ? ActionState::Started : ActionState::Stopped);

    m_isStrafingRightPressed = press;
}

bool InputCaptureComponent::OnMouseEvent(
    const InputChannel& inputChannel)
{
    const InputChannelId id = inputChannel.GetInputChannelId();
    if (id == InputDeviceMouse::Button::Left ||
        id == InputDeviceMouse::Button::Right)
    {
        // Mouse clicks can be handled from here
        return false;
    }

    if (id == InputDeviceMouse::SystemCursorPosition)
    {
        // Get the positional mouse data
        const InputChannel::PositionData2D* pos = inputChannel.
            GetCustomData<InputChannel::PositionData2D>();
        if (!pos) return false;

        // Get its normalized position in range of [0,1]
        const AZ::Vector2& position = pos->m_normalizedPosition;

        static const Vector2 center = Vector2{.5f, .5f};
        // Keep track of how much the mouse moved
        m_mouseChangeAggregate += center - position;

        // Reset the mouse back to the center of the screen
        InputSystemCursorRequestBus::Broadcast(
            &InputSystemCursorRequestBus::Events::
                SetSystemCursorPositionNormalized, center);

        // report horizontal mouse movement
        PlayerControlsRequestBus::Broadcast(
            &PlayerControlsRequestBus::Events::Turn,
            m_mouseChangeAggregate.GetX());

        // report vertical mouse movement
        PlayerControlsRequestBus::Broadcast(
            &PlayerControlsRequestBus::Events::LookUpOrDown,
            m_mouseChangeAggregate.GetY());

        return true;
    }

    return false;
}