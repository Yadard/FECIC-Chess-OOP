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

auto AssetManager::registerSFX(std::string key, sf::SoundBuffer sfx) -> void { m_sfx[key] = sfx; }
auto AssetManager::hasSFX(std::string key) -> bool { return m_sfx.contains(key); }
auto AssetManager::getSFX(std::string key) -> const sf::SoundBuffer & { return m_sfx.at(key); }

auto AssetManager::registerPreset(std::string key, Preset preset) -> void { m_presets[key] = preset; }
auto AssetManager::hasPreset(std::string key) -> bool { return m_presets.contains(key); }
auto AssetManager::getPreset(std::string key) -> const Preset & { return m_presets.at(key); }
auto AssetManager::presetCBegin() -> std::unordered_map<std::string, Preset>::const_iterator { return m_presets.cbegin(); }
auto AssetManager::presetCEnd() -> std::unordered_map<std::string, Preset>::const_iterator { return m_presets.cend(); }

auto AssetManager::registerPiece(std::string key, Piece *piece) -> void { m_pieces[key].reset(piece); }
auto AssetManager::hasPiece(std::string key) -> bool { return m_pieces.contains(key); }
auto AssetManager::getPiece(std::string key) -> const Piece & { return *m_pieces.at(key); }