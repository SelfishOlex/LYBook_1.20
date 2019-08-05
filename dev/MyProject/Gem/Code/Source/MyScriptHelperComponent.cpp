#include "MyScriptHelperComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzFramework/Network/NetBindingHandlerBus.h>

using namespace MyProject;

void MyScriptHelperComponent::Reflect(AZ::ReflectContext* rc)
{
    using namespace AZ;
    if (auto sc = azrtti_cast<SerializeContext*>(rc))
    {
        sc->Class<MyScriptHelperComponent, Component>()
            ->Version(1);
    }

    auto* bc = azrtti_cast<BehaviorContext*>(rc);
    if(!bc) return;
    bc->Class<MyHelper>("MyHelper")->
        Method("IsDedicatedServer", &MyHelper::IsDedicatedServer)->
        Method("IsServer", &MyHelper::IsEntityAuthoritative)->
        Method("IsClient",
            [](AZ::EntityId entity)
            {
                return !MyHelper::IsEntityAuthoritative(entity);
            })
    ;
}

bool MyHelper::IsEntityAuthoritative(AZ::EntityId entity)
{
    return AzFramework::NetQuery::IsEntityAuthoritative(entity);
}

bool MyHelper::IsDedicatedServer()
{
#if defined(DEDICATED_SERVER)
    return true;
#else
    return false;
#endif
}
