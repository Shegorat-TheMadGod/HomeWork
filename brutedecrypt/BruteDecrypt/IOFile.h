#pragma once
void ReadFile(const std::string& filePath, std::vector<unsigned char>& buf);
void ReadFile(const std::string& filePath, std::string& buf);
void WriteFile(const std::string& filePath, const std::vector<unsigned char>& buf);
void WriteFile(const std::string& filePath, const char* buf);
void AppendToFile(const std::string& filePath, const std::vector<unsigned char>& buf);