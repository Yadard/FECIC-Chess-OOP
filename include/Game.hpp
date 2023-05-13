#ifndef GAME_HPP
#define GAME_HPP

#include "AssetManager.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "Scenes/Scene.hpp"
#include <filesystem>
#include <memory>
#include <queue>

class Game {
  public:
    Game(sf::VideoMode t_render, const char *title);

    auto loadAssets() -> void;
    auto gameloop() -> void;

    auto quit() -> void;
    auto change_scene(Scene::IScene *new_scene) -> void;

  private:
    auto catchEvents() -> void;

    auto loadTextures() -> void;
    auto loadFonts() -> void;
    auto loadSFX() -> void;
    auto loadPresets() -> void;
    auto loadReplays() -> void;
    auto loadPieces() -> void;

    sf::RenderWindow m_render;
    std::unique_ptr<Scene::IScene> m_cur_scene;
    std::queue<sf::Event> m_event_queue;
    bool drawn = false;
    std::function<void(Scene::IScene *)> m_change_scene;
    std::function<void()> m_quit;
};

#endif // GAME_HPP