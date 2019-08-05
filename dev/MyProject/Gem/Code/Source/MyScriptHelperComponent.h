#pragma once
#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace MyProject
{
    class MyScriptHelperComponent
        : public AZ::Component
    {
    public:
        AZ_COMPONENT(MyScriptHelperComponent,
            "{10D750B4-7AD5-4FDF-8C93-056991346932}");
        ~MyScriptHelperComponent() override = default;

        static void Reflect(AZ::ReflectContext* rc);

        void Activate() override {}
        void Deactivate() override {}
    };

    class MyHelper
    {
    public:
        AZ_RTTI(MyHelper,
            "{F9168A15-69F4-4730-B222-BBC8342E5430}");
        virtual ~MyHelper() = default;

        static bool IsEntityAuthoritative(AZ::EntityId entity);
        static bool IsDedicatedServer();
    };
}