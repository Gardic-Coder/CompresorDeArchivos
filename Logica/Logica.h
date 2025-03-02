// Logica.h
#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <bitset>
#include <cwctype> // Para iswprint
#include <utf8.h>

using namespace std;

struct HuffmanNode {
    uint32_t code_point;  // Punto de código Unicode
    unsigned freq;
    HuffmanNode *left, *right;
    HuffmanNode(uint32_t cp, unsigned f) : code_point(cp), freq(f), left(nullptr), right(nullptr) {}
};

class HuffmanManager {
public:

    HuffmanManager();
    ~HuffmanManager();
    
    void processData(const std::string& content);
    void traverseTree(HuffmanNode* node, string code);
    
    vector<pair<uint32_t, int>> getFrequencyStats() const;
    map<uint32_t, string> getCodeTable() const;
    vector<string> getTreeStructure() const;
    string getCompressedData() const;
    
    size_t getOriginalSize() const;
    size_t getCompressedSize() const;
    double getCompressionRatio() const;
    
    string decompress(const map<uint32_t, string>& codes, const string& compressedData);
    HuffmanNode* rebuildTree(const map<uint32_t, string>& codes);

private:
    HuffmanNode* root;
    map<uint32_t, int> freqMap;
    map<uint32_t, string> codes;
    string compressedData;
    size_t originalSize;
    size_t compressedBytes;

    void buildTree(const string& content);
    void generateCodes();
    void deleteTree(HuffmanNode* node);
    //void treeToString(HuffmanNode* node, int depth, vector<string>& result) const;
    void treeToString(HuffmanNode* node, int depth, vector<string>& result, bool isLast, string prefix) const;
    //void treeToString(HuffmanNode* node, int depth, vector<string>& result, bool isLast = true, string prefix = "") const;
    void generateCompressedData(const string& content);
    
    struct Compare {
        bool operator()(HuffmanNode* a, HuffmanNode* b) {
            return a->freq > b->freq;
        }
    };
};