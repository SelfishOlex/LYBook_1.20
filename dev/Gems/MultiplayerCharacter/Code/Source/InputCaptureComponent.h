#pragma once
#include <AzCore/Component/Component.h>
#include <AzFramework/Input/Events/InputChannelEventListener.h>

namespace MultiplayerCharacter
{
    // Captures input the C++ way
    class InputCaptureComponent
        : public AZ::Component
        , public AzFramework::InputChannelEventListener
    {
    public:
        AZ_COMPONENT(InputCaptureComponent,
            "{AAB4DD63-3312-4C79-B4AD-108A63D0F11B}");

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* ref);

    protected:
        // AzFramework::InputChannelEventListener implementation
        bool OnInputChannelEventFiltered(const
            AzFramework::InputChannel& inputChannel) override;

        bool OnKeyboardEvent(
            const AzFramework::InputChannel& inputChannel);
        bool OnMouseEvent(
            const AzFramework::InputChannel& inputChannel);

    private:
        void CheckAndUpdateForward(bool pressed);
        void CheckAndUpdateBackward(bool pressed);
        void CheckAndUpdateStrafeLeft(bool press);
        void CheckAndUpdateStrafeRight(bool press);
        void CheckAndUpdateShoot(bool press);

        bool m_isForwardPressed = false;
        bool m_isBackwardPressed = false;
        bool m_isStrafingLeftPressed = false;
        bool m_isStrafingRightPressed = false;
        bool m_isShooting = false;

        AZ::Vector2 m_mouseChangeAggregate{0, 0};
    };
}