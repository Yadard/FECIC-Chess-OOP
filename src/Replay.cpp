// clang-format off
#include "pch.hpp"
// clang-format on

#include "Replay.hpp"

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

Replay::Replay() : m_name(""), m_board_size(0, 0) {}
Replay::Replay(std::string t_name, sf::Vector2u t_board_size, std::vector<PieceInfo> &t_pieces, std::vector<Move> &t_moves) {
    m_name = t_name;
    m_board_size = t_board_size;
    m_pieces.assign(t_pieces.cbegin(), t_pieces.cend());
    m_moves.assign(t_moves.cbegin(), t_moves.cend());
}

auto Replay::loadFromFile(const char *path) -> void {
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

auto Replay::loadFromMemory(const char *memory) -> void {
    std::stringstream stream(memory);
    ParsingInfo parse_info;
    parse_info.file_name = "<Memory>";

    this->parseInput(parse_info, stream);
}

auto Replay::loadFromFile(const std::filesystem::path &path) -> void { this->loadFromFile(path.string().c_str()); }

auto Replay::del() -> void {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path path = cwd.parent_path();
    path.append("replays");
    if (!std::filesystem::exists(path))
        return;

    std::string filename(m_name + ".replay");
    path.append(filename);

    if (!std::filesystem::exists(path))
        return;

    std::filesystem::remove(path);
}

auto Replay::save() -> void {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path path = cwd.parent_path();
    path.append("replays");
    if (!std::filesystem::exists(path))
        std::filesystem::create_directory(path);

    std::string filename(m_name + ".replay");
    path.append(filename);

    std::ofstream file(path);
    file << "Name: " << m_name << '\n';
    file << "Board: " << m_board_size.x << 'x' << m_board_size.y << '\n';
    file << "Pieces: " << '\n';
    for (auto &piece : m_pieces) {
        file << '\t' << piece.name << '[' << piece.board_pos.x << ',' << piece.board_pos.y << ',';
        if (piece.team == Team::BLACK)
            file << "BLACK]\n";
        else
            file << "WHITE]\n";
    }
    file << "Moves: " << '\n';
    for (auto &move : m_moves) {
        file << '\t' << '[' << move.getMoveOrigin().x << ',' << move.getMoveOrigin().y << "] -> [" << move.getMoveDestination().x << ','
             << move.getMoveDestination().y << "]\n";
    }
}

auto Replay::copyPreset(Preset &preset) -> void {
    m_board_size = preset.getBoardSize();
    m_pieces.assign(preset.getPiecesInfo().cbegin(), preset.getPiecesInfo().cend());
}

auto Replay::getName() const -> const std::string & { return m_name; }
auto Replay::setName(std::string name) -> void {
    if (isValidName(name))
        m_name = name;
}

auto Replay::getBoardSize() const -> sf::Vector2u { return m_board_size; }
auto Replay::setBoardSize(sf::Vector2u board_size) -> void {
    if (isValidBoardSize(board_size)) {
        m_board_size = board_size;
    }
}

auto Replay::getPiecesInfo() const -> const std::vector<PieceInfo> & { return m_pieces; }
auto Replay::addPieceInfo(PieceInfo piece_info) -> void {
    if (isValidPieceInfo(piece_info))
        m_pieces.push_back(piece_info);
}

auto Replay::getMoves() const -> const std::vector<Move> & { return m_moves; }
auto Replay::addMove(Move move) -> void {
    if (isValidMove(move))
        m_moves.push_back(move);
}

auto Replay::isValidName(std::string_view name) -> bool {
    if (name.empty())
        return false;
    return std::none_of(name.begin(), name.end(), [](const char c) { return !std::isalpha(c); });
}
auto Replay::isValidBoardSize(sf::Vector2u board_size) -> bool { return board_size.x != 0 && board_size.y != 0; }
auto Replay::isValidPieceInfo(PieceInfo piece_info) -> bool { return inBounds(piece_info.board_pos) && isValidName(piece_info.name); }
auto Replay::isValidMove(Move move) -> bool { return inBounds(move.getMoveDestination()) && inBounds(move.getMoveOrigin()); }

auto Replay::inBounds(sf::Vector2u pos) -> bool { return (pos.x >= 0 && pos.x < m_board_size.x) && (pos.y >= 0 && pos.y < m_board_size.y); }

auto Replay::parseInput(ParsingInfo &parsing_info, std::istream &input_raw) -> void {
    std::string line;
    bool parsing_pieces = false;
    bool parsing_moves = false;
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

        } else if (!parsing_moves) {
            Entry parsed = split(parsing_info.line_stripped, ":");
            if (parsed.identifier == "moves") {
                parsing_moves = true;
                continue;
            }

            if (this->parsePieces(parsing_info) == Team::BLACK)
                piece_info.amount_black++;
            else
                piece_info.amount_white++;
        } else {
            this->parseMoves(parsing_info);
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

    if (!parsing_moves) {
        m_error_msg.str(std::string());
        m_error_msg << '[' << parsing_info.file_name << "]: Field \"Moves\" wasn't found!";
        throw ParsingError(m_error_msg.str());
        return;
    }

    if (m_moves.empty()) {
        m_error_msg.str(std::string());
        m_error_msg << '[' << parsing_info.file_name << "]: There is no moves saved, file is corrupted";
        throw ParsingError(m_error_msg.str());
        return;
    }
}

auto Replay::parsePieces(ParsingInfo parsing_info) -> Team {
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

    return Team::UNKNOWN;
}

auto Replay::parseMoves(ParsingInfo parsing_info) -> void {
    const uint32_t UNINITIALIZED_BOARDPOS = std::numeric_limits<uint32_t>::max();

    sf::Vector2u origin(UNINITIALIZED_BOARDPOS, UNINITIALIZED_BOARDPOS);
    sf::Vector2u dest(UNINITIALIZED_BOARDPOS, UNINITIALIZED_BOARDPOS);

    std::string &input(parsing_info.line_stripped);

    size_t start = 0;
    size_t len = 0;

    for (size_t i = 0; i < input.length(); i++) {
        const char ch = input[i];
        if (origin.x == UNINITIALIZED_BOARDPOS) {
            if (isOpeningToken(ch)) {
                start = i + 1;
            } else if (ch == ',') {
                len = i - start;
                origin.x = std::stoul(input.substr(start, len));
                start = i + 1;
            }
        } else if (origin.y == UNINITIALIZED_BOARDPOS) {
            if (isClosingToken(ch)) {
                len = i - start;
                origin.y = std::stoul(input.substr(start, len));
            }
        } else if (dest.x == UNINITIALIZED_BOARDPOS) {
            if (isOpeningToken(ch)) {
                start = i + 1;
            } else if (ch == ',') {
                len = i - start;
                dest.x = std::stoul(input.substr(start, len));
                start = i + 1;
            }
        } else if (dest.y == UNINITIALIZED_BOARDPOS) {
            if (isClosingToken(ch)) {
                len = i - start;
                dest.y = std::stoul(input.substr(start, len));
            }
        }
    }

    m_moves.emplace_back(dest, origin);
}

auto Replay::prepareInput(std::string &line) -> std::string {
    std::string input_stripped = line;

    input_stripped.erase(std::remove_if(input_stripped.begin(), input_stripped.end(), [this](const char c) { return !this->isToken(c); }),
                         input_stripped.end());

    for (auto &ch : input_stripped) {
        if (isalpha(ch))
            ch = tolower(ch);
    }

    return input_stripped;
}

auto Replay::isToken(const char c) -> bool {
    return isalnum(c) || isOpeningToken(c) || isClosingToken(c) || c == ',' || c == '\n' || c == ':' || c == '-' || c == '>';
}
auto Replay::isOpeningToken(const char c) -> bool { return c == '(' || c == '{' || c == '['; }
auto Replay::isClosingToken(const char c) -> bool { return c == ')' || c == '}' || c == ']'; }