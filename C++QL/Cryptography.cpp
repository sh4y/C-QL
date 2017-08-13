#include "stdafx.h"
#include <openssl/sha.h>
#include <openssl/rc4.h>
#include <iomanip>
#include <sstream>

#include "cryptography.h";

using namespace std;

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