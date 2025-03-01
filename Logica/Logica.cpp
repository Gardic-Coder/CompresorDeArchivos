#include "Logica.h"

HuffmanManager::HuffmanManager() : root(nullptr), originalSize(0), compressedBytes(0) {}

HuffmanManager::~HuffmanManager() {
	deleteTree(root);
}

void HuffmanManager::processData(const string& content) {
	originalSize = content.size();
	buildTree(content);
	generateCodes();
	generateCompressedData(content);
}

void HuffmanManager::buildTree(const string& content) {
	freqMap.clear();

	auto it = content.begin();
	while (it != content.end()) {
		uint32_t cp = utf8::next(it, content.end());
		freqMap[cp]++;
	}

	priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
	for (const auto& pair : freqMap) {
		pq.push(new HuffmanNode(pair.first, pair.second));
	}

	if (pq.empty()) {
		root = nullptr;
		return;
	}

	while (pq.size() > 1) {
		auto left = pq.top();
		pq.pop();
		auto right = pq.top();
		pq.pop();

		auto internal = new HuffmanNode(0, left->freq + right->freq);
		internal->left = left;
		internal->right = right;
		pq.push(internal);
	}

	root = pq.top();
}

void HuffmanManager::generateCodes() {
	codes.clear();
	if (root) traverseTree(root, "");
}

void HuffmanManager::traverseTree(HuffmanNode* node, string code) {
	if (!node->left && !node->right) {
		if (code.empty()) code = "0";
		codes[node->code_point] = code;
		return;
	}

	if (node->left) traverseTree(node->left, code + "0");
	if (node->right) traverseTree(node->right, code + "1");
}

void HuffmanManager::generateCompressedData(const string& content) {
	compressedData.clear();
	string bits;

	auto it = content.begin();
	while (it != content.end()) {
		uint32_t cp = utf8::next(it, content.end());
		bits += codes[cp];
	}

	// Convertir bits a bytes
	size_t padding = (8 - (bits.size() % 8)) % 8;
	bits.append(padding, '0');

	for (size_t i = 0; i < bits.size(); i += 8) {
		string byte = bits.substr(i, 8);
		compressedData += static_cast<char>(bitset<8>(byte).to_ulong());
	}

	compressedBytes = compressedData.size();
}

vector<pair<uint32_t, int>> HuffmanManager::getFrequencyStats() const {
	vector<pair<uint32_t, int>> stats(freqMap.begin(), freqMap.end());
	sort(stats.begin(), stats.end(), [](const auto& a, const auto& b) {
		if (a.second == b.second) return a.first < b.first;
		return a.second < b.second;
	});
	return stats;
}

map<uint32_t, string> HuffmanManager::getCodeTable() const {
	return codes;
}

vector<string> HuffmanManager::getTreeStructure() const {
	vector<string> treeLines;
	treeToString(root, 0, treeLines);
	return treeLines;
}

string HuffmanManager::getCompressedData() const {
	return compressedData;
}

size_t HuffmanManager::getOriginalSize() const {
	return originalSize;
}

size_t HuffmanManager::getCompressedSize() const {
	return compressedBytes;
}

double HuffmanManager::getCompressionRatio() const {
	if (originalSize == 0) return 0.0;
	return (1.0 - static_cast<double>(compressedBytes) / originalSize) * 100.0;
}

// Metodos privados
void HuffmanManager::deleteTree(HuffmanNode* node) {
	if (node) {
		deleteTree(node->left);
		deleteTree(node->right);
		delete node;
	}
}

void HuffmanManager::treeToString(HuffmanNode* node, int depth, std::vector<std::string>& result) const {
    if (!node) return;

    std::stringstream ss;
    std::string indent(depth * 4, ' ');
    
    if (node->left || node->right) {
        ss << indent << "|-- [" << node->freq << "]";
    } else {
        ss << indent << "|-- ";
        std::string utf8_char;
        utf8::append(node->code_point, std::back_inserter(utf8_char));
        
        // Manejar caracteres especiales
        switch(node->code_point) {
            case U' ':  utf8_char = "[Espacio]"; break;
            case U'\n': utf8_char = "[Salto]";    break;
            case U'\t': utf8_char = "[Tab]";     break;
            default:
                if (!isprint(static_cast<unsigned char>(utf8_char[0]))) {
                    std::stringstream hex;
                    hex << "[0x" << std::hex << std::setw(4) << std::setfill('0') 
                        << node->code_point << "]";
                    utf8_char = hex.str();
                } else {
                    utf8_char = "'" + utf8_char + "'";
                }
        }
        
        ss << utf8_char << " (" << node->freq << ")";
    }

    result.push_back(ss.str());
    treeToString(node->left, depth + 1, result);
    treeToString(node->right, depth + 1, result);
}

/*void HuffmanManager::treeToString(HuffmanNode* node, int depth, vector<string>& result) const {
    if (!node) return;

    stringstream ss;
    string indent(depth * 4, ' ');

    if (node->left || node->right) {
        ss << indent << "|-- [" << node->freq << "]";
    } else {
        ss << indent << "|-- ";
        
        // Convertir code_point a UTF-8
        string utf8_char;
        utf8::append(node->code_point, back_inserter(utf8_char));
        
        // Manejar caracteres especiales
        switch(node->code_point) {
            case U' ':  utf8_char = "[Espacio]"; break;
            case U'\n': utf8_char = "[Salto]";   break;
            case U'\t': utf8_char = "[Tab]";     break;
            case U'\r': utf8_char = "[Retorno]";  break;
            default:
                if (!iswprint(node->code_point)) {  // Usar funcion wide para Unicode
                    stringstream hex;
                    hex << "[U+" << hex << setw(4) << setfill('0') 
                        << node->code_point << "]";
                    utf8_char = hex.str();
                }
        }
        
        ss << utf8_char << " (" << node->freq << ")";
    }

    result.push_back(ss.str());
    treeToString(node->left, depth + 1, result);
    treeToString(node->right, depth + 1, result);
}*/

/*void HuffmanManager::treeToString(HuffmanNode* node, int depth, vector<string>& result) const {
    if (!node) return;

    stringstream ss;
    string indent(depth * 4, ' ');
    unsigned char uc = static_cast<unsigned char>(node->code_point);

    if (node->left || node->right) {
        ss << indent << "|-- [" << node->freq << "]";
    } else {
        ss << indent << "|-- ";

        // Caracteres especiales con nombre
        if (uc == ' ') {
            ss << "[Espacio]";
        } else if (uc == '\n') {
            ss << "[Salto de Linea]";
        } else if (uc == '\t') {
            ss << "[Tabulador]";
        }
        // Caracteres extendidos (ISO-8859-1/Latin1)
        else if (uc >= 0xA0 && uc <= 0xFF) { // Rango de caracteres extendidos
            ss << "'" << node->code_point << "'";
        }
        // Caracteres no imprimibles
        else if (!isprint(uc)) {
            ss << "[0x" << hex << setw(2) << setfill('0')
               << static_cast<int>(uc) << dec << "]";
        }
        // Caracteres normales
        else {
            ss << "'" << node->code_point << "'";
        }

        ss << " (" << node->freq << ")";
    }

    result.push_back(ss.str());
    treeToString(node->left, depth + 1, result);
    treeToString(node->right, depth + 1, result);
}*/