#ifndef _COMMANDSPECIFICATION_H
#define _COMMANDSPECIFICATION_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

namespace Rhoban
{
  class CommandSpecification
  {
  public:
    CommandSpecification(string name, string description, string destination, int index, vector<string> parametersPattern, vector<string> answerPattern);

  protected:
    string name;
    string description;
    string destination;
    int index;
    vector<string> parametersPattern;
    vector<string> answerPattern;
  };
}

#endif // _COMMANDSPECIFICATION_H
