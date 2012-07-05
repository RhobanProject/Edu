#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include "CommandSpecification.h"

using namespace std;

namespace Rhoban
{
  CommandSpecification::CommandSpecification(string name, string description, string destination, int index, vector<string> parametersPattern, vector<string> answerPattern)
  {
    this->name=name;
    this->description=description;
    this->destination=destination;
    this->index=index;
    this->parametersPattern=parametersPattern;
    this->answerPattern=answerPattern;
  }
}
