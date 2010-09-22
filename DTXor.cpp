#include "DTXor.h"

#include <QtCore/QDebug>

using namespace DT;

char DTXor::keyHistory[11] = { 0x40, 0x13, 0xF8, 0xB2, 0x84, 0x23, 0x04, 0xAE, 0x6F, 0x3D, 0x00 };
char DTXor::keyConfig[19] = { 0x16, 0x48, 0xF0, 0x85, 0xA9, 0x12, 0x03, 0x98, 0xBE, 0xCF, 0x42, 0x08, 0x76, 0xA5, 0x22, 0x84, 0x00 };

QByteArray DTXor::encryptString(int keyType, QByteArray str)
{
	unsigned int ki = 0;
	char *key = 0;

	switch(keyType) {
		case XorKeys::CONFIG:
			key = keyConfig;
		break;

		case XorKeys::HISTORY:
			key = keyHistory;
		break;
	}

	for(int i = 0; i < str.size(); i++) {
		str[i] = (str[i] ^ key[ki]) + (unsigned char)(i & 0xFF);

		ki++;
		if(ki >= strlen(key)) {
			ki = 0;
		}
	}

	return str;
}

QByteArray DTXor::decryptString(int keyType, QByteArray str)
{
	unsigned int ki = 0;
	char *key = 0;

	switch(keyType) {
		case XorKeys::CONFIG:
			key = keyConfig;
		break;

		case XorKeys::HISTORY:
			key = keyHistory;
		break;
	}

	for(int i = 0; i < str.size(); i++) {
		str[i] = (str[i] - (unsigned char)(i & 0xFF)) ^ key[ki];

		ki++;
		if(ki >= strlen(key)) {
			ki = 0;
		}
	}

	return str;
}
