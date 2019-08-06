#include "MultiplayerCloseAllNetworkPeers_precompiled.h"
#include <platform_impl.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <IGem.h>
#include "GemSystemComponent.h"
#include "WorkerComponent.h"
#include "ConsoleCommands.h"

namespace MultiplayerCloseAllNetworkPeers
{
    class MultiplayerCloseAllNetworkPeersModule
        : public CryHooksModule
    {
    public:
        AZ_RTTI(MultiplayerCloseAllNetworkPeersModule, "{983AAF88-DFA3-4F14-BD1F-76F1D7F4CA69}", CryHooksModule);
        AZ_CLASS_ALLOCATOR(MultiplayerCloseAllNetworkPeersModule, AZ::SystemAllocator, 0);

        MultiplayerCloseAllNetworkPeersModule()
            : CryHooksModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                GemSystemComponent::CreateDescriptor(),
                WorkerComponent::CreateDescriptor()
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<GemSystemComponent>(),
            };
        }

        void OnCrySystemInitialized(ISystem& system,
            const SSystemInitParams& systemInitParams) override
        {
            CryHooksModule::OnCrySystemInitialized(system, systemInitParams);
            ConsoleCommands::Register();
        }

        void OnSystemEvent(ESystemEvent event,
            UINT_PTR, UINT_PTR) override
        {
            switch (event)
            {
            case ESYSTEM_EVENT_FULL_SHUTDOWN:
            case ESYSTEM_EVENT_FAST_SHUTDOWN:
                ConsoleCommands::Unregister();
            default:
                AZ_UNUSED(event);
            }
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(MultiplayerCloseAllNetworkPeers_68ef06879ec4459891ac9ad946d476e9, MultiplayerCloseAllNetworkPeers::MultiplayerCloseAllNetworkPeersModule)
