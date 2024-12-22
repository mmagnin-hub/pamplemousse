#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
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
    SDL_Color bgColor; // Background color for the scene
    std::string imagePath; // Path to the image to be displayed
};

// Struct for a chapter
struct Chapter {
    std::string title;
    std::vector<Scene> scenes;
    std::string themeMusicPath; // Path to the theme music for this chapter
};

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool isRunning;
    std::vector<Scene> scenes;
    std::vector<Chapter> chapters;
    int currentSceneID;
    int currentChapterID;
    Mix_Music* currentMusic;

public:
    Game() : window(nullptr), renderer(nullptr), font(nullptr), isRunning(true), currentSceneID(0), currentChapterID(0), currentMusic(nullptr) {}

    bool init(const char* title, int width, int height) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
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

        loadChapters();
        displayChapterSelectionMenu();
        return true;
    }

    void loadChapters() {
        Chapter poulpe;
        poulpe.title = "Poulpe";
        poulpe.scenes.push_back({0, "La vie parisienne enveloppe Juliette, jeune étudiante en école de mode, dans son petit appartement sous les toits.", {{"Suite ...", 1}}, {0, 0, 0, 255}, "../images/Poulpe/ballade_paris_0.png"});
        poulpe.scenes.push_back({1, "Aujourd'hui, la solitude lui pèse, et l’air de Paris semble l’appeler.", {{"Suite ...", 2}}, {0, 0, 0, 255}, "../images/Poulpe/ballade_paris_0.png"});
        poulpe.scenes.push_back({2, "Elle enfile son manteau et descend les escaliers grinçants, décidée à se perdre dans les rues.", {{"Suite ...", 3}}, {0, 0, 0, 255}, "../images/Poulpe/ballade_paris_0.png"});
        poulpe.scenes.push_back({3, "Elle croise son reflet dans une vitrine. Ses cheveux tombent comme un rideau fatigué autour de son visage.", {{"Suite ...", 4}}, {0, 0, 0, 255}, "../images/Poulpe/coiffure.png"});
        poulpe.scenes.push_back({4, "Pourquoi pas un changement ?, pensa-t-elle.", {{"Ça me remettra les idées en place.", 7}, {"Je n'ai pas l'énergie.", 5}}, {0, 0, 0, 255}, "../images/Poulpe/coiffure.png"});
        poulpe.scenes.push_back({5, "Vraiment pas ? Tu es sûre ?", {{"Bon d'accord.", 7}, {"NON.", 6}}, {0, 0, 0, 255}, "../images/Poulpe/coiffure_1.png"});
        poulpe.scenes.push_back({6, "NON ??? Comment ça non ? Il FAUT changer ça tout de suite!", {{"Bon d'accord.", 7}}, {0, 0, 0, 255}, "../images/Poulpe/coiffure_2.png"});
        poulpe.scenes.push_back({7, "Chez la coiffeuse, Juliette ose une nouvelle frange. En sortant, une brise caresse son visage, et elle se sent légère, comme si quelque chose avait changé en elle.", {{"Suite ...", 8}}, {0, 0, 0, 255}, "../images/Poulpe/ballade_paris_1.png"});
        poulpe.scenes.push_back({8, "Une fresque attire son regard : un poulpe majestueux peint sur un vieux mur décrépit. Ses tentacules semblent l’appeler, et une étrange sensation naît en elle.", {{"Suite ...", 9}}, {0, 0, 0, 255}, "../images/Poulpe/rencontre.png"});
        poulpe.scenes.push_back({9, "Pourquoi cette peinture la touche-t-elle autant ? Il faut que je le retrouve… murmure-t-elle.", {{"Suite ...", 10}}, {0, 0, 0, 255}, "../images/Poulpe/rencontre.png"});
        poulpe.scenes.push_back({10, "Mais demain, elle part à l’île de Ré. Les billets de train sont déjà prêts.", {{"Continuer vers le poulpe ?", 11}, {"Aller se reposer avant le voyage ?", 14}}, {0, 0, 0, 255}, "../images/Poulpe/rencontre.png"});
        poulpe.scenes.push_back({11, "En suivant la direction du poulpe, elle rencontre Mathis, un garçon au sourire franc.", {{"Suite ...", 12}}, {0, 0, 0, 255}, "../images/Poulpe/mathis_0.png"});
        poulpe.scenes.push_back({12, "Ils parlent, rient, cherchent le poulpe ensemble, mais sans succès.", {{"Suite ...", 13}}, {0, 0, 0, 255}, "../images/Poulpe/mathis_0.png"});
        poulpe.scenes.push_back({13, "Viens avec moi à l’île de Ré, lui propose-t-elle, presque sur un coup de tête. Il accepte.", {{"Suite ...", 16}}, {0, 0, 0, 255}, "../images/Poulpe/mathis_0.png"});
        poulpe.scenes.push_back({14, "Le lendemain, fatiguée du long voyage, elle arrive à l’île : Le vent salé de l’océan l’accueille, mais ses pas sont lourds.", {{"Suite ...", 15}}, {0, 0, 0, 255}, "../images/Poulpe/arrivee_ile.png"});
        poulpe.scenes.push_back({15, "Elle s’endort rapidement, rêvant de poulpes et de mystères.", {{"Suite ...", 18}}, {0, 0, 0, 255}, "../images/Poulpe/arrivee_ile.png"});
        poulpe.scenes.push_back({16, "Le lendemain, fatiguée du long voyage, ils arrivent à l’île : Le vent salé de l’océan les accueille, mais leurs pas sont lourds.", {{"Suite ...", 17}}, {0, 0, 0, 255}, "../images/Poulpe/arrivee_ile.png"});
        poulpe.scenes.push_back({17, "Elle s’endort rapidement, rêvant de poulpes et de mystères. Avec une pensée pour Mathis qui dort dans la chambre d'à côté.", {{"Suite ...", 19}}, {0, 0, 0, 255}, "../images/Poulpe/arrivee_ile.png"});
        poulpe.scenes.push_back({18, "Au matin, seule, Juliette construit un pont de sable. Malgré sa joie, son esprit est ailleurs.", {{"Suite ...", 20}}, {0, 0, 0, 255}, "../images/Poulpe/pont_juliette.png"});
        poulpe.scenes.push_back({19, "Au matin, avec Mathis, ils bâtissent ensemble un pont de sable, leurs mains s’effleurant dans les grains dorés.", {{"Suite ...", 21}}, {0, 0, 0, 255}, "../images/Poulpe/pont_mathis.png"});
        poulpe.scenes.push_back({20, "Elle enfourche un vélo et se perd dans les marais en quête du poulpe.", {{"Suite ...", 23}}, {0, 0, 0, 255}, "../images/Poulpe/velo.png"});
        poulpe.scenes.push_back({21, "Avec Mathis, ils pédalent côte à côte, leurs rires se mêlant au vent marin.", {{"Suite ...", 22}}, {0, 0, 0, 255}, "../images/Poulpe/velo.png"});
        poulpe.scenes.push_back({22, "Ils cherchent toujours ce poulpe, mais rien. Une certaine désillusion commence à apparaître.", {{"Suite ...", 25}}, {0, 0, 0, 255}, "../images/Poulpe/velo.png"});
        poulpe.scenes.push_back({23, "Sous le sable, elle découvre un garçon enterré. Elle creuse pour le libérer. Il s'appelle Mathis et était en train de rêver d'un poulpe.", {{"Suite ...", 24}}, {0, 0, 0, 255}, "../images/Poulpe/mathis_enterre.png"});
        poulpe.scenes.push_back({24, "Lorsqu'il se réveilla il était enterré. Bizarre...", {{"Suite ...", 26}}, {0, 0, 0, 255}, "../images/Poulpe/mathis_enterre.png"});
        poulpe.scenes.push_back({25, "Elle enfouit Mathis sous le sable pour plaisanter, leurs éclats de rire résonnant dans l’air chaud.", {{"Suite ...", 26}}, {0, 0, 0, 255}, "../images/Poulpe/mathis_enterre.png"});
        poulpe.scenes.push_back({26, "Mathis la regarde, ses yeux brillants d’une lueur tendre. Juliette sent son cœur battre plus fort.", {{"Suite ...", 27}}, {0, 0, 0, 255}, "../images/Poulpe/seduction.png"});
        poulpe.scenes.push_back({27, "Dois-je l’embrasser ? hésite-t-elle.", {{"Oui !", 28}, {"C'est peut-être trop tôt.", 29}}, {0, 0, 0, 255}, "../images/Poulpe/seduction.png"});
        poulpe.scenes.push_back({28, "Leur baiser est doux, infini, et le temps semble s’arrêter.", {{"Suite ...", 30}}, {0, 0, 0, 255}, "../images/Poulpe/bisou.png"});
        poulpe.scenes.push_back({29, "Mathis s’approche ... Et si on s’embrassait ? proposa-t-il.", {{"Suite ...", 28}}, {0, 0, 0, 255}, "../images/Poulpe/seduction.png"});
        poulpe.scenes.push_back({30, "De retour dans la ville lumière, ils arpentent les rues main dans la main, toujours à la recherche du poulpe.", {{"Suite ...", 31}}, {0, 0, 0, 255}, "../images/Poulpe/retour_paris.png"});
        poulpe.scenes.push_back({31, "Et puis… là-bas, dans l’ombre, ils aperçoivent enfin une forme familière.", {{"Suite ...", 32}}, {0, 0, 0, 255}, "../images/Poulpe/retour_paris.png"});
        poulpe.scenes.push_back({32, "Le poulpe est sauvé et trouve refuge dans le lit de Juliette. À ses côtés, Mathis s’installe, son bras passé autour d’elle. Leurs cœurs battent à l’unisson.", {{"Retour à l'écran d'accueil.", 33}}, {0, 0, 0, 255}, "../images/Poulpe/fin.png"});
        poulpe.scenes.push_back({33, "", {}, {0, 0, 0, 255}, ""});
        poulpe.themeMusicPath = "../audio/poulpe_theme.mp3";
        chapters.push_back(poulpe);


        Chapter taupe;
        taupe.title = "Taupe";
        taupe.scenes.push_back({0, "Juliette est entourée de ses amis sur la terrasse. L'atmosphère est joyeuse, mais une tension sous-jacente flotte dans l'air. Une taupe est parmi eux.", {{"Suite ...", 1}}, {0, 0, 0, 255}, "../images/Taupe/terrasse.png"});
        taupe.scenes.push_back({1, "Léna : 'Une taupe, sérieusement ? C’est un peu extrême comme jeu, non ?'", {{"Suite ...", 2}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_discussion.png"});
        taupe.scenes.push_back({2, "Mathis : 'Mais c’est ça qui est fun ! Tout le monde peut bluffer, mais personne ne doit être éliminé trop tôt…'", {{"Suite ...", 3}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_discussion.png"});
        taupe.scenes.push_back({3, "Aurélien : 'C’est Juliette qui décidera. Si elle désigne un groupe sans la taupe, elle perd.'", {{"Suite ...", 4}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_discussion.png"});
        taupe.scenes.push_back({4, "Juliette observe ses amis, un mélange d’excitation et de nervosité.", {{"Observer les réactions", 5}, {"Poser des questions générales", 7}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_discussion.png"});
        taupe.scenes.push_back({5, "Juliette décide d’observer les réactions de ses amis. Romane rit nerveusement, Cyriel reste concentré sur son téléphone.'", {{"Suite ...", 6}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_observation.png"});
        taupe.scenes.push_back({6, "Waldemar plaisante en disant : 'Ça pourrait être moi, mais bon, je suis un piètre menteur.'", {{"Suite ...", 10}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_observation.png"});
        taupe.scenes.push_back({7, "Juliette pose une question ouverte : 'Si vous deviez choisir quelqu’un comme taupe, qui serait-ce ?'", {{"Suite ...", 8}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_questions.png"});
        taupe.scenes.push_back({8, "Les réactions varient : Romane : 'Je ne sais pas, peut-être quelqu’un de très discret ?' Cyriel reste silencieux, tandis que Waldemar sourit subtilement. Cela intrigue Juliette.", {{"Suite ...", 9}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_questions.png"});
        taupe.scenes.push_back({9, "Cyriel reste silencieux, tandis que Waldemar sourit subtilement. Cela intrigue Juliette.", {{"Suite ...", 10}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_questions.png"});
        taupe.scenes.push_back({10, "Juliette doit choisir un groupe pour observer plus attentivement. Qui choisir ?", {{"Léna, Romane, Waldemar", 11}, {"Mathis, Cyriel, Timothée", 13}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_choix.png"});
        taupe.scenes.push_back({11, "Juliette observe Léna, Romane, et Waldemar. Léna mentionne un projet de design urgent, ...", {{"Suite ...", 12}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_groupe1.png"});
        taupe.scenes.push_back({12, "... Romane change de sujet en parlant de cuisine, et Waldemar consulte discrètement son téléphone. Cela semble étrange.", {{"Suite ...", 15}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_groupe1.png"});
        taupe.scenes.push_back({13, "Juliette observe Mathis, Cyriel, et Timothée. Mathis parle avec enthousiasme de leur prochaine sortie en vélo, ...", {{"Suite ...", 14}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_groupe2.png"});
        taupe.scenes.push_back({14, "... Timothée est plus silencieux que d’habitude, et Cyriel semble nerveux, jouant distraitement avec un objet.", {{"Suite ...", 15}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_groupe2.png"});
        taupe.scenes.push_back({15, "Juliette réfléchit aux indices qu’elle a collectés. Quelque chose semble hors de l’ordinaire, mais elle a besoin de plus d’informations pour avancer.", {{"Suite ...", 16}}, {0, 0, 0, 255}, "../images/Taupe/terrasse_recap.png"});
        taupe.scenes.push_back({16, "Le lendemain matin, les amis se retrouvent pour un petit-déjeuner sur la terrasse. L'atmosphère est plus détendue, mais Juliette garde un œil attentif sur chacun.", {{"Suite ...", 17}}, {0, 0, 0, 255}, "../images/Taupe/petit_dejeuner.png"});
        taupe.scenes.push_back({17, "Bénédicte : 'Ce jeu me stresse un peu. Je préfère rester dans les coulisses, mais je peux t’aider si tu veux.'", {{"Suite ...", 18}}, {0, 0, 0, 255}, "../images/Taupe/benedicte_offre.png"});
        taupe.scenes.push_back({18, "Son offre semble sincère, mais Juliette hésite.", {{"Accepter l’aide de Bénédicte", 19}, {"Refuser poliment", 20}}, {0, 0, 0, 255}, "../images/Taupe/benedicte_offre.png"});
        taupe.scenes.push_back({19, "Bénédicte : 'Bon, on va observer les autres ensemble.' Elle note que Waldemar semble préoccupé et que Timothée évite les regards directs. Cela intrigue Juliette.", {{"Suite ...", 21}}, {0, 0, 0, 255}, "../images/Taupe/benedicte_observation.png"});
        taupe.scenes.push_back({20, "Juliette : 'Merci, mais je préfère faire ça seule.' Elle observe discrètement, remarquant que Cyriel évite toujours de se mêler à la conversation.", {{"Suite ...", 21}}, {0, 0, 0, 255}, "../images/Taupe/juliette_observe.png"});
        taupe.scenes.push_back({21, "Les amis décident de se diviser pour des activités. Juliette choisit une activité pour se rapprocher de certains amis.", {{"Aller à la plage des massages", 22}, {"Participer à un jeu de société", 23}}, {0, 0, 0, 255}, "../images/Taupe/activites.png"});
        taupe.scenes.push_back({22, "À la plage des massages, Mathis et Waldemar discutent calmement. Waldemar évoque un projet d’architecture, mais il semble distrait. Juliette note son comportement.", {{"Suite ...", 24}}, {0, 0, 0, 255}, "../images/Taupe/plage_massages.png"});
        taupe.scenes.push_back({23, "Lors du jeu de société, Cyriel et Timothée jouent en duo. Cyriel se montre inhabituellement compétitif, mais Timothée reste en retrait.", {{"Suite ...", 24}}, {0, 0, 0, 255}, "../images/Taupe/jeux_societe.png"});
        taupe.scenes.push_back({24, "Juliette rassemble ses pensées. Les comportements de Waldemar, Cyriel et Timothée restent suspects. Elle sait que son prochain choix sera crucial.", {{"Suite ...", 25}}, {0, 0, 0, 255}, "../images/Taupe/recap_jour2.png"});
        taupe.scenes.push_back({25, "Juliette décide de commencer la journée en explorant la vieille ville avec ses amis. L’ambiance est détendue, mais elle garde l'œil ouvert pour des indices.", {{"Suite ...", 26}}, {0, 0, 0, 255}, "../images/Taupe/vieille_ville.png"});
        taupe.scenes.push_back({26, "Léna : 'Tu as une idée de qui pourrait être la taupe ?' Aurélien : 'Ce jeu est une excellente métaphore politique. La suspicion et les alliances se forment naturellement.'", {{"Suite ...", 27}}, {0, 0, 0, 255}, "../images/Taupe/discussion_vieille_ville.png"});
        taupe.scenes.push_back({27, "Juliette trouve son analyse intéressante mais se concentre sur les comportements. Elle voit que Benoît n'est pas très joueur.", {{"Suite ...", 28}}, {0, 0, 0, 255}, "../images/Taupe/discussion_vieille_ville.png"});
        taupe.scenes.push_back({28, "Les amis se séparent à nouveau pour des activités. Juliette peut choisir où aller pour interagir avec d’autres groupes.", {{"Rejoindre Mathis et Bénédicte à la plage des massages", 29}, {"Aller chiller à la cuisine avec Romane et Timothée", 30}}, {0, 0, 0, 255}, "../images/Taupe/choix_activites.png"});
        taupe.scenes.push_back({29, "À la plage des massages, Mathis est détendu, plaisantant sur le jeu, tandis que Bénédicte semble préoccupé, répondant par des phrases courtes.", {{"Suite ...", 31}}, {0, 0, 0, 255}, "../images/Taupe/plage_massages.png"});
        taupe.scenes.push_back({30, "En cuisine, Romane partage des anecdotes amusantes, tandis que Timothée semble distraite, regardant fréquemment son téléphone.", {{"Suite ...", 31}}, {0, 0, 0, 255}, "../images/Taupe/cuisine.png"});
        taupe.scenes.push_back({31, "De retour dans le salon, Juliette réfléchit aux comportements observés. Des détails intrigants commencent à se connecter.", {{"Suite ...", 32}}, {0, 0, 0, 255}, "../images/Taupe/salon_reflexion.png"});
        taupe.scenes.push_back({32, "Waldemar : 'Ce jeu commence à devenir sérieux. Tu vas devoir faire un choix bientôt.' Son ton semble détaché, mais son sourire en coin met Juliette mal à l’aise.", {{"Suite ...", 33}}, {0, 0, 0, 255}, "../images/Taupe/discussion_salon.png"});
        taupe.scenes.push_back({33, "Juliette sait qu'elle doit éliminer un groupe pour avancer. Dans quel groupe est la taupe ?", {{"Léna, Romane, Aurélien, Waldemar, Benoît", 34}, {"Mathis, Cyriel, Bénédicte, Timothée", 35}}, {0, 0, 0, 255}, "../images/Taupe/salon_choix.png"});
        taupe.scenes.push_back({34, "Juliette désigne Léna, Romane, Aurélien, Waldemar et Benoît. Mais la taupe était dans l’autre groupe ! La partie est terminée.", {{"Retour à l'écran d'accueil.", 74}}, {0, 0, 0, 255}, "../images/Taupe/defaite.png"});
        taupe.scenes.push_back({35, "Juliette désigne Mathis, Cyriel, Bénédicte et Timothée. Un soulagement palpable traverse l’équipe. Elle est encore en jeu, mais la taupe reste à découvrir.", {{"Suite ...", 36}}, {0, 0, 0, 255}, "../images/Taupe/succes.png"});
        taupe.scenes.push_back({36, "Juliette commence à analyser les comportements dans le groupe restant. Elle sait qu’elle doit se montrer stratégique pour repérer la taupe.", {{"Suite ...", 37}}, {0, 0, 0, 255}, "../images/Taupe/analyse_groupe.png"});
        taupe.scenes.push_back({37, "Les amis décident de jouer à un jeu de rôles pour détendre l’atmosphère. Chaque personne doit incarner un personnage, et Juliette observe attentivement.", {{"Suite ...", 38}}, {0, 0, 0, 255}, "../images/Taupe/jeu_roles.png"});
        taupe.scenes.push_back({38, "Mathis joue un détective, tandis que Cyriel semble mal à l’aise dans son rôle. Timothée est inhabituellement enjoué, et Bénédicte reste silencieuse.", {{"Suite ...", 39}}, {0, 0, 0, 255}, "../images/Taupe/jeu_roles_observation.png"});
        taupe.scenes.push_back({39, "Juliette décide de poser une question déstabilisante : 'Si vous étiez la taupe, quelle serait votre stratégie ?'", {{"Suite ...", 40}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe.png"});
        taupe.scenes.push_back({40, "Les réponses varient : Mathis plaisante et élabore une stratégie farfelue, Cyriel hésite avant de répondre, ...", {{"Suite ...", 41}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({41, "... Bénédicte dit qu’elle essaierait de se fondre dans le décor, et Timothée déclare qu’il jouerait le rôle de l’accusateur pour détourner les soupçons.", {{"Suite ...", 42}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({42, "Juliette réfléchit aux réponses. Et remarque une certaine similitude avec leurs comportements précédents.", {{"Suite ...", 43}}, {0, 0, 0, 255}, "../images/Taupe/choix_elimination.png"});
        taupe.scenes.push_back({43, "Le lendemain matin, l'ambiance est à nouveau détendue. Celles et ceux qui sont éliminés font des remarques comiques après être allés à la pêche aux informations.", {{"Suite ...", 44}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({44, "Romane : 'Moi ... je sais qui c'est la taupe, hi hi.'", {{"Suite ...", 45}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({45, "Juliette répond agacée : 'Merci Ronron, c'est très constructif.' Puis propose d'aller faire un tour au marché. Tout le monde accepte.", {{"Suite ...", 46}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({46, "Une fois au marché, les quatre suspects restants se séparent en deux groupes. Lesquels Juliette va-t-elle suivre ?", {{"Bénédicte et Mathis", 47}, {"Cyriel et Timothée", 51}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({47, "Bénédicte chuchote un mot à l'oreille de Mathis, puis il rigole. Lorsque tu l'interpelles, il évite le sujet d'un air léger.", {{"Suite ...", 48}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({48, "Vous finissez par acheter des perles. Puis Juliette laisse Bénédicte partir devant pour cuisiner Mathis.", {{"Insister sur les messes basses.", 49}, {"Extorquer des informations en échange d'un baiser.", 50}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({49, "Mathis reste de marbre, mais il te dit qu'il pourra te révéler cette surprise bientôt.", {{"Suite ...", 52}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({50, "Ça marche très bien, et il te dévoile que ce n'est pas lui la taupe.", {{"Suite ...", 52}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({51, "Ils se concentrent sur les stands de chapeaux. Juliette se lasse vite d'être avec ces deux guignols.", {{"Suite ...", 52}}, {0, 0, 0, 255}, "../images/Taupe/question_taupe_reponses.png"});
        taupe.scenes.push_back({52, "Le soir, les amis jouent à un jeu de cartes. Chacun essaie de cacher ses émotions, mais Juliette capte des indices subtils dans leurs comportements.", {{"Suite ...", 53}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({53, "Alors que le jeu avance, Juliette se sent prête à éliminer deux autres personnes. Qui, selon elle, ne sont pas la taupe.", {{"Mathis et Timothée", 54}, {"Bénédicte et Cyriel", 56}}, {0, 0, 0, 255}, "../images/Taupe/preparation_elimination.png"});
        taupe.scenes.push_back({54, "Mathis et Timothée regardent Juliette longuement. 'T'es sûre de ton choix ? Pourquoi nous avoir éliminés ?'", {{"Suite ...", 55}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({55, "Juliette a vu juste. La taupe est parmi Bénédicte et Cyriel.", {{"Suite ...", 57}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({56, "Juliette a éliminé la taupe. Elle a perdu.", {{"Retour à l'écran d'accueil.", 74}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({57, "Après ce choix décisif, la tension descend pour Juliette. Elle propose de passer le reste de la journée à la plage.", {{"Suite ...", 58}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({58, "Une fois au bord de mer, tout le monde se met d'accord pour sculpter une baleine dans le sable.", {{"Suite ...", 59}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({59, "À la plage, Juliette décide d'interroger les autres : Romane, Léna et Timothée. Ils sont tous d'accord mais à une condition.", {{"Suite ...", 60}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({60, "Juliette doit répondre correctement à une question de leur choix. Timothée commence.", {{"Suite ...", 61}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({61, "Timothée : 'Dans quel endroit s'est déroulée la première scène du séjour ?'", {{"Au marché", 62}, {"Sur une terrasse", 63}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({62, "Timothée : 'Non.'", {{"Suite ...", 65}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({63, "Timothée : 'Oui bravo, c'était sur une terrasse. Mon indice, c'est que la taupe a de beaux cheveux.'", {{"Suite ...", 64}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({64, "Léna pose la prochaine question : 'Quel âge a la sœur de Mathis ?'", {{"24", 66}, {"25", 65}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({65, "Léna : 'Non. Dommage.'", {{"Suite ...", 67}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({66, "Léna : 'Oui bravo, c'était 24 ans, le 30 novembre 2024. D'après moi, la taupe aime le Cenovis.'", {{"Suite ...", 67}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({67, "Romane pose la dernière question : 'Quelle est la dérivée de sin(x) ?'", {{"cos(x)", 69}, {"-cos(x)", 68}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({68, "Romane : 'Non. Je ne savais pas non plus.'", {{"Suite ...", 70}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({69, "Romane : 'Oui bravo, c'est juste. Alexandrou serait fier de toi.' Elle murmure : 'La taupe aime les bad boys.'", {{"Suite ...", 70}}, {0, 0, 0, 255}, "../images/Taupe/jeu_cartes.png"});
        taupe.scenes.push_back({70, "De retour à la maison, Juliette réfléchit à tous les indices accumulés pour prendre sa décision. Qui est la taupe ?", {{"Bénédicte", 71}, {"Cyriel", 75}}, {0, 0, 0, 255}, "../images/Taupe/decision_finale.png"});
        taupe.scenes.push_back({71, "Bénédicte avoue finalement qu'elle est la taupe. Juliette a gagné !", {{"Fin.", 73}}, {0, 0, 0, 255}, "../images/Taupe/victoire.png"});
        taupe.scenes.push_back({72, "Juliette accuse Cyriel, mais il nie avec véhémence. Bénédicte révèle alors qu'elle est la taupe. Juliette a perdu !", {{"Retour à l'écran d'accueil.", 74}}, {0, 0, 0, 255}, "../images/Taupe/defaite.png"});
        taupe.scenes.push_back({73, "Bravo mon amour, t'es trop forte. J'espère que cette expérience t'aura plu. <3", {{"Retour à l'écran d'accueil.", 74}}, {0, 0, 0, 255}, "../images/Taupe/victoire.png"});
        taupe.scenes.push_back({74, "", {{}}, {}, ""});
        taupe.themeMusicPath = "../audio/poulpe_theme.mp3"; 
        chapters.push_back(taupe);
        
        scenes = chapters[currentChapterID].scenes;
    }

    void playChapterMusic() {
        if (currentMusic) {
            Mix_HaltMusic();
            Mix_FreeMusic(currentMusic);
        }

        std::string musicPath = chapters[currentChapterID].themeMusicPath;
        currentMusic = Mix_LoadMUS(musicPath.c_str());
        if (!currentMusic) {
            std::cerr << "Failed to load music! Mix_Error: " << Mix_GetError() << std::endl;
        } else {
            Mix_PlayMusic(currentMusic, -1); // Play music in a loop
        }
    }

    void handleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1 && scenes[currentSceneID].choices.size() > 0) {
                    renderBlackScreenWithDelay(500);
                    currentSceneID = scenes[currentSceneID].choices[0].nextSceneID;
                } else if (event.key.keysym.sym == SDLK_2 && scenes[currentSceneID].choices.size() > 1) {
                    renderBlackScreenWithDelay(500);
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
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), {255, 255, 255, 255});
        if (!surface) {
            std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
            return;
        }
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

        int winW, winH, imgW, imgH;
        SDL_GetWindowSize(window, &winW, &winH);
        imgW = image->w;
        imgH = image->h;

        float aspectRatio = static_cast<float>(imgW) / imgH;
        SDL_Rect dstRect;
        if (imgW > imgH) {
            dstRect.w = winW;
            dstRect.h = static_cast<int>(winW / aspectRatio);
        } else {
            dstRect.h = winH;
            dstRect.w = static_cast<int>(winH * aspectRatio);
        }
        dstRect.x = (winW - dstRect.w) / 2;
        dstRect.y = (winH - dstRect.h) / 2;

        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_FreeSurface(image);
        SDL_DestroyTexture(texture);
    }

    void render() {
        if (currentChapterID == -1) {
            // Render Welcome Screen
            renderWelcomeScreen();
            return;
        }
        else if (currentSceneID < scenes.size() - 1) {
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
            return;
        }
        currentChapterID = -1;
        currentSceneID = 0;
        displayChapterSelectionMenu();
        return;
    }

    void renderWelcomeScreen() {
        // Set the background color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render a festive message at the top
        renderText("JOYEUX NOEL", 200, 50, 400);

        // Render the game description
        renderText("Le jeux dont tu es l'héroïne.", 100, 100, 600); 

        // Render the welcome message
        renderText("Welcome! Select a Chapter:", 100, 150, 600);

        // Render the list of chapters
        for (int i = 0; i < chapters.size(); ++i) {
            renderText(std::to_string(i + 1) + ". " + chapters[i].title, 100, 200 + (i * 40), 600); 
        }

        // Render instructions for the player
        renderText("Appuie une fois sur le nombre qui correspond à ton choix.", 100, 200 + (chapters.size() * 40) + 20, 600); 

        // Render a heartfelt message at the bottom
        renderText("Je t'aime <3", 300, 400, 200);

        // Present the rendered screen
        SDL_RenderPresent(renderer);
    }


    void displayChapterSelectionMenu() {
        bool selecting = true;
        while (selecting) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    isRunning = false;
                    selecting = false;
                } else if (event.type == SDL_KEYDOWN) {
                    int selectedChapter = -1;
                    if (event.key.keysym.sym == SDLK_1 && chapters.size() > 0) {
                        selectedChapter = 0;
                    } else if (event.key.keysym.sym == SDLK_2 && chapters.size() > 1) {
                        selectedChapter = 1;
                    }

                    if (selectedChapter != -1) {
                        currentChapterID = selectedChapter;
                        scenes = chapters[currentChapterID].scenes;
                        playChapterMusic();
                        selecting = false;
                    }
                }
            }
            renderWelcomeScreen();
        }
    }

    void startChapter(int chapterIndex) {
        currentChapterID = chapterIndex;
        scenes = chapters[currentChapterID].scenes;
        currentSceneID = 0;
        playChapterMusic();
    }

    void clean() {
        if (currentMusic) Mix_FreeMusic(currentMusic); // Free the music
        Mix_CloseAudio();
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
        return -1;
    }

    game.run();
    game.clean();
    return 0;
}