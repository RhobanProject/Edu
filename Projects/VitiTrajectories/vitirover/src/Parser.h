#ifndef PARSER_H
#define PARSER_H

/**
  * Class de Parsing des fichiers de configuration.
  **/

#include <vector>
#include <iostream>
#include <fstream>

#include "Robot.h"

using namespace std;

class Parser
{
 public:
  Parser();
  Parser(char*path);
  void loadConfigurationFile(char*path);
};

#endif // PARSER_H
