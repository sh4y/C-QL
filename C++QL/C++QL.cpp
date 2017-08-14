// C++QL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iomanip>
#include <fstream>
#include <iostream>

#include "fmanip.h"
#include "Cryptography.h"

using namespace std;
string KEY_FILE = "Files/pw.txt";
string encodedPasskey;
const unsigned char * key;

void first_time_run()
{
	char pw[100];
	cout << "Choose a password: ";
	cin.getline(pw, sizeof pw);
	string encodedPassword = sha256(string(pw));
	writeStringToFile(KEY_FILE, encodedPassword);
}

bool login()
{
	cout << "->Please enter your password: ";
	char pw[100];
	cin.getline(pw, sizeof pw);
	string encodedPassword = sha256(string(pw));
	string line;

	ifstream myfile(KEY_FILE);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (encodedPassword == line)
			{
				encodedPasskey = encodedPassword;
				key = (const unsigned char *)encodedPasskey.c_str();
				return true;
			}
		}
	}
	return false;
}

void parse(string line)
{
	if (line.compare(0, 13, "create table ") == 0)
	{
		string tbl_name = line.substr(13, line.length() - 1);
		cout << "->Creating a new table: " << tbl_name << endl;
		//create empty txt file
		writeStringToFile(tbl_name + ".txt", "");
	}
	else if (line.compare(0, 11, "open table ") == 0)
	{
		string tbl_name = line.substr(11, line.length() - 1);
		cout << "->Opening table: " << tbl_name << endl;
		
		ifstream myfile("Files" + tbl_name + ".txt");
		if (!myfile.is_open())
		{
			cout << "Failed to open table. Table does not exist." << endl;
			exit;
		} else
		{
			myfile.close();
		}


		string cmd;
		while (cmd != "exit")
		{
			cmd = askForInput();
			if (cmd.compare(0, 7, "insert ") == 0)
			{
				string textToInsert = cmd.substr(7, cmd.length() - 1);
				textToInsert = encode_rc4(textToInsert, key);
				writeStringToFile(tbl_name + ".txt", textToInsert);
			}
			else if (cmd == "decode")
			{
				decode_file(tbl_name + ".txt", key);
			}
		}
		cout << "->Exited." << endl;
	}
}

int main()
{	
	if (login())
	{
		cout << "->Login successful!" << endl;
		string line;
		while (line != "exit") {
			line = askForInput();
			parse(line);			
		}
		
	} else
	{
		cout << "->Login unsuccessful!" << endl;
	}
	getchar();
    return 0;
}

