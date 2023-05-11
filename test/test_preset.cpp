#include <Preset.hpp>
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(Preset, loadValidInput) {
    Preset p;
    const char *data = "Name: Piss\n"
                       "Board: 8x8\n"
                       "Pieces:\n"
                       "    Pawn[2,3, WHITE]\n"
                       "    Pawn[5,4, BLACK]\n";

    try {
        p.loadFromMemory(data);

        ASSERT_EQ(p.getName(), "Piss");

        ASSERT_EQ(p.getBoardSize().x, 8);
        ASSERT_EQ(p.getBoardSize().y, 8);

        ASSERT_EQ(p.getPiecesInfo().size(), 2);
        ASSERT_EQ(p.getPiecesInfo()[0].name, "Pawn");
        ASSERT_EQ(p.getPiecesInfo()[0].board_pos.x, 2);
        ASSERT_EQ(p.getPiecesInfo()[0].board_pos.y, 3);
        ASSERT_EQ(p.getPiecesInfo()[0].team, Team::WHITE);

        ASSERT_EQ(p.getPiecesInfo()[1].name, "Pawn");
        ASSERT_EQ(p.getPiecesInfo()[1].board_pos.x, 5);
        ASSERT_EQ(p.getPiecesInfo()[1].board_pos.y, 4);
        ASSERT_EQ(p.getPiecesInfo()[1].team, Team::BLACK);
    } catch (Preset::ParsingError &error) {
        ASSERT_TRUE(false);
    }
}

TEST(Preset, missingName) {
    Preset p;
    const char *data = "Board: 8x8\n"
                       "Pieces:\n"
                       "    Pawn[2,3, WHITE]\n"
                       "    Pawn[5,4, BLACK]\n";

    try {
        p.loadFromMemory(data);
        FAIL() << "Expected Preset::ParsingError";
    } catch (Preset::ParsingError const &err) {
        EXPECT_EQ(err.what(), std::string("[<Memory>]: Field \"Name\" wasn't found"));
    } catch (...) {
        FAIL() << "Expected Preset::ParsingError";
    }
}

TEST(Preset, missingBoard) {
    Preset p;
    const char *data = "Name: Hello\n"
                       "Board: 0x0\n"
                       "Pieces:\n"
                       "    Pawn[2,3, WHITE]\n"
                       "    Pawn[5,4, BLACK]\n";

    try {
        p.loadFromMemory(data);
        FAIL() << "Expected Preset::ParsingError";
    } catch (Preset::ParsingError const &err) {
        EXPECT_EQ(err.what(), std::string("[<Memory>]: Field \"Board\" wasn't found or setted to 0x0 which is invalid!"));
    } catch (...) {
        FAIL() << "Expected Preset::ParsingError";
    }
}

TEST(Preset, invalidBoard) {
    Preset p;
    const char *data = "Name: Hello\n"
                       "Pieces:\n"
                       "    Pawn[2,3, WHITE]\n"
                       "    Pawn[5,4, BLACK]\n";

    try {
        p.loadFromMemory(data);
        FAIL() << "Expected Preset::ParsingError";
    } catch (Preset::ParsingError const &err) {
        EXPECT_EQ(err.what(), std::string("[<Memory>]: Field \"Board\" wasn't found or setted to 0x0 which is invalid!"));
    } catch (...) {
        FAIL() << "Expected Preset::ParsingError";
    }
}

TEST(Preset, missingPieces) {
    Preset p;
    const char *data = "Name: Hello\n"
                       "Board: 8x8\n";

    try {
        p.loadFromMemory(data);
        FAIL() << "Expected Preset::ParsingError";
    } catch (Preset::ParsingError const &err) {
        EXPECT_EQ(err.what(), std::string("[<Memory>]: Field \"Pieces\" wasn't found!"));
    } catch (...) {
        FAIL() << "Expected Preset::ParsingError";
    }
}

TEST(Preset, cantStartMatchBLACK) {
    Preset p;
    const char *data = "Name: Hello\n"
                       "Board: 8x8\n"
                       "Pieces:\n"
                       "    Pawn[2,3, WHITE]\n"
                       "    Pawn[5,4, WHITE]\n";

    try {
        p.loadFromMemory(data);
        FAIL() << "Expected Preset::ParsingError";
    } catch (Preset::ParsingError const &err) {
        EXPECT_EQ(err.what(), std::string("[<Memory>]: There isn't a piece for the BLACK team, can't start a match"));
    } catch (...) {
        FAIL() << "Expected Preset::ParsingError";
    }
}

TEST(Preset, cantStartMatchWHITE) {
    Preset p;
    const char *data = "Name: Hello\n"
                       "Board: 8x8\n"
                       "Pieces:\n"
                       "    Pawn[2,3, BLACK]\n"
                       "    Pawn[5,4, BLACK]\n";

    try {
        p.loadFromMemory(data);
        FAIL() << "Expected Preset::ParsingError";
    } catch (Preset::ParsingError const &err) {
        EXPECT_EQ(err.what(), std::string("[<Memory>]: There isn't a piece for the WHITE team, can't start a match"));
    } catch (...) {
        FAIL() << "Expected Preset::ParsingError";
    }
}
