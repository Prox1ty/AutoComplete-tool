#include <iostream>
#include "Structure.cpp"

int main() {
	Trie trie;
	trie.insertWord("banana");
	trie.insertWord("ban");
	trie.insertWord("bandana");

	std::cout << trie.doesContain("nababa") << std::endl;
	std::vector<std::string> buffer = trie.autocomplete("ban");
	for (const auto& child : buffer) {
		std::cout << child << std::endl;
	}
}