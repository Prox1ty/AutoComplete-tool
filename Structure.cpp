#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
class TrieNode {
	public:
		std::unordered_map<char, TrieNode*> children;
		bool isTerminal;

		TrieNode() {
			isTerminal = false;
		}
		// Deconstructor
		~TrieNode() {
			for (const auto& pair : children) {
				delete pair.second;
			}
		}
};

class Trie {
private:
	TrieNode* root;
	//DFS - Depth First Search
	void findAllWords(TrieNode* node,
		std::string prefix,
		std::vector<std::string>& results) {
		if (node->isTerminal) {
			results.push_back(prefix);
		}
		for (auto& child : node->children) {
			char character = child.first;
			TrieNode* next = child.second;

			findAllWords(next, prefix + character, results);
		}
	}

public:
	Trie() {
		root = new TrieNode();
	}
	// Deconstructor. Basically frees up memory allocated on the heap due to new()
	~Trie() {
		delete root;
	}

	void insertWord(std::string word) {
		TrieNode* current = root;
		for (const char& c : word) {
			if (current->children.find(c) == current->children.end()) {
				current->children[c] = new TrieNode();
			}
			current = current->children[c];
		}
		current->isTerminal = true;
	}

	std::string doesContain(std::string word) {
		std::vector<char> arr = {};
		TrieNode* current = root;
		for (const char& c : word) {
			if (current->children.find(c) == current->children.end()) {
				return "Not Found";
			}
			arr.push_back(c);
			current = current->children[c];
		}
		std::string str = "";
		for (const char& c : arr) {
			str += c;
		}
		return "Found " + str;
		
	}
	std::vector<std::string> autocomplete(std::string word) {
		TrieNode* current = root;
		std::vector<std::string> accumulator;
		std::string str = "";
		for (const char& c : word) {
			if (current->children.find(c) == current->children.end()) {
				return {};
			}
			str += c;
			current = current->children[c];
		}
		// At final char of prefix
		findAllWords(current, str, accumulator);
		return accumulator;
	}

};
