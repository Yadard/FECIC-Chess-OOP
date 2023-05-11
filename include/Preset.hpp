#ifndef PRESET_H
#define PRESET_H

#include <SFML/Graphics.hpp>
#include <Scenes/Chess/Pieces/Piece.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

class Preset {
  public:
    Preset();
    class ParsingError : public std::exception {
      public:
        ParsingError(const std::string &errorMsg) : errorMsg_(errorMsg) {}
        const char *what() const noexcept override { return errorMsg_.c_str(); }

      private:
        std::string errorMsg_;
    };

    struct ParsingInfo {
        std::string_view file_name;
        std::string line_raw;
        std::string line_stripped;
        size_t line_num;
    };

    struct PieceInfo {
        std::string name;
        sf::Vector2u board_pos;
        Team team;
    };

    struct PieceParsingInfo {
        size_t amount_black = 0;
        size_t amount_white = 0;
    };

    auto loadFromFile(const char *path) -> void;
    auto loadFromFile(const std::filesystem::path &path) -> void;

    auto loadFromMemory(const char *memory) -> void;

    auto saveToFile(const char *path) -> void;
    auto saveToFile(const std::filesystem::path &path) -> void;

    auto setBoardSize(sf::Vector2u board_size) -> void;
    auto setName(std::string name) -> void;
    auto addPiecesInfo(PieceInfo piece) -> void;
    auto erasePiecesInfo(size_t index) -> void;

    auto getBoardSize() const -> sf::Vector2u;
    auto getName() const -> const std::string &;
    auto getPiecesInfo() const -> const std::vector<PieceInfo> &;

  private:
    auto parseInput(ParsingInfo &parsing_info, std::istream &input_raw) -> void;
    auto parsePieces(ParsingInfo parsing_info) -> Team;

    auto prepareInput(std::string &line) -> std::string;
    auto isValidPieceIdentifier(std::string_view id) -> bool;

    auto isToken(const char c) -> bool;
    auto isOpeningToken(const char c) -> bool;
    auto isClosingToken(const char c) -> bool;

    sf::Vector2u m_board_size;
    std::string m_name;
    inline static std::stringstream m_error_msg;

    std::vector<PieceInfo> m_pieces;
};

#endif // PRESET_H