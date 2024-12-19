#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream> 

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
    SDL_Color bgColor; // New background color for the scene
    std::string imagePath; // Path to the image to be displayed
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

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
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

        font = TTF_OpenFont("../fonts/Avenir.ttc", 24);
        if (!font) {
            std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
            return false;
        }

        loadScenes();
        return true;
    }

    void loadScenes() {
        scenes.push_back({0, "You wake up in a dark forest. What do you do?", { {"Look around", 1}, {"Go back to sleep", 2} }, {0, 0, 0, 255}, "../images/forest.png"});
        scenes.push_back({1, "You see a path leading north and a river to the south. What do you do?", { {"Follow the path", 3}, {"Go to the river", 4} }, {34, 139, 34, 255}, "../images/path.png"});
        scenes.push_back({2, "You fall into a deep sleep and never wake up. The end.", {}, {128, 0, 128, 255}, "../images/bed.png"});
    }

    void handleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1 && scenes[currentSceneID].choices.size() > 0) {
                    renderBlackScreenWithDelay(700);
                    currentSceneID = scenes[currentSceneID].choices[0].nextSceneID;
                } else if (event.key.keysym.sym == SDLK_2 && scenes[currentSceneID].choices.size() > 1) {
                    renderBlackScreenWithDelay(700);
                    currentSceneID = scenes[currentSceneID].choices[1].nextSceneID;
                }
            }
        }
    }

    void renderBlackScreenWithDelay(int ms) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(ms);
    }

    void renderTextInBox(const std::string& text, int x, int y, int width, int height) {
        SDL_Color color = {255, 255, 255, 255};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_Rect textBox = {x, y, width, height};
        SDL_RenderFillRect(renderer, &textBox);
        renderText(text, x + 10, y + 10, width - 20);
    }

    void renderText(const std::string& text, int x, int y, int lineWidth) {
        std::istringstream stream(text);
        std::string line;
        int yOffset = y;

        while (std::getline(stream, line)) {
            std::istringstream wordStream(line);
            std::string currentLine;
            std::string word;

            while (wordStream >> word) {
                std::string testLine = currentLine + (currentLine.empty() ? "" : " ") + word;
                int textWidth, textHeight;
                TTF_SizeText(font, testLine.c_str(), &textWidth, &textHeight);
                if (textWidth > lineWidth) {
                    renderTextLine(currentLine, x, yOffset);
                    yOffset += textHeight + 10; 
                    currentLine = word; 
                } else {
                    currentLine = testLine; 
                }
            }

            if (!currentLine.empty()) {
                int textWidth, textHeight;
                TTF_SizeText(font, currentLine.c_str(), &textWidth, &textHeight);
                renderTextLine(currentLine, x, yOffset);
                yOffset += textHeight + 10;
            }
        }
    }


    void renderTextLine(const std::string& text, int x, int y) {
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), {255, 255, 255, 255});
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect dstRect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    void renderImage(const std::string& imagePath) {
        SDL_Surface* image = IMG_Load(imagePath.c_str());
        if (!image) {
            std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
            return;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_FreeSurface(image);
        SDL_DestroyTexture(texture);
    }

    void render() {
        Scene& currentScene = scenes[currentSceneID];
        SDL_SetRenderDrawColor(renderer, currentScene.bgColor.r, currentScene.bgColor.g, currentScene.bgColor.b, currentScene.bgColor.a);
        SDL_RenderClear(renderer);
        renderImage(currentScene.imagePath);
        std::string textBox = currentScene.dialogue;
        for (size_t i = 0; i < currentScene.choices.size(); ++i) {
            textBox += "\n" + std::to_string(i + 1) + ". " + currentScene.choices[i].text;
        }
        renderTextInBox(textBox, 50, 400, 700, 180);

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
            SDL_Delay(100);
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
