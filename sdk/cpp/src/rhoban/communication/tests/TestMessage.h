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

        /**
         * Test d'écriture le contenu d'un message
         */
        void testWriteContents()
        {
            Message msg;
            char contents[] = {
                0x00, 0x00, 0x00, 0x7b,
                0x00, 0x00, 0x00, 0x05,
                'H', 'e', 'l', 'l', 'o'
            };
            char *raw;

            msg.clear();
            msg.append((ui32)123);
            msg.append("Hello");
            raw = msg.getRaw();

            assertEquals(msg.length, 13);
            assertEqualsSize(raw+MSG_HEADER_SIZE, contents, sizeof(contents));
        }

        /**
         * Test de lecture le contenu d'un message
         */
        void testReadContents()
        {
            char header[] = {
                0x00, 0x00, 0x00, 0x7b, // 123
                0x00, 0x00, 0x00, 0x03, // 3
                0x00, 0x00, 0x00, 0x02, // 2
                0x00, 0x00, 0x00, 0x09, // 9
            };

            char contents[] = {
                0x00, 0x00, 0x00, 0x01, // 1
                0x00, 0x00, 0x00, 0x05, // 5
                'H', 'e', 'l', 'l', 'o'
            };

            Message msg;
            msg.clear();
            msg.read_header(header);
            msg.alloc(sizeof(contents) + MSG_HEADER_SIZE);
            msg.size = sizeof(contents) + MSG_HEADER_SIZE;
            memcpy(msg.buffer + MSG_HEADER_SIZE, contents, sizeof(contents));
            
            assertEquals(msg.uid, 123);
            assertEquals(msg.destination, 3);
            assertEquals(msg.command, 2);
            assertEquals(msg.length, 9);
            assertEquals(msg.read_int(), 1);
            assertEquals(msg.read_string(), "Hello");
        }

    protected:
        void _run()
        {
            testInit();
            testHeader();
            testWriteContents();
            testReadContents();
        }
};
