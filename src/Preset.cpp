// clang-format off
#include "pch.hpp"
// clang-format on

#include "Preset.hpp"

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

Preset::Preset() : m_name(""), m_board_size(0, 0) {}

auto Preset::loadFromFile(const char *path) -> void {
    std::ifstream file(path);
    bool parsing_pieces = false;
    ParsingInfo parse_info;
    parse_info.file_name = path;
    m_pieces.clear();

    if (!file.is_open()) {
        m_error_msg.str(std::string());
        m_error_msg << "Couldn't open the file \"" << path << "\"";
        throw ParsingError(m_error_msg.str());
        return;
    }

    parseInput(parse_info, file);
    file.close();
}

auto Preset::loadFromMemory(const char *memory) -> void {
    std::stringstream stream(memory);
    ParsingInfo parse_info;
    parse_info.file_name = "<Memory>";

    this->parseInput(parse_info, stream);
}

auto Preset::loadFromFile(const std::filesystem::path &path) -> void { this->loadFromFile(path.string().c_str()); }
auto Preset::saveToFile(const char *path) -> void {
    std::ofstream file(path);
    if (!file.is_open()) {
        m_error_msg.str(std::string());
        m_error_msg << "Couldn't create file \"" << path << "\"";
        throw ParsingError(m_error_msg.str());
        return;
    }

    file << "Name: " << this->m_name << '\n';
    file << "Board: " << this->m_board_size.x << 'x' << this->m_board_size.y << '\n';
    file << "Pieces:" << '\n';
    for (auto &piece : this->m_pieces) {
        std::string team = piece.team == Team::WHITE ? "White" : "Black";
        file << piece.name << '[' << piece.board_pos.x << ',' << piece.board_pos.y << ',' << team << '\n';
    }

    file.close();
}
auto Preset::saveToFile(const std::filesystem::path &path) -> void { this->saveToFile(path.string().c_str()); }

auto Preset::setBoardSize(sf::Vector2u board_size) -> void { m_board_size = board_size; }
auto Preset::setName(std::string name) -> void { m_name = name; }
auto Preset::addPiecesInfo(PieceInfo piece) -> void { m_pieces.push_back(piece); }
auto Preset::erasePiecesInfo(size_t index) -> void { m_pieces.erase(std::next(m_pieces.begin(), index)); }

auto Preset::getBoardSize() const -> sf::Vector2u { return this->m_board_size; }
auto Preset::getName() const -> const std::string & { return this->m_name; }
auto Preset::getPiecesInfo() const -> const std::vector<PieceInfo> & { return this->m_pieces; }

auto Preset::parseInput(ParsingInfo &parsing_info, std::istream &input_raw) -> void {
    std::string line;
    bool parsing_pieces = false;
    m_pieces.clear();
    PieceParsingInfo piece_info;

    for (size_t i = 0; std::getline(input_raw, line); i++) {
        std::cout << line << '\n';

        parsing_info.line_raw = line;
        parsing_info.line_stripped = this->prepareInput(line);
        parsing_info.line_num = i;

        if (!parsing_pieces) {
            Entry parsed = split(parsing_info.line_stripped, ":");

            if (parsed.identifier == "name") {
                m_name = parsed.value;
                m_name[0] = toupper(m_name[0]);
            } else if (parsed.identifier == "board") {
                Entry size = split(parsed.value, "x");
                m_board_size = sf::Vector2u(std::stoul(std::string(size.identifier)), std::stoul(std::string(size.value)));
            } else {
                parsing_pieces = parsed.identifier == "pieces";
            }

        } else {
            if (this->parsePieces(parsing_info) == Team::BLACK)
                piece_info.amount_black++;
            else
                piece_info.amount_white++;
        }
    }

    if (m_name.length() == 0) {
        m_error_msg.str(std::string());
        m_error_msg << '[' << parsing_info.file_name << "]: Field \"Name\" wasn't found";
        throw ParsingError(m_error_msg.str());
        return;
    }

    if (m_board_size.x == 0 || m_board_size.y == 0) {
        m_error_msg.str(std::string());
        m_error_msg << '[' << parsing_info.file_name << "]: Field \"Board\" wasn't found or setted to 0x0 which is invalid!";
        throw ParsingError(m_error_msg.str());
        return;
    }

    if (!parsing_pieces) {
        m_error_msg.str(std::string());
        m_error_msg << '[' << parsing_info.file_name << "]: Field \"Pieces\" wasn't found!";
        throw ParsingError(m_error_msg.str());
        return;
    }

    if (piece_info.amount_black == 0) {
        m_error_msg.str(std::string());
        m_error_msg << '[' << parsing_info.file_name << "]: There isn't a piece for the BLACK team, can't start a match";
        throw ParsingError(m_error_msg.str());
        return;
    }

    if (piece_info.amount_white == 0) {
        m_error_msg.str(std::string());
        m_error_msg << '[' << parsing_info.file_name << "]: There isn't a piece for the WHITE team, can't start a match";
        throw ParsingError(m_error_msg.str());
        return;
    }
}

auto Preset::parsePieces(ParsingInfo parsing_info) -> Team {
    std::string_view input(parsing_info.line_stripped);
    PieceInfo piece;
    size_t opening_token_location = input.npos;
    const uint32_t UNINITIALIZED_BOARDPOS = std::numeric_limits<uint32_t>::max();
    piece.board_pos.x = UNINITIALIZED_BOARDPOS;
    piece.board_pos.y = UNINITIALIZED_BOARDPOS;

    size_t start = input.npos;
    auto to_int = [](std::string_view str) { return std::stoul(std::string(str)); };

    for (size_t i = 0; i < input.length(); i++) {
        if (isOpeningToken(input[i])) {
            opening_token_location = i;
            piece.name = input.substr(0, i);
            piece.name[0] = toupper(piece.name[0]);
        } else if (opening_token_location != input.npos) {
            if (input[i] != ',' && !isClosingToken(input[i]))
                continue;

            if (piece.board_pos.x == UNINITIALIZED_BOARDPOS) {
                start = opening_token_location + 1;
                size_t len = i - start;
                piece.board_pos.x = to_int(input.substr(start, len));
                start = i + 1;
            } else if (piece.board_pos.y == UNINITIALIZED_BOARDPOS) {
                size_t len = i - start;
                piece.board_pos.y = to_int(input.substr(start, len));
                start = i + 1;
            } else if (isClosingToken(input[i])) {
                size_t len = i - start;
                std::string_view team_str = input.substr(start, len);
                if (team_str == "black")
                    piece.team = Team::BLACK;
                else if (team_str == "white")
                    piece.team = Team::WHITE;
                else {
                    m_error_msg.str(std::string());
                    m_error_msg << parsing_info.file_name << '[' << parsing_info.line_num << "]: "
                                << "Found a INVALID TEAM NAME \"" << team_str << "\" in \"" << parsing_info.line_raw << "\"";
                    throw ParsingError(m_error_msg.str());
                }

                m_pieces.push_back(piece);
                return piece.team;
            }
        }
    }
}
auto Preset::prepareInput(std::string &line) -> std::string {
    std::string input_stripped = line;

    input_stripped.erase(std::remove_if(input_stripped.begin(), input_stripped.end(),
                                        [this](const char c) {
                                            bool istoken = this->isToken(c);
                                            return !istoken;
                                        }),
                         input_stripped.end());

    for (auto &ch : input_stripped) {
        if (isalpha(ch))
            ch = tolower(ch);
    }

    return input_stripped;
}

auto Preset::isToken(const char c) -> bool { return isalnum(c) || isOpeningToken(c) || isClosingToken(c) || c == ',' || c == '\n' || c == ':'; }
auto Preset::isOpeningToken(const char c) -> bool { return c == '(' || c == '{' || c == '['; }
auto Preset::isClosingToken(const char c) -> bool { return c == ')' || c == '}' || c == ']'; }