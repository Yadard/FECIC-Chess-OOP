#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include "Preset.hpp"
#include "Scenes/Chess/Pieces/Piece.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class AssetManager {

  public:
    AssetManager(AssetManager &other) = delete;
    void operator=(const AssetManager &) = delete;

    static auto GetInstance() -> AssetManager &;

    auto registerSFX(std::string key, sf::SoundBuffer sfx) -> void;
    auto hasSFX(std::string key) -> bool;
    auto getSFX(std::string key) -> const sf::SoundBuffer &;

    auto registerTexture(std::string key, sf::Texture texture) -> void;
    auto hasTexture(std::string key) -> bool;
    auto getTexture(std::string key) -> const sf::Texture &;

    auto registerFont(std::string key, sf::Font font) -> void;
    auto hasFont(std::string key) -> bool;
    auto getFont(std::string key) -> const sf::Font &;

    auto registerPreset(std::string key, Preset preset) -> void;
    auto hasPreset(std::string key) -> bool;
    auto getPreset(std::string key) -> const Preset &;
    auto presetCBegin() -> std::unordered_map<std::string, Preset>::const_iterator;
    auto presetCEnd() -> std::unordered_map<std::string, Preset>::const_iterator;

    auto registerPiece(std::string key, Piece *piece) -> void;
    auto hasPiece(std::string key) -> bool;
    auto getPiece(std::string key) -> const Piece &;

  private:
    AssetManager() = default;

    static std::unique_ptr<AssetManager> m_instance;

    std::unordered_map<std::string, sf::Texture> m_sprites;
    std::unordered_map<std::string, sf::Font> m_fonts;
    std::unordered_map<std::string, sf::SoundBuffer> m_sfx;
    std::unordered_map<std::string, Preset> m_presets;
    std::unordered_map<std::string, std::unique_ptr<Piece>> m_pieces;
};

#endif // ASSETMANAGER_HPP