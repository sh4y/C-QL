// C++QL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <openssl/sha.h>
#include <openssl/rc4.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>


using namespace std;

string FN = "db.txt";
string KEY_FILE = "pw.txt";
string encodedPasskey;
const unsigned char * key;


string sha256(const string str)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);
	stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		ss << hex << setw(2) << setfill('0') << (int)hash[i];
	}
	return ss.str();
}

string decode_rc4(const string &data, const unsigned char * KEY) {
	RC4_KEY key;

	int len = data.size();
	unsigned char *obuf = (unsigned char*)malloc(len + 1);
	memset(obuf, 0, len + 1);

	RC4_set_key(&key, sizeof(KEY), KEY);
	RC4(&key, len, (const unsigned char*)data.c_str(), obuf);

	string decode_data((char*)obuf, len);
	free(obuf);

	return decode_data;
}

string encode_rc4(const string &data, const unsigned char * KEY) {
	RC4_KEY key;

	int len = data.size();
	unsigned char *obuf = (unsigned char*)malloc(len + 1);
	memset(obuf, 0, len + 1);

	RC4_set_key(&key, sizeof(KEY), KEY);
	RC4(&key, len, (const unsigned char*)data.c_str(), obuf);

	string encode_data((char*)obuf, len);
	free(obuf);

	return encode_data;
}

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
	cout << "Please enter your password: ";
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
		while (getline(myfile, line))
		{
			string txt = decode_rc4(line, key);
			cout << txt << endl;
		}
	}
	myfile.close();
}

int main()
{	
	if (login())
	{
		decode_file(FN);

		cout << "Login successful!" << endl;
		cout << "What would you like to write to file?: ";
		
		while (true) {
			char text[1000];
			cin.getline(text, sizeof text);

			string line = (string)text;
			string encodedText = encode_rc4(line, key);

			writeStringToFile(FN, encodedText);
		}
		
	} else
	{
		cout << "Login unsuccessful!" << endl;
	}
	getchar();
    return 0;
}

