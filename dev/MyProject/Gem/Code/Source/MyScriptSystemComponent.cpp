#include "MyScriptSystemComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <ISystem.h>
#include <ScriptCanvas/MyNodeLibrary.h>

using namespace MyProject;

void MyScriptSystemComponent::Reflect(AZ::ReflectContext* rc)
{
    if (auto* sc = azrtti_cast<AZ::SerializeContext*>(rc))
    {
        MyNodeLibrary::Reflect(rc);

        sc->Class<MyScriptSystemComponent, Component>()
            ->Version(1);
        if (AZ::EditContext* ec = sc->GetEditContext())
        {
            using namespace AZ::Edit::Attributes;
            using namespace AZ::Edit::UIHandlers;
            ec->Class<MyScriptSystemComponent>(
                "My Script System Component",
                "[various helpful tool for scripts]")
                ->ClassElement(
                    AZ::Edit::ClassElements::EditorData, "")
                // mark it as a system component
                ->Attribute(AppearsInAddComponentMenu,
                    AZ_CRC("System", 0xc94d118b));
        }
    }
    if(auto* bc = azrtti_cast<AZ::BehaviorContext*>(rc))
    {
        bc->EBus<MyHelperBus>("MyHelperBus")->
            Event("IsEditor", &MyHelperBus::Events::IsEditor);
    }
}

void MyScriptSystemComponent::Init()
{
    using namespace ScriptCanvas;
    if (auto nodeRegistryVariable = GetNodeRegistry())
    {
        NodeRegistry& nodeRegistry = nodeRegistryVariable.Get();
        MyNodeLibrary::InitNodeRegistry(nodeRegistry);
    }
}

bool MyScriptSystemComponent::IsEditor()
{
    ISystem* system = nullptr;
    CrySystemRequestBus::BroadcastResult(system,
        &CrySystemRequestBus::Events::GetCrySystem);
    if (system && system->GetGlobalEnvironment())
    {
        return system->GetGlobalEnvironment()->IsEditor();
    }
    return false;
}