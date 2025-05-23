#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// Структура узла дерева
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
};

// Компаратор для приоритетной очереди
struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq; // Узел с меньшей частотой имеет более высокий приоритет
    }
};

// Функция для построения дерева Хаффмана
Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    // Создаем узлы для каждого символа и добавляем их в кучу
    for (const auto& pair : freqMap) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    // Построение дерева
    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        Node* newNode = new Node('0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        minHeap.push(newNode);
    }

    return minHeap.top(); // Корень дерева
}

// Функция для получения кодов Хаффмана
void getHuffmanCodes(Node* root, const string& str, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    // Если узел является листом, сохраняем код
    if (root->left == nullptr && root->right == nullptr) {
        huffmanCodes[root->ch] = str;
    }

    getHuffmanCodes(root->left, str + "0", huffmanCodes);
    getHuffmanCodes(root->right, str + "1", huffmanCodes);
}

// Функция для кодирования строки
string encode(const string& text, const unordered_map<char, string>& huffmanCodes) {
    string encodedString;
    for (char ch : text) {
        encodedString += huffmanCodes.at(ch);
    }
    return encodedString;
}

// Функция для освобождения памяти
void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    string text = "this is an example for huffman encoding";

    // Подсчет частоты символов
    unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }

    // Построение дерева Хаффмана
    Node* root = buildHuffmanTree(freqMap);

    // Получение кодов Хаффмана
    unordered_map<char, string> huffmanCodes;
    getHuffmanCodes(root, "", huffmanCodes);

    // Кодирование текста
    string encodedString = encode(text, huffmanCodes);

    // Вывод результатов
    cout << "Исходный текст: " << text << endl;
    cout << "Закодированная строка: " << encodedString << endl;

    cout << "Коды Хаффмана:" << endl;
    for (const auto& pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Освобождение памяти
    deleteTree(root);

    return 0;
}
