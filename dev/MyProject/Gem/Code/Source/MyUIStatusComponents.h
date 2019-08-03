#pragma once
#include <AzCore/Component/Component.h>
#include <MultiplayerCharacter/PlayerControlsRequestBus.h>

namespace MyProject
{
    // An example of UI interaction
    class MyUIForwardComponent
        : public AZ::Component
        , public MultiplayerCharacter::
                PlayerControlsRequestBus::Handler
    {
    public:
        AZ_COMPONENT(MyUIForwardComponent,
            "{2FBF833D-E29A-4FAF-AE4B-0732ED04460F}");

        // AZ::Component overrides
        void Activate() override { BusConnect(GetEntityId()); }
        void Deactivate() override { BusDisconnect(); }

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* rc);

    protected:
        // PlayerControlsRequestBus overrides
        void MoveForward(
            MultiplayerCharacter::ActionState state) override;
    };

    // An example of UI interaction
    class MyUIBackwardComponent
        : public AZ::Component
        , public MultiplayerCharacter::
                PlayerControlsRequestBus::Handler
    {
    public:
        AZ_COMPONENT(MyUIBackwardComponent,
            "{E3B07140-DD69-47B0-8C50-2E33C15B499F}");

        // AZ::Component overrides
        void Activate() override { BusConnect(GetEntityId()); }
        void Deactivate() override { BusDisconnect(); }

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* rc);

    protected:
        // PlayerControlsRequestBus overrides
        void MoveBackward(
            MultiplayerCharacter::ActionState state) override;
    };

    // An example of UI interaction
    class MyUIStrafeLeftComponent
        : public AZ::Component
        , public MultiplayerCharacter::
                PlayerControlsRequestBus::Handler
    {
    public:
        AZ_COMPONENT(MyUIStrafeLeftComponent,
            "{18F69738-84AD-4ACC-8781-62BA29E7F2B4}");

        // AZ::Component overrides
        void Activate() override { BusConnect(GetEntityId()); }
        void Deactivate() override { BusDisconnect(); }

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* rc);

    protected:
        // PlayerControlsRequestBus overrides
        void StrafeLeft(
            MultiplayerCharacter::ActionState state) override;
    };

    // An example of UI interaction
    class MyUIStrafeRightComponent
        : public AZ::Component
        , public MultiplayerCharacter::
                PlayerControlsRequestBus::Handler
    {
    public:
        AZ_COMPONENT(MyUIStrafeRightComponent,
            "{ABEABC3B-628E-47FC-A2D9-BE984164B1CE}");

        // AZ::Component overrides
        void Activate() override { BusConnect(GetEntityId()); }
        void Deactivate() override { BusDisconnect(); }

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* rc);

    protected:
        // PlayerControlsRequestBus overrides
        void StrafeRight(
            MultiplayerCharacter::ActionState state) override;
    };
}