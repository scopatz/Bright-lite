#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "origenBuilder.h"

vector<double> ParseOriginFile(String file_location);
vector<double> FindNeutronProduction(String line);

#endif // PARSER_H_INCLUDED
