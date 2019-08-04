#pragma once
#include <AzCore/Component/EntityId.h>
#include <AzCore/Component/ComponentBus.h>

namespace MultiplayerCharacter
{
    class FootstepComponentRequests
        : public AZ::ComponentBus
    {
    public:
        virtual ~FootstepComponentRequests() = default;

        // Put your public methods here
        virtual void TickFootstep(float seconds) = 0;
    };

    using FootstepComponentBus =
        AZ::EBus<FootstepComponentRequests>;
} // namespace MultiplayerCharacter  