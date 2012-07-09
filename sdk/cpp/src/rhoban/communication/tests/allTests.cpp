#include <main/Command.h>
#include <tests/runTests.h>
#include "TestEncoding.h"

COMMAND_DEFINE(communication_tests, "Run the Rhoban communication tests")
{
    runTests<TestEncoding>("Tests d'encodage");
}
