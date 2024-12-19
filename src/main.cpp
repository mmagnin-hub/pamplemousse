#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>

// Struct for a choice the player can make
struct Choice {
    std::string text;
    int nextSceneID; // The ID of the scene that follows this choice
};

// Struct for a scene that contains dialogue and choices
struct Scene {
    int id;
    std::string dialogue;
    std::vector<Choice> choices;
};

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool isRunning;
    std::vector<Scene> scenes;
    int currentSceneID;

public:
    Game() : window(nullptr), renderer(nullptr), font(nullptr), isRunning(true), currentSceneID(0) {}

    bool init(const char* title, int width, int height) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        if (TTF_Init() == -1) {
            std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
            return false;
        }

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            return false;
        }

        font = TTF_OpenFont("../fonts/Avenir.ttc", 24); // Change this to the path to your font file
        if (!font) {
            std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
            return false;
        }

        loadScenes();
        return true;
    }

    void loadScenes() {
        scenes.push_back({0, "You wake up in a dark forest. What do you do?", { {"Look around", 1}, {"Go back to sleep", 2} }});
        scenes.push_back({1, "You see a path leading north and a river to the south. What do you do?", { {"Follow the path", 3}, {"Go to the river", 4} }});
        scenes.push_back({2, "You fall into a deep sleep and never wake up. The end.", {}});
        scenes.push_back({3, "You reach a small village. The villagers greet you warmly.", {}});
        scenes.push_back({4, "The river is cold and fast. Do you try to swim across?", { {"Yes", 5}, {"No", 6} }});
        scenes.push_back({5, "You get swept away by the current. The end.", {}});
        scenes.push_back({6, "You stay on the shore and wait. Nothing happens.", {}});
    }

    void handleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1 && scenes[currentSceneID].choices.size() > 0) {
                    currentSceneID = scenes[currentSceneID].choices[0].nextSceneID;
                } else if (event.key.keysym.sym == SDLK_2 && scenes[currentSceneID].choices.size() > 1) {
                    currentSceneID = scenes[currentSceneID].choices[1].nextSceneID;
                }
            }
        }
    }

    void renderText(const std::string& text, int x, int y) {
        SDL_Color color = {255, 255, 255, 255}; // White color
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect dstRect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render current scene
        Scene& currentScene = scenes[currentSceneID];
        renderText(currentScene.dialogue, 50, 50);

        int yOffset = 100;
        for (size_t i = 0; i < currentScene.choices.size(); ++i) {
            std::string choiceText = std::to_string(i + 1) + ". " + currentScene.choices[i].text;
            renderText(choiceText, 50, 50 + yOffset);
            yOffset += 40;
        }

        SDL_RenderPresent(renderer);
    }

    void clean() {
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

    void run() {
        while (isRunning) {
            handleInput();
            render();
            SDL_Delay(100); // Simulate a frame delay
        }
    }
};

int main(int argc, char* argv[]) {
    Game game;
    if (!game.init("Text Adventure Game", 800, 600)) {
        std::cerr << "Game failed to initialize." << std::endl;
        return -1;
    }

    game.run();
    game.clean();
    
    return 0;
} 
