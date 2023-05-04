#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class AssetManager {

  public:
    AssetManager(AssetManager &other) = delete;
    void operator=(const AssetManager &) = delete;

    static auto GetInstance() -> AssetManager &;

    auto registerTexture(std::string key, sf::Texture texture) -> void;
    auto hasTexture(std::string key) -> bool;
    auto getTexture(std::string key) -> const sf::Texture &;

    auto registerFont(std::string key, sf::Font font) -> void;
    auto hasFont(std::string key) -> bool;
    auto getFont(std::string key) -> const sf::Font &;

  private:
    AssetManager() = default;

    static std::unique_ptr<AssetManager> m_instance;

    std::unordered_map<std::string, sf::Texture> m_sprites;
    std::unordered_map<std::string, sf::Font> m_fonts;
};


#endif // ASSETMANAGER_HPP