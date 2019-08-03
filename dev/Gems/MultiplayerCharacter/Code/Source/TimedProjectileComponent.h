#pragma once
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzFramework/Physics/RigidBodyBus.h>

namespace MultiplayerCharacter
{
    class TimedProjectileComponent
        : public AZ::Component
        , public AZ::TickBus::Handler
        , public Physics::RigidBodyNotificationBus::Handler
    {
    public:
        AZ_COMPONENT(TimedProjectileComponent,
            "{30E90E50-DD3A-4F4C-AC4C-35553740EA40}");
        static void Reflect(AZ::ReflectContext* context);

        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        static void GetRequiredServices(
            AZ::ComponentDescriptor::DependencyArrayType& req)
        {
            req.push_back(AZ_CRC("PhysXRigidBodyService"));
        }

    protected:
        // AZ::TickBus interface implementation
        void OnTick(float dt, AZ::ScriptTimePoint) override;

        // RigidBodyNotificationBus
        void OnPhysicsEnabled() override;
        void OnPhysicsDisabled() override {}

    private:
        float m_lifetime = 0.f;
        float m_maxLifetime = 3.f;
        float m_velocity = 10.f; // over one second

        void SetInitialVelocity();
    };
}