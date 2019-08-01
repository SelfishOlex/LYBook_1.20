
#include <AzTest/AzTest.h>

class MyProjectTest
    : public ::testing::Test
{
protected:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(MyProjectTest, ExampleTest)
{
    ASSERT_TRUE(true);
}

AZ_UNIT_TEST_HOOK();
