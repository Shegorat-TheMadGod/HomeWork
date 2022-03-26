// XOREncoding.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include <string>

// TODO: This is an example of a library function
struct myStr {
	std::string str;
	std::string prevEncode;
};
void Encript(myStr* myStr, char* key) {
	int keyChar = 0;
	for (unsigned int i = 0; i < myStr->str.size(); i++) {
		if (i != 0) {
			if (!key[keyChar]) {
				keyChar = 0;
			}
			myStr->str[i] = myStr->str[i] ^ key[keyChar] ^ myStr->str[i - 1];
			myStr->prevEncode[i] = myStr->str[i - 1];
			keyChar++;
		}
		else {
			myStr->prevEncode[i] = rand() % 256;
			myStr->str[i] = myStr->str[i] ^ key[keyChar] ^ myStr->prevEncode[i];
			keyChar++;
		}
	}
}
void Decript(myStr* myStr, char* key) {
	int keyChar = 0;
	for (unsigned int i = 0; i < myStr->str.size(); i++) {
		if (i != 0) {
			if (!key[keyChar]) {
				keyChar = 0;
			}
			myStr->str[i] = myStr->str[i] ^ key[keyChar] ^ myStr->prevEncode[i];
			myStr->prevEncode[i] = myStr->str[i - 1];
			keyChar++;
		}
		else {
			myStr->str[i] = myStr->str[i] ^ key[keyChar] ^ myStr->prevEncode[i];
			keyChar++;
		}
	}
}