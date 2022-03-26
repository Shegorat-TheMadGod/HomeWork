#include "stdafx.h"
#include "openssl/evp.h"
#include <openssl/aes.h>
#include "openssl/sha.h"


bool PasswordToKey(std::string& password, unsigned char* key, unsigned char* iv,std::mutex* mut)
{
	const EVP_MD* dgst = EVP_get_digestbyname("md5");
	if (!dgst)
	{
		return false;
	}
	
	const unsigned char* salt = NULL;
		if (!EVP_BytesToKey(EVP_aes_128_cbc(), EVP_md5(), salt,
			reinterpret_cast<unsigned char*>(&password[0]),
			password.size(), 1, key, iv))
		{
			return false;
		}
	return true;
}

void EncryptAes(const std::vector<unsigned char> plainText, std::vector<unsigned char>& chipherText, unsigned char* key, unsigned char* iv)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
	{
		throw std::runtime_error("EncryptInit error");
	}

	std::vector<unsigned char> chipherTextBuf(plainText.size() + AES_BLOCK_SIZE);
	int chipherTextSize = 0;
	if (!EVP_EncryptUpdate(ctx, &chipherTextBuf[0], &chipherTextSize, &plainText[0], plainText.size())) {
		EVP_CIPHER_CTX_free(ctx);
		throw std::runtime_error("Encrypt error");
	}

	int lastPartLen = 0;
	if (!EVP_EncryptFinal_ex(ctx, &chipherTextBuf[0] + chipherTextSize, &lastPartLen)) {
		EVP_CIPHER_CTX_free(ctx);
		throw std::runtime_error("EncryptFinal error");
	}
	chipherTextSize += lastPartLen;
	chipherTextBuf.erase(chipherTextBuf.begin() + chipherTextSize, chipherTextBuf.end());

	chipherText.swap(chipherTextBuf);

	EVP_CIPHER_CTX_free(ctx);
}
bool DecryptAes(const std::vector<unsigned char> plainText, std::vector<unsigned char>& chipherText, unsigned char* key, unsigned char* iv)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
	{
		return false;
	}

	std::vector<unsigned char> chipherTextBuf(plainText.size() + AES_BLOCK_SIZE);
	int chipherTextSize = 0;
	if (!EVP_DecryptUpdate(ctx, &chipherTextBuf[0], &chipherTextSize, &plainText[0], plainText.size())) {
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	int lastPartLen = 0;
	if (!EVP_DecryptFinal_ex(ctx, &chipherTextBuf[0] + chipherTextSize, &lastPartLen)) {
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}
	chipherTextSize += lastPartLen;
	chipherTextBuf.erase(chipherTextBuf.begin() + chipherTextSize, chipherTextBuf.end());

	chipherText.swap(chipherTextBuf);

	EVP_CIPHER_CTX_free(ctx);
	return true;
}

void CalculateHash(const std::vector<unsigned char>& data, std::vector<unsigned char>& hash)
{
	std::vector<unsigned char> hashTmp(SHA256_DIGEST_LENGTH);

	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, &data[0], data.size());
	SHA256_Final(&hashTmp[0], &sha256);

	hash.swap(hashTmp);
}