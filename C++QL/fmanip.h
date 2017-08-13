#pragma once
#include <string>
using namespace std;
void writeStringToFile(string fn, string text);

void decode_file(string fn, unsigned const char * key);

string askForInput();