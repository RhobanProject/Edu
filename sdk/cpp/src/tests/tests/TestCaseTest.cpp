#include <cstdlib>
#include <iostream>
#include <main/Command.h>
#include <tests/TestCase.h>

using namespace std;

class TestCaseTest : public TestCase
{
    public:
        void testTest()
        {
            char buffer[10] = {0x32, 0x66, 0x83, 0x85, 0x12};

            assertTrue(true);
            assertFalse(false);
            assertEquals(1, 1);
            assertEquals('x', 'x');
            assertEquals("Hello", "Hello");
            assertEqualsSize(buffer, buffer, 5);
        }

    protected:
        void _run()
        {
            testTest();
        }
};

COMMAND_DEFINE(tests_tests, "Tests the testing methods")
{
    TestCaseTest test;

    test.run();
}
