#include <cstdio>
#include <cstdlib>
#include <main/Command.h>

using namespace std;
using namespace Rhoban;

COMMAND_DEFINE(example, "Example command")
{
    cout << "Hello there!" << endl;
}
