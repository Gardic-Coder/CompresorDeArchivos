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

void FileHandler::writeFile(const string& filename, const string& content) {
    ofstream file(filename, ios::binary);
    if (!file) throw runtime_error("Error creando archivo: " + filename);
    file.write(content.data(), content.size());
}

void FileHandler::writeCompressedFile(const string& filename, 
                                     const string& compressedData,
                                     const map<uint32_t, string>& codes) {
    ofstream file(filename, ios::binary);
    if (!file) throw runtime_error("Error creando archivo: " + filename);
    
    // Guardar número de códigos (1 byte)
    uint8_t numCodes = static_cast<uint8_t>(codes.size());
    file.write(reinterpret_cast<char*>(&numCodes), sizeof(numCodes));
    
    // Guardar códigos
    for (const auto& [cp, code] : codes) {
        // Guardar punto de código (4 bytes)
        file.write(reinterpret_cast<const char*>(&cp), sizeof(cp));
        
        // Guardar longitud del código (1 byte)
        uint8_t codeLength = static_cast<uint8_t>(code.size());
        file.write(reinterpret_cast<char*>(&codeLength), sizeof(codeLength));
        
        // Guardar código (como bits)
        bitset<256> bits(code);
        file.write(reinterpret_cast<char*>(&bits), codeLength / 8 + 1);
    }
    
    // Guardar datos comprimidos
    file.write(compressedData.data(), compressedData.size());
}

pair<map<uint32_t, string>, string> 
FileHandler::readCompressedFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) throw runtime_error("Error abriendo archivo: " + filename);
    
    // Leer número de códigos
    uint8_t numCodes;
    file.read(reinterpret_cast<char*>(&numCodes), sizeof(numCodes));
    
    map<uint32_t, string> codes;
    for (int i = 0; i < numCodes; ++i) {
        // Leer punto de código
        uint32_t cp;
        file.read(reinterpret_cast<char*>(&cp), sizeof(cp));
        
        // Leer longitud del código
        uint8_t codeLength;
        file.read(reinterpret_cast<char*>(&codeLength), sizeof(codeLength));
        
        // Leer código
        bitset<256> bits;
        file.read(reinterpret_cast<char*>(&bits), codeLength / 8 + 1);
        codes[cp] = bits.to_string().substr(256 - codeLength);
    }
    
    // Leer datos comprimidos
    string compressedData((istreambuf_iterator<char>(file)), 
                              istreambuf_iterator<char>());
    
    return {codes, compressedData};
}

/*void FileHandler::writeCompressedFile(const string& filename, const string& compressedData) {
    ofstream file(filename, std::ios::binary);
    if (!file) throw runtime_error("Error creando archivo: " + filename);
    file.write(compressedData.data(), compressedData.size());
}*/