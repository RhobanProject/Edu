#include <cstdio>
#include <main/Command.h>
#include <tests/TestCase.h>
#include <rhoban/communication/Message.h>

using namespace Rhoban;

/**
 * Test sur la création de messages
 */
class TestMessage : public TestCase
{
    public:
        /**
         * Teste l'initialisation du message
         */
        void testInit()
        {
            Message msg;
            msg.clear();

            assertEquals((int)msg.size, MSG_HEADER_SIZE);
        }

        /**
         * Tests la création d'en-têtes
         */
        void testHeader()
        {
            Message msg;
            char header[] = {0x00, 0x00, 0x00, 0x7b,
                0x00, 0x00, 0x01, 0xc8,
                0x00, 0x00, 0x03, 0x15,
                0x00, 0x00, 0x03, 0xf3};

            msg.clear();
            msg.uid = 123;
            msg.destination = 456;
            msg.command = 789;
            msg.size = 1011 + MSG_HEADER_SIZE;

            assertEqualsSize(msg.getRaw(), header, 13);//sizeof(header));
        }

    protected:
        void _run()
        {
            testInit();
            testHeader();
        }
};
