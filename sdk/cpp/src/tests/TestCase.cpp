#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string.h>
#include "TestCase.h"

using namespace std;

TestCase::TestCase() : assertions(0), assertionsPassed(0)
{
}

void TestCase::run()
{
    assertions = 0;
    assertionsPassed = 0;

    _run();

    cout << endl;
    cout << "-------------------------------" << endl;
    cout << "Ended, " << assertionsPassed << "/" << assertions << " assertions passed" << endl << endl;
}

void TestCase::setPlace(string file, int line, string function)
{
    ostringstream oss;
    oss << file << ":" << line << " in " << function;
    place = oss.str();
}

void TestCase::error(string error)
{
    assertions++;
    cout << "!FAIL " << place << " " << current << ": " << error << endl;
}

void TestCase::pass()
{
    assertions++;
    assertionsPassed++;
    cout << "." << flush;
}

void TestCase::_assertTrue(bool expression)
{
    if (!expression) {
        error("Expression is false");
    } else {
        pass();
    }
}

void TestCase::_assertFalse(bool expression)
{
    if (expression) {
        error("Expression is true");
    } else {
        pass();
    }
}

void TestCase::_assertEquals(string str1, string str2)
{
    if (str1 != str2) {
        error("String differs");
    } else {
        pass();
    }
}

void TestCase::_assertEquals(char *char1, char *char2, int size)
{
    if (char1 == NULL || char2 == NULL) {
        error("Null pointer");
        return;
    }

    if (memcmp(char1, char2, size) != 0) {
        error("Char* differs");
    } else {
        pass();
    }
}

void TestCase::_assertEquals(int int1, int int2)
{
    if (int1 != int2) {
        error("Int differs");
    } else {
        pass();
    }
}

void TestCase::_assertEquals(char char1, char char2)
{
    if (char1 != char2) {
        error("Char differs");
    } else {
        pass();
    }
}

void TestCase::_assertEqualsDelta(float f1, float f2, float delta)
{
    if (abs(f1-f2) > delta) {
        error("Number differs");
    } else {
        pass();
    }
}
