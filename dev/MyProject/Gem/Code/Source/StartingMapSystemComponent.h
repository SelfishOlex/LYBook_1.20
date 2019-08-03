#pragma once
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>

namespace MyProject
{
    class StartingMapSystemComponent
        : public AZ::Component
        , public AZ::SystemTickBus::Handler
    {
    public:
        AZ_COMPONENT(StartingMapSystemComponent,
            "{771A7066-45AF-4E0D-8915-04129BC64E0E}");

        // provides Lumberyard reflection of this component
        static void Reflect(AZ::ReflectContext* context);

    protected:
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        // AZ::SystemTickBus interface implementation
        void OnSystemTick() override;

    private:
        // the map to start with by default on a client
        AZStd::string m_clientMap;
        // the map to start with by default on the server
        AZStd::string m_serverMap;
    };
}