#pragma once
#include <AzCore/EBus/EBus.h>
#include <GridMate\Session\Session.h>

namespace MultiplayerCharacter
{
    class MultiplayerCharacterRequests : public AZ::EBusTraits
    {
    public:
        virtual ~MultiplayerCharacterRequests() = default;
        // EBusTraits overrides
        static const AZ::EBusHandlerPolicy HandlerPolicy =
            AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy =
            AZ::EBusAddressPolicy::Single;

        // Returns local network id
        virtual GridMate::MemberIDCompact GetLocal() = 0;
    };
    using MultiplayerCharacterRequestBus =
        AZ::EBus<MultiplayerCharacterRequests>;
} // namespace MultiplayerCharacter