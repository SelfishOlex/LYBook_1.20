#pragma once
#include <AzCore/Component/Component.h>
#include <MultiplayerCharacter/FootstepComponentBus.h>

namespace MultiplayerCharacter
{
    // Triggers footstep audio based on time
    class FootstepComponent
        : public AZ::Component
        , public FootstepComponentBus::Handler
    {
    public:
        AZ_COMPONENT(FootstepComponent,
            "{8FACA314-FD9F-4304-BAE2-4D3F68F44353}");

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* ref);

    protected:
        // FootstepComponentBus
        void TickFootstep(float dtSeconds) override;

    private:
        float m_footstepPeriod = 0.25; // seconds
        float m_currentFootstepTime = 0; // seconds
    };
}  