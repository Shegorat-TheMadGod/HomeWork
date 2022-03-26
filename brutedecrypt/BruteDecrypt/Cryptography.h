#pragma once
bool PasswordToKey(std::string& password, unsigned char* key, unsigned char* iv, std::mutex* mut);
void EncryptAes(const std::vector<unsigned char> plainText, std::vector<unsigned char>& chipherText, unsigned char* key, unsigned char* iv);
bool DecryptAes(const std::vector<unsigned char> plainText, std::vector<unsigned char>& chipherText, unsigned char* key, unsigned char* iv);
void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash);