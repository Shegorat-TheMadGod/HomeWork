// BruteDecrypt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "openssl/evp.h"
#include <openssl/aes.h>
#include "openssl/sha.h"

#include "IOFile.h"
#include "Info.h"
#include "Cryptography.h"


class BruteDecrypt {
private:
	void GetConfigData(const std::string& configFile) {
		m_possibleStr = configFile.substr(configFile.find("PossibleChars = ") + strlen("PossibleChars = "),
			configFile.find(";") - configFile.find("PossibleChars = ") - strlen("PossibleChars = ")); //possible chars for password
		m_passwordCharsCount = stoi(configFile.substr(configFile.find("PasswordMaxLength = ") + strlen("PasswordMaxLength = "),
			configFile.find(";", configFile.find("PasswordMaxLength = ")) - configFile.find("PasswordMaxLength = ") - strlen("PasswordMaxLength = ")));
		m_maxThreadsCount = stoi(configFile.substr(configFile.find("ThreadsMaxCount = ") + strlen("ThreadsMaxCount = "),
			configFile.find(";", configFile.find("ThreadsMaxCount = ")) - configFile.find("ThreadsMaxCount = ") - strlen("ThreadsMaxCount = ")));
	}
	void GetPathToReadWrite(const std::string& pathToFileRead) {
		m_pathToFileRead = pathToFileRead;
		m_pathToFileWrite = m_pathToFileRead;
		m_pathToFileWrite += "_CheckedPassword";
	}
	void GetTextToDecrypt() {
		ReadFile(m_pathToFileRead, m_plainText);
	}
	void HashCutting() {
		for (size_t pos = 0; pos < m_hash.size(); pos++) {
			m_hash[m_hash.size() - 1 - pos] = m_plainText[m_plainText.size() - 1];
			m_plainText.pop_back();
		}
	}

	static constexpr int m_arrSize = 2000;
	std::mutex m_mutex;
	bool m_logPassCheck;
	size_t m_checkPasswordsCount = 0; //quantity of checked passwords
	std::string m_strForCheckedPassOut;
	size_t m_threadsCount = 0;
	std::string m_possibleStr;
	size_t m_passwordCharsCount;
	size_t m_maxThreadsCount;
	std::string m_pathToFileRead;
	std::string m_pathToFileWrite; //path for checked passwords output
	bool m_finish = false; //process termination condition
	std::array<std::string, m_arrSize> m_arr = {}; //array for thransition to the thread
	int m_arrSizeCounter = 0; //count of elements at the array
	std::vector<std::thread> m_threadsVector; //vector of threads

	std::vector<unsigned char> m_plainText; //encrypted text
	std::vector<unsigned char> m_hash; //hash of encrypted text
	std::string m_bruteStr;

public:
	BruteDecrypt(const std::string& configFile, const std::string& pathToFileRead, bool logPassCheck) {
		GetConfigData(configFile);
		GetPathToReadWrite(pathToFileRead);
		ReadFile(pathToFileRead, m_plainText);
		CalculateHash(m_plainText, m_hash);
		HashCutting();
		m_logPassCheck = logPassCheck;

	}

	void PassBatchProducer() {
		OpenSSL_add_all_digests();
		std::vector<int> charCount;
		std::array<std::string, m_arrSize>arr = {};
		for (size_t i = 0; i < m_passwordCharsCount; i++) {
			charCount.push_back(-1);
		}
		m_bruteStr.push_back('\n');
		std::thread* infoThread = new std::thread();
		if (m_logPassCheck) {
			*infoThread=std::thread(ShowInfo, &m_finish, &m_checkPasswordsCount, m_passwordCharsCount, m_possibleStr); //data output into console window
		}
		charCount[0] = 0;
		bool check = false;
		while (!m_finish) {
			size_t i = 0;
			while (charCount[i] == m_possibleStr.size()) {
				charCount[i] = 0;
				if (++i > m_passwordCharsCount - 1 || m_finish) {
					check = true;
					m_threadsVector.emplace(m_threadsVector.begin(), std::thread(&BruteDecrypt::PassBatchToThread, this, arr));
					m_threadsCount++;
					break;
				}
				charCount[i] += 1;
			}
			if (check) {
				break;
			}
			for (size_t j = 0; j <= i; j++) {
				if (m_bruteStr.size() <= i) {
					m_bruteStr.push_back(m_possibleStr.front());
				}
				if (j < m_passwordCharsCount) {
					m_bruteStr[j] = m_possibleStr[charCount[j]];
				}
			}
			arr[m_arrSizeCounter] = m_bruteStr;
			if (m_arrSizeCounter == arr.size() - 1) {
				m_threadsVector.push_back(std::thread(&BruteDecrypt::PassBatchToThread, this, arr));
				m_threadsCount++;
				arr = {};
				m_arrSizeCounter = 0;
			}
			m_arrSizeCounter++;
			charCount[0]++;
			if (m_threadsCount >= m_maxThreadsCount) {
				for (int pos = m_threadsCount - 1; pos >= 0; pos--) {
					if (m_threadsVector[pos].native_handle()) {
						m_threadsVector[pos].join();
					}
					m_threadsVector.pop_back();
					m_threadsCount--;
					if (m_threadsCount < (3 * m_maxThreadsCount / 4)) {
						break;
					}
				}
			}
		}
		for (int pos = m_threadsCount - 1; pos >= 0; pos--) {
			m_threadsVector[pos].join();
			m_threadsVector.pop_back();
		}
		m_threadsCount = 0;
		m_finish = true;
		if (m_logPassCheck) {
			infoThread->join();
		}
		const char* constStr = reinterpret_cast<const char*>(m_strForCheckedPassOut.c_str());
		WriteFile(m_pathToFileWrite, constStr);
		delete infoThread;

	}
	void PassBatchToThread(std::array<std::string, m_arrSize>& arr) {
		std::vector<unsigned char> chipherText; //decrypted text
		std::vector<unsigned char> chipherHash; //is required for hash checking
		unsigned char key[EVP_MAX_KEY_LENGTH];
		unsigned char iv[EVP_MAX_IV_LENGTH];
		std::string tmpStr;
		for (size_t pos = 0; pos < arr.size() - 1; pos++) {
			if (m_finish == true) {
				break;
			}
			tmpStr += arr[pos];
			tmpStr += "\n";
			m_checkPasswordsCount++;
			if (PasswordToKey(arr[pos], &key[0], &iv[0],&m_mutex)) {
				if (DecryptAes(m_plainText, chipherText, &key[0], &iv[0])) {
					CalculateHash(chipherText, chipherHash);
					if (m_hash == chipherHash) {
						WriteFile("plain_text_brute_force", chipherText);
						std::cout << "The password is: " << arr[pos] << std::endl;
						m_finish = true;
						break;
					}
				}
			}
		}
		const std::lock_guard<std::mutex> lock(m_mutex);
		m_strForCheckedPassOut += tmpStr;
	}
};


int main(int argc, char* argv[])
{
	try {
		std::string configFile;
		ReadFile("config", configFile);


		bool logPassCheck = false;
		std::string pathToFileRead;
		for (int pos = 0; pos < argc; pos++) {
			if (strcmp(argv[pos], "--log_passwords") == 0) {
				logPassCheck = true;
				break;
			}
		}
		std::cout << "Enter path to file: ";
		getline(std::cin, pathToFileRead);
		BruteDecrypt bruteDecrypt(configFile, pathToFileRead, logPassCheck);
		bruteDecrypt.PassBatchProducer();
	}

	catch (const std::runtime_error& ex)
	{
		std::cerr << ex.what();
	}

}

