#pragma once
#include <AzCore/EBus/EBus.h>
#include <AzCore/Component/EntityId.h>

namespace MultiplayerCharacter
{
    enum class ActionState
    {
        Started,
        Stopped
    };

    class PlayerControlsRequests
        : public AZ::EBusTraits
    {
    public:
        virtual ~PlayerControlsRequests() = default;

        // EBusTraits overrides
        static const AZ::EBusHandlerPolicy HandlerPolicy =
            AZ::EBusHandlerPolicy::Single;
        static const AZ::EBusAddressPolicy AddressPolicy =
            AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::EntityId;

        // Put your public methods here
        virtual void MoveForward(ActionState state) {}
        virtual void MoveBackward(ActionState state) {}
        virtual void StrafeLeft(ActionState state) {}
        virtual void StrafeRight(ActionState state) {}
        virtual void Turn(float amount) {}
        virtual void LookUpOrDown(float amount) {}
        virtual void Shoot(ActionState state) {}
    };

    using PlayerControlsRequestBus =
        AZ::EBus<PlayerControlsRequests>;
} // namespace MultiplayerCharacter