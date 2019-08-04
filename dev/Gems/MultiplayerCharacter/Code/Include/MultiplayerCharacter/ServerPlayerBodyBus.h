#pragma once
#include <AzCore/EBus/EBus.h>
#include <GridMate/Session/Session.h>

namespace MultiplayerCharacter
{
    class ServerPlayerBodyInterface
        : public AZ::ComponentBus
    {
    public:
        virtual ~ServerPlayerBodyInterface() = default;

        virtual void SetAssociatedPlayerId(
            GridMate::MemberIDCompact player) = 0;
    };

    using ServerPlayerBodyBus =
        AZ::EBus<ServerPlayerBodyInterface>;
} // namespace MultiplayerCharacter   