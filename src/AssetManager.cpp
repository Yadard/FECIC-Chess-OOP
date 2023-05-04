#include "AssetManager.hpp"

std::unique_ptr<AssetManager> AssetManager::m_instance = nullptr;

auto AssetManager::GetInstance() -> AssetManager & {
    if (!m_instance)
        m_instance.reset(new AssetManager());
    return *m_instance;
}

auto AssetManager::registerTexture(std::string key, sf::Texture texture) -> void { this->m_sprites[key] = texture; }
auto AssetManager::hasTexture(std::string key) -> bool { return this->m_sprites.contains(key); }
auto AssetManager::getTexture(std::string key) -> const sf::Texture & { return this->m_sprites.at(key); }

auto AssetManager::registerFont(std::string key, sf::Font font) -> void { this->m_fonts[key] = font; }
auto AssetManager::hasFont(std::string key) -> bool { return this->m_fonts.contains(key); }
auto AssetManager::getFont(std::string key) -> const sf::Font & { return this->m_fonts.at(key); }