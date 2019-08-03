#pragma once
#include <AzCore/Component/Component.h>
#include <MultiplayerCharacter/PlayerControlsRequestBus.h>
#include <AzCore/Component/TickBus.h>

namespace MultiplayerCharacter
{
    // Implements camera pitch controls
    class CameraControlsComponent
        : public AZ::Component
        , public PlayerControlsRequestBus::Handler
    {
    public:
        AZ_COMPONENT(CameraControlsComponent,
            "{6E612DB6-D875-4D15-ABB6-76B852BD67BB}");

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* ref);

    protected:
        // PlayerControlsRequestBus interface
        void LookUpOrDown(float amount) override;

    private:
        float m_lookUpSpeed = 0.6f;

        float m_rotY = 0.f;
        void SetRotation();
    };
}