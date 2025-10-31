Wordle Backend (C++)

This project implements a backend for a Wordle-style game using a trie data structure for efficient word storage and lookup.

Features
- Fast word validation: Check if a guessed word exists in the dictionary in O(word length) time.
- Memory-efficient: Shares common prefixes among words, reducing storage needs.
- Trie-based logic: Traverses each guess letter by letter to validate words quickly.

Implementation
- Written in C++ for performance.
- Uses a custom trie class to store a dictionary of valid words.
