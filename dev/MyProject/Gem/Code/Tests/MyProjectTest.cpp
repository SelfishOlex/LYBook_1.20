#include <AzTest/AzTest.h>
#include <Tests/TestTypes.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TransformBus.h>
#include <OscillatorComponent.h>
#include <AzFramework/Components/TransformComponent.h>

using namespace AZ;
using namespace AzFramework;
using namespace AZStd;
using namespace MyProject;
using namespace ::testing;

class OscillatorTest
    : public ::UnitTest::AllocatorsFixture
{
    AZStd::unique_ptr<AZ::SerializeContext> m_sc;
    AZStd::unique_ptr<AZ::ComponentDescriptor> m_td;
    AZStd::unique_ptr<AZ::ComponentDescriptor> m_od;

protected:
    void SetUp() override
    {
        ::UnitTest::AllocatorsFixture::SetUp();

        // register components involved in testing
        m_sc = AZStd::make_unique<AZ::SerializeContext>();
        m_td.reset(TransformComponent::CreateDescriptor());
        m_td->Reflect(m_sc.get());
        m_od.reset(OscillatorComponent::CreateDescriptor());
        m_od->Reflect(m_sc.get());
    }

    void TearDown() override
    {
        m_td.reset();
        m_od.reset();
        m_sc.reset();

        ::UnitTest::AllocatorsFixture::TearDown();
    }

    // helper method
    void PopulateEntity(Entity& e)
    {
        // OscillatorComponent is the component we are testing
        e.CreateComponent<OscillatorComponent>();
        // And how it interacts with
        e.CreateComponent<AzFramework::TransformComponent>();

        // Bring the entity online
        e.Init();
        e.Activate();
    }
};

TEST_F(OscillatorTest, EntityMovingUp)
{
    Entity e;
    PopulateEntity(e);

    // Move entity to (0,0,0)
    TransformBus::Event(e.GetId(),
        &TransformBus::Events::SetWorldTranslation,
        Vector3::CreateZero());

    // tick once
    TickBus::Broadcast(&TickBus::Events::OnTick, 0.1f,
            ScriptTimePoint());

    // Get entity's position
    Vector3 change;
    TransformBus::EventResult(change, e.GetId(),
        &TransformBus::Events::GetWorldTranslation);

    // check that it moved up, by any amount
    ASSERT_TRUE(change.GetZ() > 0);
}

// Include this only ONCE per module
AZ_UNIT_TEST_HOOK();
