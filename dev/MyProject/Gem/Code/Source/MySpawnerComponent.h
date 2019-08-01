#pragma once
#include <AzCore/Component/Component.h>

namespace MyProject
{
    // An example of spawning a dynamic slice
    class MySpawnerComponent : public AZ::Component
    {
    public:
        AZ_COMPONENT(MySpawnerComponent,
            "{864896FA-C6D0-48CA-8CE9-3DBF2B0FB393}");

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override {}

        // Provide runtime reflection, if any
        static void Reflect(AZ::ReflectContext* reflection);

        // what other components does this component require?
        static void GetRequiredServices(
            AZ::ComponentDescriptor::DependencyArrayType& req);
    };
}