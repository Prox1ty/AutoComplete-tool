#define SDL_MAIN_EXPORTED
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "imgui_impl_sdl3.h"
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include "Structure.h"
#include "Renderer.h"
#include "vector"

Renderer::Renderer(Trie& trieRef)
	: trie(trieRef)
{}

bool Renderer::init() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Autocomplete Tool", 800, 600, SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		SDL_Log("Error: %s", SDL_GetError());
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	return true;
}

void Renderer::run() {
	bool running = true;
	std::vector<std::string> suggestions;

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Autocomplete Tool");

		if (ImGui::InputText("Type Here", inputBuffer, IM_ARRAYSIZE(inputBuffer))) {
			suggestions = getSuggestions(inputBuffer);
			if (!suggestions.empty()) {
				ImGui::OpenPopup("##Suggestions");
			}
			else {
				ImGui::CloseCurrentPopup();
			}
		}

		if (ImGui::BeginPopup("##Suggestions")) {
			for (const auto& s : suggestions) {
				if (ImGui::Selectable(s.c_str())) {
					strncpy_s(inputBuffer, sizeof(inputBuffer), s.c_str(), _TRUNCATE);
					suggestions.clear();
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
			
		}

		ImGui::End();

		ImGui::Render();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_RenderPresent(renderer);

		openPopup = false;
	}
}

void Renderer::shutdown() {
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

std::vector<std::string> Renderer::getSuggestions(const std::string& input) {
	std::vector<std::string> suggestions;
	if (input.empty()) {
		return suggestions;
	}
	suggestions = trie.autocomplete(input);
	return suggestions;
}
