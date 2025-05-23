echo "# kursovaya" >> README.md
git init
git add README.md
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/hochuavtomat/kursovaya.git
git push -u origin main

#include <iostream>
#include <queue>
#include <map>           // Изменено с unordered_map на map для частот
#include <vector>
#include <memory>

// Переименована структура в TreeNode для ясности
struct TreeNode {
    char symbol;
    int frequency;
    std::shared_ptr<TreeNode> left, right;

    TreeNode(char s, int f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
    TreeNode(int f) : symbol('\0'), frequency(f), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(const std::shared_ptr<TreeNode>& a, const std::shared_ptr<TreeNode>& b) {
        return a->frequency > b->frequency;  // Очередь с приоритетом по минимальной частоте
    }
};

// Добавлена функция для построения дерева
std::shared_ptr<TreeNode> buildHuffmanTree(const std::map<char, int>& freqMap) {
    std::priority_queue<std::shared_ptr<TreeNode>, std::vector<std::shared_ptr<TreeNode>>, CompareNodes> minHeap;
    
    for (const auto& pair : freqMap) {
        minHeap.push(std::make_shared<TreeNode>(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        auto left = minHeap.top();
        minHeap.pop();
        auto right = minHeap.top();
        minHeap.pop();
        
        auto internalNode = std::make_shared<TreeNode>(left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;
        
        minHeap.push(internalNode);
    }
    
    return minHeap.empty() ? nullptr : minHeap.top();
}

// Изменены биты: левый - '1', правый - '0'
void generateHuffmanCodes(const std::shared_ptr<TreeNode>& node, const std::string& code, std::map<char, std::string>& codes) {
    if (!node) return;
    
    if (!node->left && !node->right) {
        codes[node->symbol] = code;
    }
    
    generateHuffmanCodes(node->left, code + "1", codes);  // Изменено здесь
    generateHuffmanCodes(node->right, code + "0", codes); // И здесь
}

// Добавлена проверка на отсутствующие символы
std::string encodeData(const std::string& input, const std::map<char, std::string>& codes) {
    std::string result;
    for (char ch : input) {
        auto it = codes.find(ch);
        if (it == codes.end()) {
            throw std::runtime_error("Symbol not found in codes!");
        }
        result += it->second;
    }
    return result;
}

// Изменены условия перемещения по дереву
std::string decodeData(const std::string& encodedStr, const std::shared_ptr<TreeNode>& root) {
    std::string result;
    auto currentNode = root;
    
    for (char bit : encodedStr) {
        if (bit == '1') {                   // Изменено здесь
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
        
        if (!currentNode->left && !currentNode->right) {
            result += currentNode->symbol;
            currentNode = root;
        }
    }
    
    return result;
}

int main() {
    std::string inputData = "abacabad";
    
    // Подсчёт частот с использованием map (отсортированный)
    std::map<char, int> frequencies;
    for (char ch : inputData) {
        frequencies[ch]++;
    }
    
    // Построение дерева
    auto huffmanRoot = buildHuffmanTree(frequencies);
    
    // Генерация кодов
    std::map<char, std::string> codeTable;
    generateHuffmanCodes(huffmanRoot, "", codeTable);
    
    // Вывод таблицы кодов
    std::cout << "Таблица кодов:\n";
    for (const auto& pair : codeTable) {
        std::cout << "'" << pair.first << "' : " << pair.second << "\n";
    }
    
    // Кодирование
    std::string encoded = encodeData(inputData, codeTable);
    std::cout << "\nЗакодированная строка: " << encoded << std::endl;
    
    // Декодирование
    std::string decoded = decodeData(encoded, huffmanRoot);
    std::cout << "Раскодированная строка: " << decoded << std::endl;
    
    // Проверка
    std::cout << "\nРезультат проверки: " << (inputData == decoded ? "Успех!" : "Ошибка!") << std::endl;

    return 0;
}