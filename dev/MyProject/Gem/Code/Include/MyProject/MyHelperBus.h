#pragma once
#include <AzCore/EBus/EBus.h>

namespace MyProject
{
    class MyHelperRequests : public AZ::EBusTraits
    {
    public:
        // EBusTraits overrides
        static const AZ::EBusHandlerPolicy HandlerPolicy =
            AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy =
            AZ::EBusAddressPolicy::Single;

        // Put your public methods here
        virtual bool IsEditor() = 0;
    };
    using MyHelperBus = AZ::EBus<MyHelperRequests>;
} // namespace MyProject
