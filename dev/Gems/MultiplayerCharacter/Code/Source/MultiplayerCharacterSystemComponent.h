#pragma once
#include <AzCore/Component/Component.h>
#include <MultiplayerCharacter/MultiplayerCharacterBus.h>
#include <GridMate/Session/Session.h>
#include <CrySystemBus.h>

namespace MultiplayerCharacter
{
    class MultiplayerCharacterSystemComponent
        : public AZ::Component
        , protected MultiplayerCharacterRequestBus::Handler
        , public GridMate::SessionEventBus::Handler
        , public CrySystemEventBus::Handler
    {
    public:
        AZ_COMPONENT(MultiplayerCharacterSystemComponent,
            "{D1D1F21F-2302-4437-92EA-624BAF1A2ED4}");

        static void Reflect(AZ::ReflectContext* context);

    protected:
        // MultiplayerCharacterRequestBus
        GridMate::MemberIDCompact GetLocal() override;

        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        // CrySystemEventBus
        void OnCrySystemInitialized(ISystem& system,
            const SSystemInitParams& param) override;

        // GridMate::SessionEventBus implementation
        void OnSessionJoined(
            GridMate::GridSession* session) override;

    private:
        GridMate::MemberIDCompact m_selfId = 0;
    };
}