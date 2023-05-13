#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include "Preset.hpp"
#include "Replay.hpp"
#include "Scenes/Chess/Pieces/Piece.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#ifdef _WIN32 // Windows
#include <Windows.h>
using DLLHandle_t = HMODULE;
#else // Unix-like systems (Linux, macOS)
using DLLHandle_t = void *;
#include <dlfcn.h>
#endif

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
    auto hasPreset(std::string key) const -> bool;
    auto getPreset(std::string key) const -> const Preset &;
    auto getPresets() const -> const std::unordered_map<std::string, Preset> &;

    auto registerReplay(std::string key, Replay replay) -> void;
    auto hasReplay(std::string key) const -> bool;
    auto getReplay(std::string key) const -> const Replay &;
    auto getReplays() const -> const std::unordered_map<std::string, Replay> &;

    class PieceFactory {
      public:
        struct TextureInfo {
            const sf::Texture *piece_black_texture = nullptr;
            const sf::Texture *piece_white_texture = nullptr;
        };

        ~PieceFactory();

        auto createPiece(std::string name, Team t_team, Move::BoardPos t_position) -> Piece *;
        auto loadPiece(std::string name) -> void;
        auto cbegin() -> std::unordered_map<std::string, AssetManager::PieceFactory::TextureInfo>::const_iterator;
        auto cend() -> std::unordered_map<std::string, AssetManager::PieceFactory::TextureInfo>::const_iterator;

      private:
        struct Entry {
            DLLHandle_t handle = nullptr;
            PieceMaker piece_maker = nullptr;
        };

        auto loadImplementation(std::string name) -> void;
        auto getFunc(DLLHandle_t handle) -> PieceMaker;
        auto openDLL(std::filesystem::path path, DLLHandle_t handle) -> DLLHandle_t;
        auto closeDLL(DLLHandle_t handle) -> void;

        std::filesystem::path folder = "./../pieces";
        std::unordered_map<std::string, Entry> m_pieces;
        std::unordered_map<std::string, TextureInfo> m_sprite_info;
    } piece_factory;

  private:
    AssetManager() = default;

    static std::unique_ptr<AssetManager> m_instance;

    std::unordered_map<std::string, sf::Texture> m_sprites;
    std::unordered_map<std::string, sf::Font> m_fonts;
    std::unordered_map<std::string, sf::SoundBuffer> m_sfxs;
    std::unordered_map<std::string, Preset> m_presets;
    std::unordered_map<std::string, Replay> m_replays;
};

#endif // ASSETMANAGER_HPP