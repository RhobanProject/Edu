#include <cstdlib>
#include <main/Command.h>
#include <sockets/TCPClient.h>
#include <joystick/joystick.h>

double direction;
double power;

using namespace Rhoban;

COMMAND_DEFINE(joystick, "Joystick send")
{
    int n;
    struct js_event event;
    TCPClient client;

    open_joystick((const char*)"/dev/input/js0");
    
    client.connectTo("192.168.16.50", 7878);
    //client.connectTo("localhost", 7878);

    while (1)
    {
        n = read_joystick_event(&event);

        if (n == 1) {
            if (event.number == 2) {
                power = -event.value/((double)0xffff/2);
            }
            if (event.number == 3) {
                direction = event.value/((double)0xffff/2);
            }

            client.transmitFormat("%g, %g", direction, power);
        }
    }
}
