#ifndef REPLAY_HPP
#define REPLAY_HPP

#include "Preset.hpp"
#include "Scenes/Chess/Pieces/Piece.hpp"
#include <SFML/Graphics.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

class Replay {
  public:
    Replay();
    Replay(std::string t_name, sf::Vector2u t_board_size, std::vector<PieceInfo> &t_pieces, std::vector<Move> &t_moves);
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

    struct PieceParsingInfo {
        size_t amount_black = 0;
        size_t amount_white = 0;
    };

    auto loadFromFile(const char *path) -> void;
    auto loadFromFile(const std::filesystem::path &path) -> void;

    auto loadFromMemory(const char *memory) -> void;

    auto del() -> void;
    auto save() -> void;

    auto copyPreset(Preset &preset) -> void;

    auto getName() const -> const std::string &;
    auto setName(std::string name) -> void;

    auto getBoardSize() const -> sf::Vector2u;
    auto setBoardSize(sf::Vector2u board_size) -> void;

    auto getPiecesInfo() const -> const std::vector<PieceInfo> &;
    auto addPieceInfo(PieceInfo piece_info) -> void;

    auto getMoves() const -> const std::vector<Move> &;
    auto addMove(Move move) -> void;

  private:
    auto isValidName(std::string_view name) -> bool;
    auto isValidBoardSize(sf::Vector2u board_size) -> bool;
    auto isValidPieceInfo(PieceInfo piece_info) -> bool;
    auto isValidMove(Move move) -> bool;
    auto inBounds(sf::Vector2u pos) -> bool;

    auto parseInput(ParsingInfo &parsing_info, std::istream &input_raw) -> void;
    auto parsePieces(ParsingInfo parsing_info) -> Team;
    auto parseMoves(ParsingInfo parsing_info) -> void;

    auto prepareInput(std::string &line) -> std::string;
    auto isValidPieceIdentifier(std::string_view id) -> bool;

    auto isToken(const char c) -> bool;
    auto isOpeningToken(const char c) -> bool;
    auto isClosingToken(const char c) -> bool;

    std::string m_name;
    sf::Vector2u m_board_size;
    std::vector<PieceInfo> m_pieces;
    std::vector<Move> m_moves;
    inline static std::stringstream m_error_msg;
};

#endif // REPLAY_HPP