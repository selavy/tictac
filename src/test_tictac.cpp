#include "tictac.h"
#include <catch2/catch.hpp>
#include <vector>

TEST_CASE("Move List") {
    Moves moves;
    REQUIRE(moves.length() == 0);

    moves.append(4);
    REQUIRE(moves.length() == 1);
    REQUIRE(moves[0] == 4);

    moves.append(3);
    REQUIRE(moves.length() == 2);
    REQUIRE(moves[0] == 4);
    REQUIRE(moves[1] == 3);

    moves.pop();
    REQUIRE(moves.length() == 1);
    REQUIRE(moves[0] == 4);

    moves.pop();
    REQUIRE(moves.length() == 0);

    std::vector<int> vs = {3, 7, 8, 1, 2, 4, 0, 5, 6};
    for (int i = 0; i < (int)vs.size(); ++i) {
        REQUIRE(moves.length() == i);
        moves.append(vs[i]);
        REQUIRE(moves.back() == vs[i]);
        for (int j = 0; j < i; ++j) {
            REQUIRE(moves[j] == vs[j]);
        }
        REQUIRE(moves.length() == i + 1);
    }
}

TEST_CASE("Board") {
    Board board;
    REQUIRE(board.xtm() == true);
    REQUIRE(board.xmoves() == 0);
    REQUIRE(board.omoves() == 0);

    board.make_move(0);
    REQUIRE(board.xtm() == false);
    REQUIRE(board.xmoves() == 1);
    REQUIRE(board.omoves() == 0);
    REQUIRE(board.xplayed(0) == true);
    REQUIRE(board.oplayed(0) == false);

    board.make_move(1);
    REQUIRE(board.xtm() == true);
    REQUIRE(board.xmoves() == 1);
    REQUIRE(board.omoves() == 1);
    REQUIRE(board.xplayed(0) == true);
    REQUIRE(board.oplayed(0) == false);
    REQUIRE(board.xplayed(1) == false);
    REQUIRE(board.oplayed(1) == true);

    board.make_move(8);
    REQUIRE(board.xtm() == false);
    REQUIRE(board.xmoves() == 2);
    REQUIRE(board.omoves() == 1);
    REQUIRE(board.xplayed(0) == true);
    REQUIRE(board.oplayed(0) == false);
    REQUIRE(board.xplayed(1) == false);
    REQUIRE(board.oplayed(1) == true);
    REQUIRE(board.xplayed(8) == true);
    REQUIRE(board.oplayed(8) == false);

    board.undo_move(8);
    REQUIRE(board.xtm() == true);
    REQUIRE(board.xmoves() == 1);
    REQUIRE(board.omoves() == 1);
    REQUIRE(board.xplayed(0) == true);
    REQUIRE(board.oplayed(0) == false);
    REQUIRE(board.xplayed(1) == false);
    REQUIRE(board.oplayed(1) == true);

    board.undo_move(1);
    REQUIRE(board.xtm() == false);
    REQUIRE(board.xmoves() == 1);
    REQUIRE(board.omoves() == 0);
    REQUIRE(board.xplayed(0) == true);
    REQUIRE(board.oplayed(0) == false);
}

TEST_CASE("Detect wins") {
    // 0 | 1 | 2
    // ---------
    // 3 | 4 | 5
    // ---------
    // 6 | 7 | 8
    SECTION("X - Top row") {
        Moves moves = Moves::make({0, 3, 1, 4, 2});
        Board board = Board::make(moves);
        REQUIRE(board.xwin());
        REQUIRE(!board.owin());
    }

    SECTION("X - Middle row") {
        Moves moves = Moves::make({3, 0, 4, 1, 5});
        Board board = Board::make(moves);
        REQUIRE(board.xwin());
        REQUIRE(!board.owin());
    }

    SECTION("X - Bottom row") {
        Moves moves = Moves::make({6, 0, 7, 1, 8});
        Board board = Board::make(moves);
        REQUIRE(board.xwin());
        REQUIRE(!board.owin());
    }

    SECTION("X - Left column") {
        Moves moves = Moves::make({0, 1, 3, 2, 6});
        Board board = Board::make(moves);
        REQUIRE(board.xwin());
        REQUIRE(!board.owin());
    }

    SECTION("X - Middle column") {
        Moves moves = Moves::make({1, 3, 4, 2, 7});
        Board board = Board::make(moves);
        REQUIRE(board.xwin());
        REQUIRE(!board.owin());
    }

    SECTION("X - Right column") {
        Moves moves = Moves::make({2, 3, 5, 1, 8});
        Board board = Board::make(moves);
        REQUIRE(board.xwin());
        REQUIRE(!board.owin());
    }

    SECTION("X - Top Left to Bottom Right diagonal") {
        Moves moves = Moves::make({0, 3, 4, 1, 8});
        Board board = Board::make(moves);
        REQUIRE(board.xwin());
        REQUIRE(!board.owin());
    }

    SECTION("X - Bottom Right to Top Left diagonal") {
        Moves moves = Moves::make({6, 3, 4, 1, 2});
        Board board = Board::make(moves);
        REQUIRE(board.xwin());
        REQUIRE(!board.owin());
    }

    // ----

    SECTION("O - Top row") {
        Moves moves = Moves::make({8, 0, 3, 1, 4, 2});
        Board board = Board::make(moves);
        REQUIRE(!board.xwin());
        REQUIRE(board.owin());
    }

    SECTION("O - Middle row") {
        Moves moves = Moves::make({8, 3, 0, 4, 1, 5});
        Board board = Board::make(moves);
        REQUIRE(!board.xwin());
        REQUIRE(board.owin());
    }

    SECTION("O - Bottom row") {
        Moves moves = Moves::make({3, 6, 0, 7, 1, 8});
        Board board = Board::make(moves);
        REQUIRE(!board.xwin());
        REQUIRE(board.owin());
    }

    SECTION("O - Left column") {
        Moves moves = Moves::make({5, 0, 1, 3, 2, 6});
        Board board = Board::make(moves);
        REQUIRE(!board.xwin());
        REQUIRE(board.owin());
    }

    SECTION("O - Middle column") {
        Moves moves = Moves::make({0, 1, 3, 4, 2, 7});
        Board board = Board::make(moves);
        REQUIRE(!board.xwin());
        REQUIRE(board.owin());
    }

    SECTION("O - Right column") {
        Moves moves = Moves::make({4, 2, 3, 5, 1, 8});
        Board board = Board::make(moves);
        REQUIRE(!board.xwin());
        REQUIRE(board.owin());
    }

    SECTION("O - Top Left to Bottom Right diagonal") {
        Moves moves = Moves::make({5, 0, 3, 4, 1, 8});
        Board board = Board::make(moves);
        REQUIRE(!board.xwin());
        REQUIRE(board.owin());
    }

    SECTION("O - Bottom Right to Top Left diagonal") {
        Moves moves = Moves::make({0, 6, 3, 4, 1, 2});
        Board board = Board::make(moves);
        REQUIRE(!board.xwin());
        REQUIRE(board.owin());
    }
}
