#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <MyProject/MyHelperBus.h>

namespace MyProject
{
    class MyScriptSystemComponent
        : public AZ::Component
        , public MyHelperBus::Handler
    {
    public:
        AZ_COMPONENT(MyScriptSystemComponent,
            "{96DF8D55-DA21-4A39-9624-9F9389C5C62B}");

        ~MyScriptSystemComponent() override = default;
        static void Reflect(AZ::ReflectContext* rc);
        void Init() override;
        void Activate() override { BusConnect(); }
        void Deactivate() override { BusDisconnect(); }

        // MyHelperBus
        bool IsEditor() override;
    };
}