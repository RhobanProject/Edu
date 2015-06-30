#include "Parser.h"

void Parser::loadConfigurationFile(char*path)
{
  ifstream conf_file;
  conf_file.open (path);
  if ( conf_file.is_open() )
    {

      
      conf_file.close();
    }


}
