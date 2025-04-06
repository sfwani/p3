#include "HuffmanTree.hpp"

// Constructor
HuffmanTree::HuffmanTree() : root(nullptr) {
}

// Destructor
HuffmanTree::~HuffmanTree() {
    destroyTree(root);
}

// Helper method to destroy the tree recursively
void HuffmanTree::destroyTree(HuffmanNode* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

// Helper method to build frequency map from input string
void HuffmanTree::buildFrequencyMap(const std::string& inputStr) {
    frequencyMap.clear();
    
    // Count frequency of each character
    for (char c : inputStr) {
        frequencyMap[c]++;
    }
}

// Helper method to build Huffman tree using the frequency map
void HuffmanTree::buildTree() {
    // Clear old tree
    destroyTree(root);
    root = nullptr;
    
    // Create a priority queue
    HeapQueue<HuffmanNode*, HuffmanNode::Compare> pq;
    
    // Add all characters to the priority queue as leaf nodes
    for (const auto& pair : frequencyMap) {
        HuffmanNode* node = new HuffmanNode(pair.first, pair.second);
        pq.insert(node);
    }
    
    // Special case: if there's only one unique character in the input
    if (pq.size() == 1) {
        HuffmanNode* onlyNode = pq.min();
        pq.removeMin();
        
        // Create a parent node with the only character as left child
        root = new HuffmanNode('\0', onlyNode->getFrequency());
        root->left = onlyNode;
        root->right = nullptr;
        return;
    }
    
    // Build the tree by combining nodes until only one remains
    while (pq.size() > 1) {
        // Get the two nodes with smallest frequencies
        HuffmanNode* left = pq.min();
        pq.removeMin();
        
        HuffmanNode* right = pq.min();
        pq.removeMin();
        
        // Create a new internal node with these two as children
        unsigned combinedFreq = left->getFrequency() + right->getFrequency();
        HuffmanNode* parent = new HuffmanNode('\0', combinedFreq);
        parent->left = left;
        parent->right = right;
        
        // Add the new node back to the queue
        pq.insert(parent);
    }
    
    // The last node is the root of the Huffman tree
    if (!pq.empty()) {
        root = pq.min();
        pq.removeMin();
    }
}

// Helper method to build code map by traversing the tree
void HuffmanTree::buildCodeMap(HuffmanNode* node, std::string code) {
    if (node == nullptr) return;
    
    // If this is a leaf node, store the code
    if (node->left == nullptr && node->right == nullptr) {
        codeMap[node->getCharacter()] = code;
    }
    
    // Traverse left (add 0)
    buildCodeMap(node->left, code + "0");
    
    // Traverse right (add 1)
    buildCodeMap(node->right, code + "1");
}

// Compress input string into Huffman code
std::string HuffmanTree::compress(const std::string inputStr) {
    // If input is empty, return empty
    if (inputStr.empty()) return "";
    
    // Clear previous data
    codeMap.clear();
    
    // Build frequency map
    buildFrequencyMap(inputStr);
    
    // Build Huffman tree
    buildTree();
    
    // Build code map by traversing the tree
    buildCodeMap(root, "");
    
    // Compress the input string using the codes
    std::string compressed;
    for (char c : inputStr) {
        compressed += codeMap[c];
    }
    
    return compressed;
}

// Helper method to serialize the tree recursively
void HuffmanTree::serializeTreeHelper(HuffmanNode* node, std::string& result) const {
    if (node == nullptr) return;
    
    // Post-order traversal
    serializeTreeHelper(node->left, result);
    serializeTreeHelper(node->right, result);
    
    // If it's a leaf node, add 'L' followed by the character
    if (node->left == nullptr && node->right == nullptr) {
        result += "L";
        result += node->getCharacter();
    }
    // If it's an internal node, add 'B'
    else {
        result += "B";
    }
}

// Serialize the tree
std::string HuffmanTree::serializeTree() const {
    std::string serialized;
    serializeTreeHelper(root, serialized);
    return serialized;
}

// Helper method to rebuild tree from serialized string
HuffmanNode* HuffmanTree::rebuildTree(const std::string& serializedTree, int& index) {
    if (index < 0) return nullptr;
    
    // Read the next character
    char nodeType = serializedTree[index--];
    
    // If it's a branch node
    if (nodeType == 'B') {
        HuffmanNode* node = new HuffmanNode('\0', 0);
        
        // Right comes first since we're going backwards
        node->right = rebuildTree(serializedTree, index);
        node->left = rebuildTree(serializedTree, index);
        
        return node;
    }
    // If it's a leaf node
    else if (nodeType == 'L') {
        // Read the character
        char c = serializedTree[index--];
        return new HuffmanNode(c, 0);
    }
    
    return nullptr;
}

// Decompress code using serialized tree
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree) {
    // Clear old tree
    destroyTree(root);
    root = nullptr;
    
    // If input is empty, return empty
    if (inputCode.empty() || serializedTree.empty()) return "";
    
    // Rebuild the tree from serialized string
    int index = serializedTree.length() - 1;
    root = rebuildTree(serializedTree, index);
    
    // Decompress the code using the tree
    std::string decompressed;
    HuffmanNode* current = root;
    
    for (char bit : inputCode) {
        // Navigate based on bit
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        // If we reached a leaf node
        if (current->left == nullptr && current->right == nullptr) {
            decompressed += current->getCharacter();
            current = root;
        }
    }
    
    return decompressed;
}