#pragma once
#include <AzCore/Component/Component.h>
#include <MultiplayerCharacter/LocalClientBus.h>

namespace MultiplayerCharacter
{
    class LocalClientComponent
        : public AZ::Component
        , public LocalClientBus::Handler
    {
    public:
        AZ_COMPONENT(LocalClientComponent,
            "{A088C8A8-8568-405A-A409-1576F0D2BCCF}");

        static void Reflect(AZ::ReflectContext* context);

    protected:
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        // LocalClientBus implementation
        void AttachToBody(
            GridMate::MemberIDCompact playerId,
            const AZ::EntityId& body) override;

    private:
        AZ::Vector3 m_cameraOffset = AZ::Vector3::CreateZero();
    };
}