// Datos.h
#pragma once
#include <utf8.h>
#include <string>
#include <fstream>
#include <stdexcept>
#include <utf8.h>

using namespace std;

class FileHandler {
public:
    string readFile(const string& filename);
    void writeCompressedFile(const string& filename, const string& compressedData);
};