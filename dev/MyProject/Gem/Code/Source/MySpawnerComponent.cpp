#include "MySpawnerComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <LmbrCentral/Scripting/SpawnerComponentBus.h>
#include <AzFramework/Network/NetBindingHandlerBus.h>

using namespace MyProject;

void MySpawnerComponent::Activate()
{
    using namespace AzFramework;
    if (!NetQuery::IsEntityAuthoritative(GetEntityId())) return;

    // Spawn the selected dynamic slice
    LmbrCentral::SpawnerComponentRequestBus::Event(GetEntityId(),
        &LmbrCentral::SpawnerComponentRequestBus::Events::Spawn);
}

void MySpawnerComponent::GetRequiredServices(
    AZ::ComponentDescriptor::DependencyArrayType& req)
{
    // We require SpawnerComponent on the same entity
    req.push_back(AZ_CRC("SpawnerService"));
    // Make sure this component is activated after NetworkBinding
    req.push_back(AZ_CRC("ReplicaChunkService", 0xf86b88a8));
}

void MySpawnerComponent::Reflect(AZ::ReflectContext* reflection)
{
    auto sc = azrtti_cast<AZ::SerializeContext*>(reflection);
    if (!sc) return;

    sc->Class<MySpawnerComponent, Component>()
        ->Version(1);

    AZ::EditContext* ec = sc->GetEditContext();
    if (!ec) return;

    using namespace AZ::Edit::Attributes;
    // reflection of this component for Lumberyard Editor
    ec->Class<MySpawnerComponent>("My Spawner Component",
        "[spawns a dynamic slice using Spawner]")
      ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
        ->Attribute(AppearsInAddComponentMenu, AZ_CRC("Game"))
        ->Attribute(Category, "My Project");
}