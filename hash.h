#ifndef HASH_H
#define HASH_H

#include <string>
#include <cmath>

unsigned long long passToHash(std::string password)
{
	int len = password.length();
	unsigned long long temp = 0;
	for (int i = 0; i < len; i++)
	{
		int ch = int(password[i]);
		temp += ch*(unsigned long long)pow(128, len-1-i);
	}
	unsigned int w[4] = {};
	for (int i = 0; i<4; i++)
	{
		w[3-i] = temp%65521;
		temp /= 65521;
	}
	temp = (45912 * w[0] + 35511 * w[1] + 65169 * w[2] + 4625 * w[3]);
	return temp%65521;
}

#endif