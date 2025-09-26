#pragma once
#include <SDL3/SDL.h>
#include "Structure.h"
#include <string>

class Renderer {
public:
	Renderer(Trie& trieRef);
	bool init();
	void run();
	void shutdown();

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	char inputBuffer[256] = "";
	bool openPopup = false;

	Trie& trie;

	std::vector<std::string> getSuggestions(const std::string& input);
};
