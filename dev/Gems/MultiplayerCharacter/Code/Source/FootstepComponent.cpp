#include "FootstepComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <LmbrCentral/Audio/AudioTriggerComponentBus.h>
#include <platform_impl.h>

using namespace AZ;
using namespace MultiplayerCharacter;
using namespace LmbrCentral;

void FootstepComponent::Activate()
{
    FootstepComponentBus::Handler::BusConnect(GetEntityId());
}

void FootstepComponent::Deactivate()
{
    FootstepComponentBus::Handler::BusDisconnect();
}

void FootstepComponent::Reflect(AZ::ReflectContext* ref)
{
    auto sc = azrtti_cast<AZ::SerializeContext*>(ref);
    if (!sc) return;

    using Self = FootstepComponent; // to shorten lines
    sc->Class<FootstepComponent, Component>()
        ->Field("Footstep period", &Self::m_footstepPeriod)
        ->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    using namespace AZ::Edit::UIHandlers;
    // reflection of this component for Lumberyard Editor
    ec->Class<FootstepComponent>("Footstep Component",
        "[Triggers footstep audio based on time]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu,
            AZ_CRC("Game", 0x232b318c))
        ->Attribute(Category, "Multiplayer Character")
        ->DataElement(Default, &Self::m_footstepPeriod,
            "Footstep period", "how often to play audio effect");
}

void FootstepComponent::TickFootstep(float dtSeconds)
{
    m_currentFootstepTime += dtSeconds;

    if (m_currentFootstepTime >= m_footstepPeriod)
    {
        m_currentFootstepTime = 0;
        using Audio = AudioTriggerComponentRequestBus;
        Audio::Event(GetEntityId(), &Audio::Events::Play);
    }
}  