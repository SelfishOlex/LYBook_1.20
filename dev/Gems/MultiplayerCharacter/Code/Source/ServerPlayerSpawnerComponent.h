#pragma once
#include <AzCore/Component/Component.h>
#include <GridMate/Session/Session.h>
#include <AzFramework/Entity/EntityContextBus.h>
#include "AzCore/Math/Random.h"

namespace MultiplayerCharacter
{
    class ServerPlayerSpawnerComponent
        : public AZ::Component
        , public GridMate::SessionEventBus::Handler
        , AzFramework::SliceInstantiationResultBus::MultiHandler
    {
    public:
        AZ_COMPONENT(ServerPlayerSpawnerComponent,
            "{49BA7630-8B7E-48CA-9FFD-152268C2A461}");

        static void Reflect(AZ::ReflectContext* context);

    protected:
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        // GridMate::SessionEventBus interface
        void OnMemberJoined(
            GridMate::GridSession* session,
            GridMate::GridMember* member) override;

        // SliceInstantiationResultBus interface
        void OnSliceInstantiated(
            const AZ::Data::AssetId&,
            const AZ::SliceComponent::SliceInstanceAddress&)
                override;
    private:
        AZStd::unordered_map<
            AzFramework::SliceInstantiationTicket,
            GridMate::MemberIDCompact> m_joiningPlayers;
        AZ::u8 m_playerCount = 0;

        void SpawnPlayer(GridMate::MemberIDCompact playerId);
    };
}