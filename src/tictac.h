#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

static constexpr int MaxMoves = 9;
static constexpr int MaxMoveNum = MaxMoves - 1;

constexpr bool is_valid_move(int move) noexcept {
    return 0 <= move && move <= MaxMoveNum;
}

using Move = int;

class Moves {
    static constexpr uint64_t LengthBits = 8;
    static constexpr uint64_t LengthMask = (1ull << LengthBits) - 1;
    static constexpr uint64_t MoveBits = 4;
    static constexpr uint64_t MoveMask = (1ull << MoveBits) - 1;
    static_assert(MaxMoveNum < (1u << MoveBits));
    static_assert(MaxMoves < (1u << LengthBits));

public:
    class ConstIterator;

    static constexpr Moves make(std::initializer_list<int> moves) {
        Moves result;
        for (int move : moves) {
            result.append(move);
        }
        return result;
    }

    constexpr bool isempty() const noexcept { return length() == 0; }

    constexpr int length() const noexcept { return _buf & LengthMask; }

    constexpr void pop() noexcept {
        uint64_t len = length();
        assert(0 < len && len <= MaxMoves);
        _buf = (_buf & ~LengthMask) | (len - 1);
    }

    constexpr void append(Move move) noexcept {
        uint64_t len = length();
        assert(0 <= len && len < MaxMoves);
        assert(0 <= move && move <= MaxMoveNum);
        int shift = _shift(len);
        _buf &= ~(MoveMask << shift);
        _buf |= (static_cast<uint64_t>(move) << shift);
        _buf = (_buf & ~LengthMask) | (len + 1);
    }

    constexpr int back() const noexcept {
        assert(!isempty());
        return operator[](length() - 1);
    }

    constexpr Move operator[](int movenum) const noexcept {
        assert(0 <= movenum && movenum < length());
        return (_buf >> _shift(movenum)) & MoveMask;
    }

private:
    static constexpr int _shift(int index) noexcept {
        assert(0 <= index && index <= MaxMoves);
        return (MoveBits * index + LengthBits);
    }

    uint64_t _buf = 0u;
};
static_assert(std::is_trivially_copyable_v<Moves>);
static_assert(sizeof(Moves) == 8);

class Moves::ConstIterator {
    Moves _moves;
    int _index = 0;

public:
    constexpr ConstIterator() noexcept = default;
    constexpr ConstIterator(Moves m, int i) noexcept : _moves{m}, _index{i} {}
    constexpr Move operator*() const noexcept { return _moves[_index]; }
    constexpr ConstIterator &operator++() noexcept {
        ++_index;
        return *this;
    }
    friend bool operator!=(ConstIterator lhs, ConstIterator rhs) noexcept {
        return lhs._index != rhs._index;
    }
};

constexpr Moves::ConstIterator begin(Moves moves) noexcept {
    return {moves, 0};
}
constexpr Moves::ConstIterator end(Moves moves) noexcept {
    return {moves, moves.length()};
}

enum class Side {
    X = 0,
    O = 1,
};

// TODO: why can't this be inside the Board class?
constexpr uint16_t _mask(int x) noexcept { return 1u << x; }

class Board {
    // 0 | 1 | 2
    // ---------
    // 3 | 4 | 5
    // ---------
    // 6 | 7 | 8
    static constexpr std::array<uint16_t, 8> WinMasks = {
        _mask(0) | _mask(1) | _mask(2), // top horiz
        _mask(3) | _mask(4) | _mask(5), // middle horiz
        _mask(6) | _mask(7) | _mask(8), // bottom horiz
        _mask(0) | _mask(3) | _mask(6), // left vert
        _mask(1) | _mask(4) | _mask(7), // middle vert
        _mask(2) | _mask(5) | _mask(8), // right vert
        _mask(0) | _mask(4) | _mask(8), // left top -> bottom right diag
        _mask(6) | _mask(4) | _mask(2), // left bottom -> top right diag
    };

    static constexpr uint16_t CatGameMask = _mask(0) | _mask(1) | _mask(2) |
                                            _mask(3) | _mask(4) | _mask(5) |
                                            _mask(6) | _mask(7) | _mask(8);

public:
    static constexpr Board make(Moves moves) noexcept {
        Board board;
        int length = moves.length();
        for (int i = 0; i < length; ++i) {
            board.make_move(moves[i]);
        }
        return board;
    }

    constexpr void make_move(int move) noexcept {
        assert(0 <= move <= 8);
        assert(!xplayed(move));
        assert(!oplayed(move));
        if (xtm()) {
            xs |= _mask(move);
        } else {
            os |= _mask(move);
        }
    }

    constexpr void undo_move(int move) noexcept {
        assert(is_valid_move(move));
        if (xtm()) {
            assert(oplayed(move));
            os &= ~(_mask(move));
        } else {
            assert(xplayed(move));
            xs &= ~(_mask(move));
        }
    }

    constexpr bool is_valid(int sq) const noexcept {
        return is_valid_move(sq) && is_available(sq);
    }

    constexpr bool is_available(int sq) const noexcept {
        return ((xs | os) & _mask(sq)) == 0u;
    }

    constexpr bool is_played(Side xtm, int move) const noexcept {
        assert(is_valid_move(move));
        return xtm == Side::X ? xplayed(move) : oplayed(move);
    }

    constexpr bool xplayed(int move) const noexcept {
        assert(is_valid_move(move));
        return (xs & _mask(move)) != 0;
    }

    constexpr bool oplayed(int move) const noexcept {
        assert(is_valid_move(move));
        return (os & _mask(move)) != 0;
    }

    constexpr int xmoves() const noexcept { return __builtin_popcount(xs); }

    constexpr int omoves() const noexcept { return __builtin_popcount(os); }

    constexpr bool xtm() const noexcept { return xmoves() == omoves(); }

    constexpr bool xwin() const noexcept {
        for (auto mask : WinMasks) {
            if ((xs & mask) == mask)
                return true;
        }
        return false;
    }

    constexpr bool owin() const noexcept {
        for (auto mask : WinMasks) {
            if ((os & mask) == mask)
                return true;
        }
        return false;
    }

    constexpr int tie() const noexcept { return (xs | os) == CatGameMask; }

    constexpr Moves genmoves() noexcept {
        Moves moves;
        for (int i = 0; i <= MaxMoveNum; ++i) {
            if (is_available(i))
                moves.append(i);
        }
        return moves;
    }

private:
    uint16_t xs = 0u;
    uint16_t os = 0u;
};
static_assert(std::is_trivially_copyable_v<Moves>);
static_assert(sizeof(Board) == 4);
