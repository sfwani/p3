#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include <string>
#include <map>
#include <stack>
#include <vector>

class HuffmanTree : public HuffmanTreeBase {
private:
    // Root node of the tree
    HuffmanNode* root;
    
    // Map to store character frequencies
    std::map<char, unsigned> frequencyMap;
    
    // Map to store character codes
    std::map<char, std::string> codeMap;
    
    // Helper methods
    void buildFrequencyMap(const std::string& inputStr);
    void buildTree();
    void buildCodeMap(HuffmanNode* node, std::string code);
    void serializeTreeHelper(HuffmanNode* node, std::string& result) const;
    HuffmanNode* rebuildTree(const std::string& serializedTree, int& index);
    void destroyTree(HuffmanNode* node);
    
public:
    // Constructor and destructor
    HuffmanTree();
    ~HuffmanTree();
    
    // Required methods from base class
    std::string compress(const std::string inputStr) override;
    std::string serializeTree() const override;
    std::string decompress(const std::string inputCode, const std::string serializedTree) override;
};

#endif // HUFFMAN_TREE_HPP