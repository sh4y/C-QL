#pragma once
using namespace std;

string sha256(const std::string str);

string decode_rc4(const string &data, const unsigned char * KEY);

string encode_rc4(const string &data, const unsigned char * KEY);

