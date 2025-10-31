#include "Trie.h"

// trie_counter should be incremented by one after
// each word is checked in the 'filter' function.
int trie_counter = 0;

TrieNode::TrieNode() {
}

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    deleteTrie(root);
}

void Trie::deleteTrie(TrieNode* node) {
    // recursively deletes a node and its children

    // node is null - nothing to delete
    if (node == nullptr) {
        return;
    }

    // iterate through all children of the current node
    // call the function with the child node, delete from bottom to top
    unordered_map<char, TrieNode*>::iterator it;
    for (it = node->children.begin(); it != node->children.end(); ++it) {
        TrieNode* child = it->second;
        // recursively delete child node
        deleteTrie(child);
    }

    // delete node after children are deleted
    delete node;
}

unsigned int Trie::countLeaves(TrieNode* node) const {
    // counts the number of leaf nodes (nodes with no children) in the trie

    // no node = no leaves
    if (node == nullptr) {
        return 0;
    }

    // if node is root, and it has no children, size = 0 -> has no words
    if (node == root && node->children.empty()) {
        return 0;
    }

    if (node->children.empty()) {
        // no children means this node is a leaf
        return 1;
    }

    unsigned int count = 0;
    // iterate through all the children and increase the count
    unordered_map<char, TrieNode*>::const_iterator it;
    for (it = node->children.begin(); it != node->children.end(); ++it) {
        // recursively count leaves in children
        count += countLeaves(it->second);
    }
    return count;
}

unsigned int Trie::size() const {
    return countLeaves((root));
}

bool Trie::insert(const std::string &word) {
    // inserts word

    // word must be 5 letters
    if (word.length() != 5) return false;

    TrieNode* current = root;
    int i = 0;

    // traverse through all the existing nodes to check if there's a prefix match
    for (; i < 5; ++i) {
        char c = word[i];

        // if the child doesn't exist, break to insert
        if (current->children.count(c) == 0) {
            break;
        }
        current = current->children[c];
    }

    // if the whole word is traversed then the prefix exists
    if (i == 5) {
        // word already exists
        return false;
    }

    // insert the remaining letters
    for (; i < 5; ++i) {
        char c = word[i];
        current->children[c] = new TrieNode();
        current = current->children[c];
    }

    return true;
}

std::string Trie::getFirstWord() {
    // return the first word in trie

    std::string word;
    TrieNode* current = root;

    while (word.size() < 5 && current) {
        if (current->children.empty()) {
            break; // incomplete word
        }

        auto it = current->children.begin();
        char letter = it->first;
        TrieNode* next = it->second;

        word += letter;
        current = next;
    }

    // return only if it's exactly 5 letters
    if (word.length() == 5)
        return word;

    // try getAllWords() as backup
    std::list<std::string> allWords = getAllWords();
    for (const std::string& w : allWords) {
        if (w.length() == 5)
            return w;
    }

    return ""; // no valid word found
}

std::list<std::string> Trie::getAllWords() const {
    std::list<std::string> words;
    std::string currentWord = "";
    getWordsRecursively(root, currentWord, words);
    return words;
}

void Trie::getWordsRecursively(TrieNode *node, std::string currentWord, std::list<std::string> &words) const {
    // base case - full word
    if (currentWord.size() == 5) {
        words.push_back(currentWord);
        return;
    }

    // recursive case
    for (auto& child : node->children) {
        getWordsRecursively(child.second, currentWord + child.first, words);
    }
}

bool Trie::filter(const std::string &guess, const std::string &pattern) {
    // modifies tree based on filtering criteria
    // false if length of inputs is not 5

    // check for invalid input lengths
    if (guess.length() != 5 || pattern.length() != 5) {
        return false;
    }
    trie_counter++;
    // call recursive function
    filterRecursively(root, "", guess, pattern);
    cout << "1 cycle" << endl;
    return true;
}

void Trie::filterRecursively(TrieNode* node, std::string currentWord, const std::string &guess, const std::string &pattern) {

    // if node is null
    if (!node) return;

    // base case: full word is formed
    if (currentWord.size() == 5) {
        bool matches = true;
        // letter by letter
        for (int i = 0; i < 5; ++i) {
            char g_char = guess[i];
            char p_char = pattern[i];
            char w_char = currentWord[i];

            if (p_char == 'g') {
                if (w_char != g_char) {
                    matches = false;
                    break;
                }
            }
            else if (p_char == 'y') {
                if (w_char == g_char || currentWord.find(g_char) == std::string::npos) {
                    matches = false;
                    break;
                }
            }
            else if (p_char == 'b') {
                if (currentWord.find(g_char) != std::string::npos) {
                    matches = false;
                    break;
                }
            }
        }

        // if this word doesn't match, delete children (none at depth 5), return upward
        if (!matches) {
            // make parent delete this node if it's a leaf
            node->children.clear();
        }
        return;
    }

    // recursive case: go through children
    for (auto it = node->children.begin(); it != node->children.end();) {
        char letter = it->first;
        TrieNode* child = it->second;

        filterRecursively(child, currentWord + letter, guess, pattern);

        if (child->children.empty()) {
            // check if this leaf word is valid with respect to guess + pattern
            std::string childWord = currentWord + letter;
            bool valid = true;
            for (int i = 0; i < 5; ++i) {
                char g_char = guess[i];
                char p_char = pattern[i];
                char w_char = childWord[i];

                if (p_char == 'g' && w_char != g_char) {
                    valid = false;
                    break;
                }
                if (p_char == 'y' && (w_char == g_char || childWord.find(g_char) == std::string::npos)) {
                    valid = false;
                    break;
                }
                if (p_char == 'b' && childWord.find(g_char) != std::string::npos) {
                    valid = false;
                    break;
                }
            }

            if (!valid) {
                delete child;
                it = node->children.erase(it);
                continue;
            }
        }
        ++it;
    }
}

void Trie::print() {
    // prints all words in trie

    std::list<std::string> words = getAllWords();
    for (const std::string& word : words) {
        std::cout << word << std::endl;
    }
}