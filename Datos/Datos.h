// Datos.h
#pragma once
#include <utf8.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <map>
#include <bitset>
#include <utf8.h>

using namespace std;

class FileHandler {
public:
    string readFile(const string& filename);
    void writeFile(const string& filename, const string& content);
    //void writeCompressedFile(const string& filename, const string& compressedData);
    void writeCompressedFile(const string& filename, const string& compressedData, const map<uint32_t, string>& codes);
    pair<map<uint32_t, string>, string> readCompressedFile(const string& filename);
};