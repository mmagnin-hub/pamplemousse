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
        scenes.push_back({0, "La vie parisienne enveloppe Juliette, jeune étudiante en école de mode, dans son petit appartement sous les toits.", { {"Suite ...", 1} }, {0, 0, 0, 255}, "../images/Poulpe/ballade_paris_0.png"});
        scenes.push_back({1, "Aujourd'hui, la solitude lui pèse, et l’air de Paris semble l’appeler.", { {"Suite ...", 2} }, {0, 0, 0, 255}, "../images/Poulpe/ballade_paris_0.png"});
        scenes.push_back({2, "Elle enfile son manteau et descend les escaliers grinçants, décidée à se perdre dans les rues.", { {"Suite ...", 3} }, {0, 0, 0, 255}, "../images/Poulpe/ballade_paris_0.png"});
        scenes.push_back({3, "Elle croise son reflet dans une vitrine. Ses cheveux tombent comme un rideau fatigué autour de son visage.", {{"Suite ...", 4}}, {0, 0, 0, 255}, "../images/Poulpe/coiffure.png"});
        scenes.push_back({4, "Pourquoi pas un changement ?, pensa-t-elle.", {{"Ça me remettra les idées en place.", 7}, {"Je n'ai pas l'énergie.", 5}}, {0, 0, 0, 255}, "../images/Poulpe/coiffure.png"});
        scenes.push_back({5, "Vraiment pas ? Tu es sûre ?", {{"Bon d'accord.", 7}, {"NON.", 6}}, {0, 0, 0, 255}, "../images/Poulpe/coiffure_1.png"});
        scenes.push_back({6, "NON ??? Comment  ça non ? Il FAUT changer ça tout de suite!", {{"Bon d'accord.", 7} }, {0, 0, 0, 255}, "../images/Poulpe/coiffure_2.png"});
        scenes.push_back({7, "Chez la coiffeuse, Juliette ose une nouvelle frange. En sortant, une brise caresse son visage, et elle se sent légère, comme si quelque chose avait changé en elle.", {{"Suite ...", 8} }, {0, 0, 0, 255}, "../images/Poulpe/ballade_paris_1.png"});
        scenes.push_back({8, "Une fresque attire son regard : un poulpe majestueux peint sur un vieux mur décrépit. Ses tentacules semblent l’appeler, et une étrange sensation naît en elle.", {{"Suite ...", 9} }, {0, 0, 0, 255}, "../images/Poulpe/rencontre.png"});
        scenes.push_back({9, "Pourquoi cette peinture la touche-t-elle autant ? Il faut que je le retrouve… murmure-t-elle.", {{"Suite ...", 10} }, {0, 0, 0, 255}, "../images/Poulpe/rencontre.png"});
        scenes.push_back({10, "Mais demain, elle part à l’île de Ré. Les billets de train sont déjà prêts.", {{"Continuer vers le poulpe ?", 11}, {"Aller se reposer avant le voyage ?", 14} }, {0, 0, 0, 255}, "../images/Poulpe/rencontre.png"});
        scenes.push_back({11, "En suivant la direction du poulpe, elle rencontre Mathis, un garçon au sourire franc.", {{"Suite ...", 12} }, {0, 0, 0, 255}, "../images/Poulpe/mathis_0.png"});
        scenes.push_back({12, "Ils parlent, rient, cherchent le poulpe ensemble, mais sans succès.", {{"Suite ...", 13} }, {0, 0, 0, 255}, "../images/Poulpe/mathis_0.png"});
        scenes.push_back({13, "Viens avec moi à l’île de Ré, lui propose-t-elle, presque sur un coup de tête. Il accepte.", {{"Suite ...", 16} }, {0, 0, 0, 255}, "../images/Poulpe/mathis_0.png"});
        scenes.push_back({14, "Le lendemain, fatiguée du long voyage, elle arrive à l’île : Le vent salé de l’océan l’accueille, mais ses pas sont lourds.", {{"Suite ...", 15} }, {0, 0, 0, 255}, "../images/Poulpe/arrivee_ile.png"});
        scenes.push_back({15, "Elle s’endort rapidement, rêvant de poulpes et de mystères.", {{"Suite ...", 18} }, {0, 0, 0, 255}, "../images/Poulpe/arrivee_ile.png"});
        scenes.push_back({16, "Le lendemain, fatiguée du long voyage, ils arrivent à l’île : Le vent salé de l’océan les accueilles, mais leurs pas sont lourds.", {{"Suite ...", 17} }, {0, 0, 0, 255}, "../images/Poulpe/arrivee_ile.png"});
        scenes.push_back({17, "Elle s’endort rapidement, rêvant de poulpes et de mystères. Avec une pensée pour Mathis qui dort dans la chambre d'à côté.", {{"Suite ...", 19} }, {0, 0, 0, 255}, "../images/Poulpe/arrivee_ile.png"});
        scenes.push_back({18, "Au matin, seule, Juliette construit un pont de sable. Malgrés sa joie, son esprit est ailleurs.", {{"Suite ...", 20} }, {0, 0, 0, 255}, "../images/Poulpe/pont_juliette.png"});
        scenes.push_back({19, "Au matin, avec Mathis, ils bâtissent ensemble un pont de sable, leurs mains s’effleurant dans les grains dorés.", {{"Suite ...", 21} }, {0, 0, 0, 255}, "../images/Poulpe/pont_mathis.png"});
        scenes.push_back({20, "Elle enfourche un vélo et se perd dans les marais en quête du poulpe.", {{"Suite ...", 23} }, {0, 0, 0, 255}, "../images/Poulpe/velo.png"});
        scenes.push_back({21, "Avec Mathis, ils pédalent côte à côte, leurs rires se mêlant au vent marin.", {{"Suite ...", 22} }, {0, 0, 0, 255}, "../images/Poulpe/velo.png"});
        scenes.push_back({22, "Ils cherchent toujours ce poulpe, mais rien. Une certaine désillusion commence à apparaître.", {{"Suite ...", 23} }, {0, 0, 0, 255}, "../images/Poulpe/velo.png"});
        scenes.push_back({23, "Sous le sable, elle découvre un garçon enterré. Elle creuse pour le libérer. Il s'appelle Mathis et était en train de rêver d'un poulpe.", {{"Suite ...", 24} }, {0, 0, 0, 255}, "../images/Poulpe/mathis_enterre.png"});
        scenes.push_back({24, "Lorsqu'il se réveilla il était enterré. Bizzare...", {{"Suite ...", 26} }, {0, 0, 0, 255}, "../images/Poulpe/mathis_enterre.png"});
        scenes.push_back({25, "Elle enfouit Mathis sous le sable pour plaisanter, leurs éclats de rire résonnant dans l’air chaud.", {{"Suite ...", 26} }, {0, 0, 0, 255}, "../images/Poulpe/mathis_enterre.png"});
        scenes.push_back({26, "Mathis la regarde, ses yeux brillants d’une lueur tendre. Juliette sent son cœur battre plus fort.", {{"Suite ...", 27} }, {0, 0, 0, 255}, "../images/Poulpe/seduction.png"});
        scenes.push_back({27, "Dois-je l’embrasser ? hésite-t-elle.", {{"Oui !", 28}, {"C'est peut-être trop tôt.",29} }, {0, 0, 0, 255}, "../images/Poulpe/seduction.png"});
        scenes.push_back({28, "Leur baiser est doux, infini, et le temps semble s’arrêter.", {{"Suite ...", 30} }, {0, 0, 0, 255}, "../images/Poulpe/bisou.png"});
        scenes.push_back({29, "Mathis s’approche ... Et si on s’embrassais ? proposa-t-il.", {{"Suite ...",28} }, {0, 0, 0, 255}, "../images/Poulpe/seduction.png"});
        scenes.push_back({30, "De retour dans la ville lumière, ils arpentent les rues main dans la main, toujours à la recherche du poulpe.", {{"Suite ...",31} }, {0, 0, 0, 255}, "../images/Poulpe/retour_paris.png"});
        scenes.push_back({31, "Et puis… là-bas, dans l’ombre, ils aperçoivent enfin une forme familière.", {{"Suite ...",32} }, {0, 0, 0, 255}, "../images/Poulpe/retour_paris.png"});
        scenes.push_back({32, "Le poulpe est sauvé et trouve refuge dans le lit de Juliette. À ses côtés, Mathis s’installe, son bras passé autour d’elle. Leurs cœurs battent à l’unisson, comme une promesse. Fin", {}, {0, 0, 0, 255}, "../images/Poulpe/fin.png"});
    }

    void handleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1 && scenes[currentSceneID].choices.size() > 0) {
                    //renderBlackScreenWithDelay(700);
                    currentSceneID = scenes[currentSceneID].choices[0].nextSceneID;
                } else if (event.key.keysym.sym == SDLK_2 && scenes[currentSceneID].choices.size() > 1) {
                    // renderBlackScreenWithDelay(700);
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
    if (!game.init("Pamplemousse", 800, 600)) {
        std::cerr << "Game failed to initialize." << std::endl;
        return -1;
    }
    game.run();
    game.clean();
    return 0;
}
