
#pragma once

#include <AzCore/EBus/EBus.h>
#include <ISystem.h>
#include <IConsole.h>

namespace MultiplayerCloseAllNetworkPeers
{
    class CloseNetworkPeersRequests
        : public AZ::EBusTraits
    {
    public:
        virtual ~CloseNetworkPeersRequests() = default;

        // EBusTraits overrides
        static const AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;

        virtual void CloseAll() = 0;
    };
    using CloseNetworkPeersRequestBus = AZ::EBus<CloseNetworkPeersRequests>;
} // namespace MultiplayerCloseAllNetworkPeers
