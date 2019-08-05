#pragma once
#include <AzCore/EBus/EBus.h>

namespace MultiplayerCharacter
{
    class LocalPlayerBodyInterface
        : public AZ::ComponentBus
    {
    public:
        ~LocalPlayerBodyInterface() override = default;

        virtual void OnClientInputConnected() = 0;
    };

    using LocalPlayerBodyNotificationBus =
        AZ::EBus<LocalPlayerBodyInterface>;
} // namespace MultiplayerCharacter   