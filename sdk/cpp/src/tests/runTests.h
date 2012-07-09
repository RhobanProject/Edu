#ifndef _RUNTESTS_H
#define _RUNTESTS_H

#include <iostream>

using namespace std;

/**
 * Helper pour lancer des tests 
 */
template<class T>
void runTests(string message)
{
    T tests;
    cout << "===== " << message << endl;
    tests.run();
}

#endif // _RUNTESTS_H
