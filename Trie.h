#ifndef ASSIGNMENT_4_TRIE_H
#define ASSIGNMENT_4_TRIE_H

#include <string>
#include <unordered_map>
#include<list>
#include <iostream>

using namespace std;

class TrieNode {
public:
    //children of a current node. There can be up to 26 children in accordance with 26 letters in English alphabet.
    //unordered_map has keys (letters) and values (TrieNode pointers)
    //if the letter is the end of the word then it does not have children
    unordered_map<char, TrieNode*> children;
    //constructor of a TrieNode.
    TrieNode();
};

class Trie {
private:
    // It is not a beginning of any word, but all its children are the beginnings of words
    TrieNode* root;
    //friendly class for testing
    friend class TrieTest;

    //helper functions
    //deletes a node and all its children
    void deleteTrie(TrieNode* node);

    //counts how many leaves are there from a current node
    unsigned int countLeaves(TrieNode* node) const;
    // filterRecursively function should recursively visit children nodes and delete them if they do not satisfy filtering criteria
    // note that for checking y 'yellow' constraint you have to see the whole word and delete it from trie if needed.
    // note is non-leaf node does not have children it must be deleted because the path is invalid and does not represent any word
    void filterRecursively(TrieNode* node, std::string currentWord, const string& guess, const string& pattern);

    //getWordsRecursively function builds strings of the words from the Trie object
    void getWordsRecursively(TrieNode* node, std::string currentWord, std::list<std::string>& words) const;
public:
    //constructor of a Trie
    Trie();

    //destructor of a Trie
    ~Trie();

    //function returns the number of words in a trie.
    // Since all words have the length of 5, the size is equal to the number of leaves (nodes without children)
    unsigned  int size() const;

    //inserts a given word to the trie
    //returns true if successful
    //returns false if word is already in the trie
    //returns false if length of word is not 5
    bool insert(const string& word);

    //function calls getWordsRecursively and returns all words kept in the trie
    list<string> getAllWords() const;

    //function filter filters trie.
    //guess is a word and pattern are colors of letters consisting of 3 possible letters: y - yellow, b - black , g-green
    // y - the letter is in the word but at another position
    // g - the letter is in the word at current position
    //b - the letter is not in the word
    bool filter(const string& guess, const string& pattern);

    //return first word from the trie
    //hint: just always keep taking the first element from the children list
    // if no words are in trie, return empty string
    string getFirstWord();

    //helper function that prints a trie
    void print();
};
#endif
