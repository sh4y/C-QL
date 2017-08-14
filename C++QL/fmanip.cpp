#include "stdafx.h"
#include "fmanip.h"
#include <iostream>
#include "Cryptography.h"
#include <fstream>

void writeStringToFile(string fn, string text)
{
	ofstream openFile;
	openFile.open("Stocks/" + fn, fstream::app);
	if (openFile.is_open())
	{
		openFile << text << endl;
		openFile.close();
	}

	else cout << "Couldn't open the file" << endl;
}

void decode_file(string fn, unsigned const char * key)
{
	ifstream myfile("Stocks/" + fn);
	string line;

	if (myfile.is_open())
	{
		while (getline(myfile, line) || line != "")
		{
			string txt = decode_rc4(line, key);
			cout << txt << endl;
			line = "";
		}
	}
	myfile.close();
}

string askForInput()
{
	cout << "->Command: ";
	char text[1000];
	cin.getline(text, sizeof text);

	string line = (string)text;
	return line;
}