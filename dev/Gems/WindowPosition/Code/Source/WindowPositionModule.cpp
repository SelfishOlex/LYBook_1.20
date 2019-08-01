#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>
#include <CrySystemBus.h>
#include <WindowPositionCommands.h>
#include <platform_impl.h>

namespace WindowPosition
{
    class WindowPositionModule
        : public AZ::Module
        , public CrySystemEventBus::Handler
    {
    public:
        AZ_RTTI(WindowPositionModule,
            "{2A2A2481-B60F-46E4-9601-80191F8B3B68}", AZ::Module);
        AZ_CLASS_ALLOCATOR(WindowPositionModule,
            AZ::SystemAllocator, 0);

        WindowPositionModule()
        {
            CrySystemEventBus::Handler::BusConnect();
        }

        ~WindowPositionModule()
        {
            CrySystemEventBus::Handler::BusDisconnect();
        }

        WindowPositionCommands m_commands;

        // CrySystemEventBus
        void OnCrySystemInitialized(ISystem& system,
                    const SSystemInitParams&) override
        {
            m_commands.Register(system);
        }
        void OnCrySystemShutdown(ISystem& system) override
        {
            m_commands.Unregister(system);
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(WindowPosition_625cd93f587f4eb58b4d0c06f486e87c,
        WindowPosition::WindowPositionModule)
