#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics.hpp>
#include <functional>

namespace Scene {
class IScene {
  public:
    IScene(sf::RenderWindow &render, std::function<void()> t_quit, std::function<void(IScene *)> t_change_scene)
        : m_render(render), m_quit(t_quit), m_change_scene(t_change_scene) {}
    virtual ~IScene() {}

    virtual auto update(sf::RenderWindow &render) -> void = 0;
    virtual auto draw(sf::RenderWindow &render) -> void = 0;
    virtual auto handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void = 0;

  protected:
    sf::RenderWindow &m_render;
    std::function<void()> m_quit;
    std::function<void(IScene *)> m_change_scene;
};
} // namespace Scene

#endif // SCENE_HPP