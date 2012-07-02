#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Rhoban
{
    void sleep_ms(int ms)
    {
#ifdef WIN32
        Sleep(ms);
#else
        usleep(1000 * ms);
#endif
    }
}
