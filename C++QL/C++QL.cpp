// C++QL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Cryptography.h";
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>


using namespace std;

string FN = "db.txt";
string KEY_FILE = "pw.txt";
string encodedPasskey;
const unsigned char * key;

void writeStringToFile(string fn, string text)
{
	ofstream openFile;
	openFile.open(fn, fstream::app);
	if (openFile.is_open())
	{
		openFile << text << endl;
		openFile.close();
	}

	else cout << "Couldn't open the file" << endl;

}

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

void decode_file(string fn)
{
	ifstream myfile(fn);
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

int main()
{	
	if (login())
	{
		cout << "->Login successful!" << endl;
		/*decode_file(FN);
			string encodedText = encode_rc4(line, key);

			writeStringToFile(FN, encodedText);
		}*/

		while (true) {
			string line = askForInput();
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
						decode_file(tbl_name + ".txt");
					}
				}
				cout << "->Exited." << endl;
			}
		}
		
	} else
	{
		cout << "->Login unsuccessful!" << endl;
	}
	getchar();
    return 0;
}

