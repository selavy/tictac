#include <assert.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <climits>
#include <iostream>
#include <string>
#include "tictac.h"

void print(Board b) {
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            int i = 3 * y + x;
            char c = b.xplayed(i) ? 'X' : b.oplayed(i) ? 'O' : /*' '*/ i + '1';
            if (x != 2)
                printf(" %c |", c);
            else if (y != 2)
                printf(" %c\n-----------\n", c);
            else
                printf(" %c\n", c);
        }
    }
}

int negamax(Board board, int color, int depth) {
    if (board.xwin())
        return color * 100 * depth;
    else if (board.owin())
        return color * -100 * depth;
    else if (board.tie())
        return 0;
    int value = INT_MIN;
    Moves moves = board.genmoves();
    assert(moves.length() != 0);
    for (int i = 0; i < moves.length(); ++i) {
        board.make_move(moves[i]);
        value = std::max(value, -negamax(board, -color, depth - 1));
        board.undo_move(moves[i]);
    }
    assert(value != INT_MIN);
    return value;
}

int search(Board board) {
    int mult = board.xtm() ? -1 : 1;
    int bestscore = INT_MIN;
    int bestmove = -1;
    Moves moves = board.genmoves();
    for (int i = 0; i < moves.length(); ++i) {
        int move = moves[i];
        board.make_move(move);
        int score = -1 * negamax(board, mult, 9);
        board.undo_move(move);
        if (score > bestscore) {
            bestscore = score;
            bestmove = move;
        }
    }
    assert(bestmove != -1);
    return bestmove;
}

int get_move(Board board) {
    int x = -1;
    char* input;
    do {
        if ((input = readline("Move? (1-9) ")) == NULL) {
            exit(0);
        }
        if ('1' <= input[0] && input[0] <= '9') {
            x = input[0] - '1';
            if (!board.is_available(x)) x = -1;
        }
        free(input);
    } while (!(0 <= x && x <= 8));
    printf("\n");
    return x;
}

bool check_win(Board board) {
    if (board.xwin()) {
        print(board);
        std::cout << "\nX's win!\n";
        return true;
    } else if (board.owin()) {
        print(board);
        std::cout << "\nO's win!\n";
        return true;
    } else if (board.tie()) {
        print(board);
        std::cout << "\nCat's game!\n";
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    // max possible states is bounded by 9! = 362880, likely can reduce
    // this upper bound because many states end early from a win.
    // static Board stk[1 << 19];

    bool human_is_x = true;
    bool okay = false;
    do {
        char* input = readline("Play X's? [Y/n] ");
        if (!input) {
            exit(0);
        } else if (input[0] == '\0' || input[0] == 'Y' || input[0] == 'y') {
            human_is_x = true;
            okay = true;
        } else if (input[0] == 'N' || input[0] == 'n') {
            human_is_x = false;
            okay = true;
        }
        free(input);
    } while (!okay);
    printf("\n\n");

    Board board;
    int move;
    for (;;) {
        if (check_win(board)) break;
        if (board.xtm() == human_is_x) {
            print(board);
            printf("\n");
            move = get_move(board);
        } else {
            move = search(board);
        }
        board.make_move(move);
    }

    return 0;
}
