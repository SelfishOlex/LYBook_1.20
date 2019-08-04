#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include "MultiplayerCharacterSystemComponent.h"
#include <INetwork.h>

using namespace MultiplayerCharacter;

void MultiplayerCharacterSystemComponent::Reflect(
    AZ::ReflectContext* context)
{
    auto* s = azrtti_cast<AZ::SerializeContext*>(context);
    if (!s) return;
    s->Class<MultiplayerCharacterSystemComponent,
        AZ::Component>()->Version(1);

    AZ::EditContext* ec = s->GetEditContext();
    if (!ec) return;
    using namespace AZ::Edit::ClassElements;
    using namespace AZ::Edit::Attributes;
    ec->Class<MultiplayerCharacterSystemComponent>(
        "MultiplayerCharacter", "[description]")
        ->ClassElement(EditorData, "")
        ->Attribute(AppearsInAddComponentMenu, AZ_CRC("System"));
}

GridMate::MemberIDCompact
    MultiplayerCharacterSystemComponent::GetLocal()
{
    return m_selfId;
}

void MultiplayerCharacterSystemComponent::Activate()
{
    MultiplayerCharacterRequestBus::Handler::BusConnect();
    CrySystemEventBus::Handler::BusConnect();
}

void MultiplayerCharacterSystemComponent::Deactivate()
{
    MultiplayerCharacterRequestBus::Handler::BusDisconnect();
    if (GridMate::SessionEventBus::Handler::BusIsConnected())
    {
        GridMate::SessionEventBus::Handler::BusDisconnect();
    }
}

void MultiplayerCharacterSystemComponent::OnCrySystemInitialized(
    ISystem& system, const SSystemInitParams& param)
{
    GridMate::SessionEventBus::Handler::BusConnect(
            system.GetINetwork()->GetGridMate());

    CrySystemEventBus::Handler::BusDisconnect();
}

void MultiplayerCharacterSystemComponent::OnSessionJoined(
    GridMate::GridSession* session)
{
    m_selfId = session->GetMyMember()->GetIdCompact();
}