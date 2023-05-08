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

AssetManager::PieceFactory::~PieceFactory() {
    for (auto &entry : m_pieces) {
        if (entry.second.handle != NULL) {
            closeDLL(entry.second.handle);
        }
    }
}

#include <iostream>

auto AssetManager::PieceFactory::createPiece(std::string name, Team team, Move::BoardPos position) -> Piece * {
    std::string texture_name = name;
    if (team == Team::BLACK)
        texture_name += "_Black";
    else
        texture_name += "_White";
    return m_pieces.at(name).piece_maker(team, position, AssetManager::GetInstance().getTexture(texture_name));
}
auto AssetManager::PieceFactory::loadPiece(std::string name) -> void {
    std::string file(name + ".png");
    std::filesystem::path black_texture("./../assets/sprites/black");
    black_texture.append(file);
    std::filesystem::path white_texture("./../assets/sprites/white");
    white_texture.append(file);
    TextureInfo entry;

    sf::Texture texture;
    texture.setSmooth(true);
    if (!texture.loadFromFile(black_texture.string().c_str())) {
        std::cerr << "Failed to load black sprite for " << name << std::endl;
    }

    std::string key(name + "_Black");
    AssetManager::GetInstance().registerTexture(key, texture);
    entry.piece_black_texture = &AssetManager::GetInstance().getTexture(key);

    if (!texture.loadFromFile(white_texture.string().c_str())) {
        std::cerr << "Failed to load white sprite for " << name << std::endl;
    }
    key = name;
    key += "_White";
    AssetManager::GetInstance().registerTexture(key, texture);
    entry.piece_white_texture = &AssetManager::GetInstance().getTexture(key);
    m_sprite_info[name] = entry;

    loadImplementation(name);
}

auto AssetManager::PieceFactory::loadImplementation(std::string name) -> void {
    Entry entry;

    std::string filename("lib" + name);
    std::filesystem::path path(folder);

    path.append(filename);
    entry.handle = openDLL(path, entry.handle);

    entry.piece_maker = getFunc(entry.handle);

    if (entry.piece_maker)
        m_pieces[name] = entry;
    else
        std::cerr << "Failed to load " << name << std::endl;
}

auto AssetManager::PieceFactory::closeDLL(DLLHandle_t handle) -> void {
#ifdef _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}

auto AssetManager::PieceFactory::getFunc(DLLHandle_t handle) -> PieceMaker {
    PieceMaker func;
#ifdef _WIN32
    func = reinterpret_cast<PieceMaker>(GetProcAddress(handle, FACTORY_NAME_PIECE_MAKER));
#else
    func = reinterpret_cast<PieceMaker>(dlsym(handle, FACTORY_NAME_PIECE_MAKER));
#endif
    if (func == NULL) {
        std::cerr << "Error getting function pointer" << std::endl;
        closeDLL(handle);
        exit(EXIT_FAILURE);
    }

    return func;
}
auto AssetManager::PieceFactory::openDLL(std::filesystem::path path, DLLHandle_t handle) -> DLLHandle_t {
    std::string file = path.string();
#ifdef _WIN32
    file += ".dll";
    handle = LoadLibrary(file.c_str());
    if (handle == NULL) {
        std::cerr << "Windows: Error loading dynamic library" << std::endl;
        exit(EXIT_FAILURE);
    }
#else
    file += ".so";
    handle = dlopen(file.c_str(), RTLD_LAZY);
    if (handle == NULL) {
        std::cerr << "Unix: Error loading dynamic library: " << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }
#endif
    return handle;
}

auto AssetManager::PieceFactory::cbegin() -> std::unordered_map<std::string, AssetManager::PieceFactory::TextureInfo>::const_iterator {
    return m_sprite_info.cbegin();
}
auto AssetManager::PieceFactory::cend() -> std::unordered_map<std::string, AssetManager::PieceFactory::TextureInfo>::const_iterator {
    return m_sprite_info.cend();
}