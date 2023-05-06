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

auto AssetManager::PieceFactory::createPiece(std::string name) -> Piece * { return m_pieces.at(name).piece_maker(); }
auto AssetManager::PieceFactory::loadPiece(std::string name) -> void {
    Entry entry;

    std::string filename("lib" + name);
    std::filesystem::path path(folder);

    path.append(filename);
    openDLL(path, entry.handle);

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
}
auto AssetManager::PieceFactory::openDLL(std::filesystem::path path, DLLHandle_t handle) -> void {
    std::string file = path.string();
#ifdef _WIN32
    file += ".dll";
    HMODULE library_handle = LoadLibrary(file.c_str());
    if (library_handle == NULL) {
        std::cerr << "Windows: Error loading dynamic library" << std::endl;
        exit(EXIT_FAILURE);
    }
#else
    file += ".so";
    void *library_handle = dlopen(file.c_str(), RTLD_LAZY);
    if (library_handle == NULL) {
        std::cerr << "Unix: Error loading dynamic library: " << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }
#endif
}