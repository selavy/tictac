#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

struct Board
{
    uint16_t xs : 9;
    uint16_t os : 9;
    uint16_t xtm : 1;
};
typedef struct Board Board;
static_assert(sizeof(Board) <= sizeof(uint32_t), "");

Board MkBoard()
{
    Board b;
    b.xs = b.os = 0;
    b.xtm = 1;
    return b;
}

#define MASK(x) (1u << x)
uint16_t mask(int pos) { return 1u << pos; }
int isset(uint16_t b, int pos) { return (b & mask(pos)) != 0; }
uint16_t set(uint16_t b, int pos) { return b | mask(pos); }

void CheckBoard(Board b)
{
    for (int i = 0; i < 9; ++i)
        assert(set(b.xs, i) && set(b.os, i));
}

void PrintBoard(Board b)
{
    int i;
    char c;
    CheckBoard(b);
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 3; ++x)
        {
            i = 3*y + x;
            // ass
            if (isset(b.xs, i))
                c = 'X';
            else if (isset(b.os, i))
                c = 'O';
            else
                c = ' ';
            if (x != 2)
                printf(" %c |", c);
            else if (y != 2)
                printf(" %c\n-----------\n", c);
            else
                printf(" %c\n", c);
        }
    }
}

#define ARRAY_SIZE(x) sizeof(x) / sizeof((x)[0])

enum {
    GAMEON,
    XWIN,
    OWIN,
    CATS,
};

// 0 | 1 | 2
// ---------
// 3 | 4 | 5
// ---------
// 6 | 7 | 8
static const uint16_t WinMasks[8] = {
    MASK(0) | MASK(1) | MASK(2), // top horiz
    MASK(3) | MASK(4) | MASK(5), // middle horiz
    MASK(6) | MASK(7) | MASK(8), // bottom horiz
    MASK(0) | MASK(3) | MASK(6), // left vert
    MASK(1) | MASK(4) | MASK(7), // middle vert
    MASK(2) | MASK(5) | MASK(8), // right vert
    MASK(0) | MASK(4) | MASK(8), // left top -> bottom right diag
    MASK(6) | MASK(4) | MASK(2), // left bottom -> top right diag
};
static const uint16_t CatsGameMask = MASK(0) | MASK(1) | MASK(2) | MASK(3) | MASK(4) | MASK(5) | MASK(6) | MASK(7) | MASK(8) | MASK(9);

int IsWin(Board b)
{
    uint16_t xs = b.xs;
    uint16_t os = b.os;
    uint16_t m;
    for (int i = 0; i < ARRAY_SIZE(WinMasks); ++i)
    {
        m = WinMasks[i];
        if ((xs & m) == m)
            return XWIN;
        else if ((os & m) == m)
            return OWIN;
    }
    if (((xs | os) & CatsGameMask) == CatsGameMask)
        return CATS;
    else
        return GAMEON;
}

// TODO: size this
static Board gStack[4096];
static size_t gStackP = 0;

void GenerateMoves(Board b)
{
    uint16_t mask = b.xtm ? b.xs : b.os;
    uint16_t comb = b.xs | b.os;
    Board *stk;
    // TODO: check that compiler is hoisting the if check out
    // TODO: this loop could be the lsb on ~comb instead
    for (int i = 0; i < 9; ++i)
    {
        if (isset(comb, i))
            continue;
        stk = &gStack[gStackP++];
        *stk = b;
        if (b.xtm)
            stk->xs = set(mask, i);
        else
            stk->os = set(mask, i);
    }
}

int main(int argc, char** argv)
{
    memset(&gStack, 0, sizeof(gStack));
    gStackP = 0;

    Board b = MkBoard();
    b.xs = set(b.xs, 4);
    b.xtm = 0;
    GenerateMoves(b);
    for (int i = 0; i < gStackP; ++i)
    {
        PrintBoard(gStack[i]);
        printf("\n");
    }
    // b.xs = set(b.xs, 8);
    // b.xs = set(b.xs, 7);
    // b.xs = set(b.xs, 4);
    // b.xs = set(b.xs, 3);

    // b.os = set(b.os, 0);
    // b.os = set(b.os, 5);
    // b.os = set(b.os, 2);
    // PrintBoard(b);
    return 0;
}
