
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

#include <MyProjectSystemComponent.h>
#include <MyComponent.h>
#include <OscillatorComponent.h>
#include <MySpawnerComponent.h>
#include <StartingMapSystemComponent.h>
#include <TransformSyncComponent.h>
#include <MyUIStatusComponents.h>
#include <MyScriptHelperComponent.h>
#include <MyScriptSystemComponent.h>
#include <ScriptCanvas/MyNodeLibrary.h>

namespace MyProject
{
    class MyProjectModule
        : public AZ::Module
    {
    public:
        AZ_RTTI(MyProjectModule, "{339CB9DA-86E2-4B22-BEF1-3EBD5F3FD18F}", AZ::Module);
        AZ_CLASS_ALLOCATOR(MyProjectModule, AZ::SystemAllocator, 0);

        MyProjectModule()
            : AZ::Module()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            m_descriptors.insert(m_descriptors.end(), {
                MyProjectSystemComponent::CreateDescriptor(),
                MyComponent::CreateDescriptor(),
                OscillatorComponent::CreateDescriptor(),
                MySpawnerComponent::CreateDescriptor(),
                StartingMapSystemComponent::CreateDescriptor(),
                TransformSyncComponent::CreateDescriptor(),

                MyUIForwardComponent::CreateDescriptor(),
                MyUIBackwardComponent::CreateDescriptor(),
                MyUIStrafeLeftComponent::CreateDescriptor(),
                MyUIStrafeRightComponent::CreateDescriptor(),

                MyScriptHelperComponent::CreateDescriptor(),
                MyScriptSystemComponent::CreateDescriptor(),
            });

            const auto desc =
                MyNodeLibrary::GetComponentDescriptors();
            m_descriptors.insert(m_descriptors.end(),
                desc.begin(), desc.end());
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<MyProjectSystemComponent>(),
                azrtti_typeid<MyScriptSystemComponent>(),
                azrtti_typeid<StartingMapSystemComponent>(),
            };
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(MyProject_681a17778db146f8a57bb5fd5960333c, MyProject::MyProjectModule)
