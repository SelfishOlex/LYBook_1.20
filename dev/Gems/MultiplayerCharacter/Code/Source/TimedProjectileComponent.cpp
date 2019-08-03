#include "TimedProjectileComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzFramework/Entity/GameEntityContextBus.h>
#include <AzFramework/Network/NetBindingHandlerBus.h>
#include <AzCore/Component/TransformBus.h>

using namespace AZ;
using namespace AzFramework;
using namespace MultiplayerCharacter;

void TimedProjectileComponent::Reflect(AZ::ReflectContext* context)
{
    if (auto sc = azrtti_cast<SerializeContext*>(context))
    {
        sc->Class<TimedProjectileComponent, Component>()
            ->Field("Initial Velocity",
                &TimedProjectileComponent::m_velocity)
            ->Field("Max Life",
                &TimedProjectileComponent::m_maxLifetime)
            ->Version(1);
        if (EditContext* ec = sc->GetEditContext())
        {
            ec->Class<TimedProjectileComponent>(
                "Timed Projectile",
                "Moves the object then deletes it")
                ->ClassElement(Edit::ClassElements::EditorData,
                    "")
                ->Attribute(Edit::Attributes::Category,
                    "Multiplayer Character")
                ->Attribute(Edit::Attributes::
                    AppearsInAddComponentMenu,
                    AZ_CRC("Game"))
                ->DataElement(nullptr,
                    &TimedProjectileComponent::m_velocity,
                    "Initial Velocity", "")
                ->DataElement(nullptr,
                    &TimedProjectileComponent::m_maxLifetime,
                    "Max Life", "");
        }
    }
}
void TimedProjectileComponent::Activate()
{
    if (NetQuery::IsEntityAuthoritative(GetEntityId()))
    {
        TickBus::Handler::BusConnect();

        bool isPhysicsEnabled = false;
        using PhysBus = Physics::RigidBodyRequestBus;
        PhysBus::EventResult( isPhysicsEnabled, GetEntityId(),
            &PhysBus::Events::IsPhysicsEnabled);

        if (isPhysicsEnabled)
        {
            SetInitialVelocity();
        }
        else
        {
            Physics::RigidBodyNotificationBus::Handler::BusConnect( GetEntityId() );
        }
    }
}

void TimedProjectileComponent::Deactivate()
{
    TickBus::Handler::BusDisconnect();
}

void TimedProjectileComponent::OnTick(float dt, ScriptTimePoint)
{
    m_lifetime += dt;
    if (m_lifetime > m_maxLifetime) // life expired
    {
        TickBus::Handler::BusDisconnect();
        GameEntityContextRequestBus::Broadcast(
            &GameEntityContextRequestBus::Events::
                DestroyGameEntity, GetEntityId());
    }
}

void TimedProjectileComponent::OnPhysicsEnabled()
{
    Physics::RigidBodyNotificationBus::Handler::BusDisconnect();
    SetInitialVelocity();
}

void TimedProjectileComponent::SetInitialVelocity()
{
    using PhysBus = Physics::RigidBodyRequestBus;
    // gravity is disabled in the slice

    AZ::Quaternion myRotation;
    TransformBus::EventResult(myRotation, GetEntityId(),
        &TransformBus::Events::GetWorldRotationQuaternion);
    // velocity in the direction of the pebble's orientation
    Vector3 dir = myRotation * AZ::Vector3::CreateAxisY(m_velocity);

    PhysBus::Event( GetEntityId(),
        &PhysBus::Events::SetLinearVelocity, dir);
}