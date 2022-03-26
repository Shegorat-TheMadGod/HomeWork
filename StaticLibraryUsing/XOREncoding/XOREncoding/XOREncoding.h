#pragma once
#include <string>
struct myStr {
	std::string str;
	std::string prevEncode;
};
void Encript(myStr* myStr, char* key);
void Decript(myStr* myStr, char* key);