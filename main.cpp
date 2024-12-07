#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Node structure for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Compare structure for priority queue
struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// Function to generate Huffman codes from the tree
void generateCodes(Node* root, const string& code, unordered_map<char, string>& huffmanCode) {
    if (!root)
        return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = code;
    }

    generateCodes(root->left, code + "0", huffmanCode);
    generateCodes(root->right, code + "1", huffmanCode);
}

// Function to build Huffman Tree
Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (auto& pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Combine nodes to build the tree
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        int sum = left->freq + right->freq;
        Node* newNode = new Node('\0', sum);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    return pq.top();
}

// Function to encode the input text
string encode(const string& text, const unordered_map<char, string>& huffmanCode) {
    string encodedStr;
    for (char ch : text) {
        encodedStr += huffmanCode.at(ch);
    }
    return encodedStr;
}

// Function to decode the encoded text
string decode(const string& encodedStr, Node* root) {
    string decodedStr;
    Node* currentNode = root;

    for (char bit : encodedStr) {
        if (bit == '0')
            currentNode = currentNode->left;
        else
            currentNode = currentNode->right;

        if (!currentNode->left && !currentNode->right) {
            decodedStr += currentNode->ch;
            currentNode = root;
        }
    }

    return decodedStr;
}

int main() {
    // Read input text from file
    ifstream inputFile("input.txt");
    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // Calculate frequencies
    unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }

    // Build Huffman Tree
    Node* root = buildHuffmanTree(freqMap);

    // Generate Huffman codes
    unordered_map<char, string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    // Encode the input text
    string encodedStr = encode(text, huffmanCode);

    // Save the encoded text to a file
    ofstream encodedFile("encoded.txt");
    encodedFile << encodedStr;
    encodedFile.close();

    // Decode the encoded text
    string decodedStr = decode(encodedStr, root);

    // Save the decoded text to a file
    ofstream decodedFile("decoded.txt");
    decodedFile << decodedStr;
    decodedFile.close();

    cout << "Huffman Encoding and Decoding completed. Check encoded.txt and decoded.txt for results." << endl;

    return 0;
}
