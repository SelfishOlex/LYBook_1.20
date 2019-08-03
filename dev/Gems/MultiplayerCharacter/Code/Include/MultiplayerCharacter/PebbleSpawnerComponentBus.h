#pragma once
#include <AzCore/EBus/EBus.h>
#include <AzCore/Math/Transform.h>

namespace MultiplayerCharacter
{
    class PebbleSpawnerComponentRequests : public AZ::EBusTraits
    {
    public:
        virtual ~PebbleSpawnerComponentRequests() = default;

        virtual void SpawnPebbleAt(const AZ::Transform& tm) {}
    };

    using PebbleSpawnerComponentBus =
        AZ::EBus<PebbleSpawnerComponentRequests>;
} // namespace MultiplayerCharacter  