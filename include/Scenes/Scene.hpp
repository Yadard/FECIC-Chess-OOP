#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics.hpp>
#include <functional>

namespace Scene {
class IScene {
  public:
    virtual ~IScene() {}

    virtual auto update(sf::RenderWindow &render) -> void = 0;
    virtual auto draw(sf::RenderWindow &render) -> void = 0;
    virtual auto handle_input(std::function<void(Scene::IScene *)> change_scene, std::function<void()> quit, sf::Event event) -> void = 0;
};
} // namespace Scene

#endif // SCENE_HPP