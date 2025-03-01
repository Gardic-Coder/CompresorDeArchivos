// Datos.cpp
#include "Datos.h"

string FileHandler::readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) throw runtime_error("Error abriendo archivo: " + filename);
    
    string content((istreambuf_iterator<char>(file)), 
                istreambuf_iterator<char>());
    
    // Normalizar saltos de linea Windows (\r\n -> \n)
    content.erase(remove(content.begin(), content.end(), '\r'), content.end());
    
    // Validar UTF-8
    if (!utf8::is_valid(content.begin(), content.end())) {
        throw runtime_error("El archivo no tiene codificacion UTF-8 valida");
    }
    
    return content;
}

void FileHandler::writeCompressedFile(const string& filename, const string& compressedData) {
    ofstream file(filename, std::ios::binary);
    if (!file) throw runtime_error("Error creando archivo: " + filename);
    file.write(compressedData.data(), compressedData.size());
}