#include "stdafx.h"


void ShowInfo(bool* finish, size_t* checkPasswordsCount, int passwordCharsCount, std::string& possibleStr) {
	auto atackStart = std::chrono::high_resolution_clock::now(); //start of brute atack
	auto atackPrevPeriod = std::chrono::high_resolution_clock::now();
	auto atackCurPeriod = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> atackDuration;
	std::chrono::duration<double> timeElapsed;
	double passwordsMaxCount = 0;
	for (int pos = 1; pos <= passwordCharsCount; pos++) {
		passwordsMaxCount += pow(possibleStr.size(), pos);
	}
	int passPerSec = 0;
	while (!(*finish)) {
		atackCurPeriod = std::chrono::high_resolution_clock::now();
		atackDuration = atackCurPeriod - atackPrevPeriod;
		timeElapsed = atackCurPeriod - atackStart;

		if (atackDuration.count() >= 0.1) {
			std::cout << "Time elapsed: " << timeElapsed.count() << " sec. "
				<< *checkPasswordsCount << " of " << passwordsMaxCount << " passwords were checked, "
				<< *checkPasswordsCount / passwordsMaxCount * 100 << "%" << ". "
				<< " Speed: " << (*checkPasswordsCount - passPerSec) / atackDuration.count() << " pas/sec" << std::endl;
			atackPrevPeriod = atackCurPeriod;
			passPerSec = *checkPasswordsCount;
		}
	}
	std::cout << "Time elapsed: " << timeElapsed.count() << ". "
		<< *checkPasswordsCount << " of " << passwordsMaxCount << " were checked, "
		<< *checkPasswordsCount / passwordsMaxCount * 100 << "%" << ". "
		<< " Speed: " << (*checkPasswordsCount - passPerSec) / atackDuration.count() << " pas/sec" << std::endl;
}