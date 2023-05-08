#include "Preset.hpp"
#include <iostream>

struct Entry {
    std::string_view identifier, value;
};

static Entry split(std::string_view str, std::string_view separator) {
    Entry entry;
    size_t threshold = str.find(separator);

    entry.identifier = str.substr(0, threshold);
    entry.value = "";
    if (threshold < str.length())
        entry.value = str.substr(threshold + separator.length());

    size_t padding = 0;
    while (entry.value.length() && entry.value[padding] == ' ') {
        ++padding;
    }
    entry.value = entry.value.substr(padding);
    return entry;
}

auto Preset::loadFromFile(const char *path) -> bool {
    std::ifstream file(path);
    bool parsing_pieces = false;
    ParsingInfo parse_info;
    parse_info.file_name = path;
    m_pieces.clear();

    if (file.is_open()) {
        std::string line;

        for (size_t i = 0; std::getline(file, line); i++) {
            std::cout << line << '\n';
            parse_info.line_raw = line;
            parse_info.line_num = i;
            if (!parsing_pieces) {
                Entry parsed = split(line, ":");

                if (parsed.identifier == "Name") {
                    m_name = parsed.value;

                } else if (parsed.identifier == "Board") {
                    Entry size = split(parsed.value, "x");
                    m_board_size = sf::Vector2u(std::stoul(std::string(size.identifier)), std::stoul(std::string(size.value)));
                }

                parsing_pieces = parsed.identifier == "Pieces";
            } else {
                this->parsePiece(parse_info);
            }
        }

        file.close();
    } else {
        return false;
    }

    return true;
}
auto Preset::loadFromFile(const std::filesystem::path &path) -> bool { return this->loadFromFile(path.string().c_str()); }

auto Preset::copy(const Preset &preset) -> Preset & {
    m_name = preset.getName();
    m_board_size = preset.getBoardSize();
    for (auto &piece : preset.getPiecesInfo())
        m_pieces.push_back(piece);

    return *this;
}

auto Preset::setBoardSize(sf::Vector2u board_size) -> void { m_board_size = board_size; }
auto Preset::setName(std::string name) -> void { m_name = name; }
auto Preset::addPiecesInfo(PieceInfo piece) -> void { m_pieces.push_back(piece); }
auto Preset::erasePiecesInfo(size_t index) -> void { m_pieces.erase(std::next(m_pieces.begin(), index)); }

auto Preset::getBoardSize() const -> sf::Vector2u { return this->m_board_size; }
auto Preset::getName() const -> const std::string & { return this->m_name; }
auto Preset::getPiecesInfo() const -> const std::vector<PieceInfo> & { return this->m_pieces; }

auto Preset::isValidPieceIdentifier(std::string_view id) -> bool {
    return !std::any_of(id.begin(), id.end(), [](const char c) { return !isalpha(c); });
}

auto Preset::parsePiece(ParsingInfo parsing_info) -> void {
    bool found_opening = false;
    PieceInfo piece;
    std::string prefix(parsing_info.file_name);
    prefix += "#";
    prefix += parsing_info.line_num;
    prefix += ": ";

    size_t start = 0;
    size_t len = 0;
    size_t offset = 0;
    size_t row = std::string::npos;
    size_t col = std::string::npos;
    std::string_view team_raw = "";

    for (size_t i = 0; i < parsing_info.line_raw.length(); i++) {
        if (isblank(parsing_info.line_raw[i])) {
            ++offset;
            continue;
        }

        if (found_opening) {
            if (isClosingToken(parsing_info.line_raw[i])) {
                team_raw = parsing_info.line_raw.substr(start + offset, 5);
                if (team_raw.ends_with("WHITE")) {
                    piece.team = Team::WHITE;
                } else if (team_raw.ends_with("BLACK")) {
                    piece.team = Team::BLACK;
                } else {
                    prefix += "\"";
                    prefix += team_raw;
                    prefix += "\" isn't a valid team, remember it has to be in uppercase";
                    throw ParsingError(prefix);
                }
                this->m_pieces.push_back(piece);
                return;
            } else if (isdigit(parsing_info.line_raw[i])) {
                if (start == 0)
                    start = i;
                ++len;
            } else if (parsing_info.line_raw[i] == ',') {
                if (col == std::string::npos) {
                    col = std::stoul(std::string(parsing_info.line_raw.substr(start, len)));
                    piece.board_pos.x = col;
                    start = 0;
                    len = 0;
                } else if (row == std::string::npos) {
                    row = std::stoul(std::string(parsing_info.line_raw.substr(start, len)));
                    start = i + 1;
                    piece.board_pos.y = row;
                    len = 0;
                    offset = 0;
                }
            }
        }

        if (isOpeningToken(parsing_info.line_raw[i])) {
            std::string_view id = parsing_info.line_raw.substr(offset, i - offset);
            if (isValidPieceIdentifier(id))
                piece.name = id;
            else {
                prefix += "Invalid character on Piece identifier";
                throw ParsingError(prefix);
            }
            found_opening = true;
            offset = 0;
        }
    }
}

auto Preset::isOpeningToken(const char c) -> bool { return c == '(' || c == '{' || c == '['; }
auto Preset::isClosingToken(const char c) -> bool { return c == ')' || c == '}' || c == ']'; }