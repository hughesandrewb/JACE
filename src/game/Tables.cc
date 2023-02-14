// src/game/Tables.cc

#include <JACE/include/game/Tables.h>
#include <fstream>

namespace jace {

namespace game {

const Bitboard kNotAFile = 0xfefefefefefefefe; // ~0x0101010101010101
const Bitboard kNotHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080

constexpr Bitboard SouthOne (Bitboard b) {return  b >> 8;}
constexpr Bitboard NorthOne (Bitboard b) {return  b << 8;}
constexpr Bitboard EastOne (Bitboard b) {return (b << 1) & kNotAFile;}
constexpr Bitboard NorthEastOne (Bitboard b) {return (b << 9) & kNotAFile;}
constexpr Bitboard SouthEastOne (Bitboard b) {return (b >> 7) & kNotAFile;}
constexpr Bitboard WestOne (Bitboard b) {return (b >> 1) & kNotHFile;}
constexpr Bitboard SouthWestOne (Bitboard b) {return (b >> 9) & kNotHFile;}
constexpr Bitboard NorthWestOne (Bitboard b) {return (b << 7) & kNotHFile;}

constexpr std::array<std::array<Bitboard, 64>, 8> GenerateRays() {
    std::array<std::array<Bitboard, 64>, 8> result{};

    int directions[] = {1, 9, 8, 7, -1, -9, -8, -7};

    // TODO: add names to these
    Bitboard checks[] = {
        0x8080808080808080,
        0xff80808080808080,
        0xff00000000000000,
        0xff01010101010101,
        0x101010101010101,
        0x1010101010101ff,
        0xff,
        0x80808080808080ff};

    int direction_index = 0;
    for (direction_index = 0; direction_index < 8; direction_index++)
    {
        int square = 0;
        for (square = 0; square < 64; square++)
        {
            Bitboard bb = 0;
            Bitboard tmp = 1;
            int k = square;
            while (k < 64 && k >= 0)
            {
                tmp = 1;
                tmp <<= k;
                if ((tmp & checks[direction_index]))
                {
                    bb |= tmp;
                    break;
                }
                k += directions[direction_index];
                bb |= tmp;
            }
            result[direction_index][square] = bb ^ (1ULL << square);
        }
    }

    return result;
}

constexpr std::array<std::array<Bitboard, 64>, 8> kRays = GenerateRays();

constexpr std::array<Bitboard, 65> GenerateSquares() {
    std::array<Bitboard, 65> result{};

    for (size_t square = 0; square < 64; square++) {
        result[square] = 1ULL << square;
    }
    result[64] = 0;

    return result;
}

constexpr std::array<Bitboard, 65> kSquares = GenerateSquares();

constexpr std::array<Bitboard, 64> GenerateKingMoves() {
    std::array<Bitboard, 64> result{};

    for (int square = 0; square < 64; square++) {
        Bitboard attacks = 0;
        Bitboard bb = ((Bitboard)1) << square;

        attacks |= EastOne(bb) | WestOne(bb);
        bb |= attacks;
        attacks |= NorthOne(bb) | SouthOne(bb);

        result[square] = attacks;
    }

    return result;
}

constexpr std::array<Bitboard, 64> kKingMoves = GenerateKingMoves();

constexpr std::array<std::array<Bitboard, 64>, 2> GeneratePawnAttacks() {
    std::array<std::array<Bitboard, 64>, 2> result{};

    for (size_t color = 0; color < 2; color++) {
        for (int square = 0; square < 64; square++) {
            Bitboard attacks = 0;
            Bitboard bb = 1ULL << square;

            attacks |= color == 0 ? ((bb << 7) & 0x7f7f7f7f7f7f7f7f) : ((bb >> 7) & 0xfefefefefefefefe);
            attacks |= color == 0 ? ((bb << 9) & 0xfefefefefefefefe) : ((bb >> 9) & 0x7f7f7f7f7f7f7f7f);

            result[color][square] = attacks;
        }
    }
    
    return result;
}

constexpr std::array<std::array<Bitboard, 64>, 2> kPawnAttacks = GeneratePawnAttacks();

constexpr std::array<std::array<Bitboard, 64>, 2> GeneratePawnMoves() {
    std::array<std::array<Bitboard, 64>, 2> result{};

    for (size_t color = 0; color < 2; color++) {
        for (size_t square = 0; square < 64; square++) {
            Bitboard moves = 0;
            Bitboard bb = 1ULL << square;
            
            moves |= color == 0 ? (bb << 8) : (bb >> 8);
            if (color == 0) {
                if (square > 7 && square < 16) {
                    moves |= bb << 16;
                }
            } else {
                if (square > 47 && square < 56) {
                    moves |= bb >> 16;
                }
            }

            result[color][square] = moves;
        }
    }

    return result;
}

constexpr std::array<std::array<Bitboard, 64>, 2> kPawnMoves = GeneratePawnMoves();

constexpr std::array<Bitboard, 64> GenerateKnightMoves() {
    std::array<Bitboard, 64> result{};

    for (size_t square = 0; square < 64; square++) {
        result[square] = (kSquares[square] << 17 & 0xfefefefefefefefe)
                    | (kSquares[square] << 10 & 0xfcfcfcfcfcfcfcfc)
                    | (kSquares[square] >> 6 & 0xfcfcfcfcfcfcfcfc)
                    | (kSquares[square] >> 15 & 0xfefefefefefefefe)
                    | (kSquares[square] << 15 & 0x7f7f7f7f7f7f7f7f)
                    | (kSquares[square] << 6 & 0x3f3f3f3f3f3f3f3f)
                    | (kSquares[square] >> 10 & 0x3f3f3f3f3f3f3f3f)
                    | (kSquares[square] >> 17 & 0x7f7f7f7f7f7f7f7f);
    }

    return result;
}

constexpr std::array<Bitboard, 64> kKnightMoves = GenerateKnightMoves();

constexpr std::array<std::array<Bitboard, 64>, 64> GenerateSquaresBetween() {
    std::array<std::array<Bitboard, 64>, 64> result{};

    for (size_t square1 = 0; square1 < 64; square1++) {
        for (size_t square2 = 0; square2 < 64; square2++) {
            if (rank_of(Square(square1)) == rank_of(Square(square2))) {
                if (square1 > square2) {
                    result[square1][square2] = (kRays[kEastRays][square2] ^ kRays[kEastRays][square1]) & ~kSquares[square1];
                } else {
                    result[square1][square2] = (kRays[kEastRays][square1] ^ kRays[kEastRays][square2]) & ~kSquares[square2];
                }
            } else if (file_of(Square(square1)) == file_of(Square(square2))) {
                if (square1 > square2) {
                    result[square1][square2] = (kRays[kNorthRays][square2] ^ kRays[kNorthRays][square1]) & ~kSquares[square1];
                } else {
                    result[square1][square2] = (kRays[kNorthRays][square1] ^ kRays[kNorthRays][square2]) & ~kSquares[square2];
                }
            } else if (diagonal_of(Square(square1)) == diagonal_of(Square(square2))) {
                if (square1 > square2) {
                    result[square1][square2] = (kRays[kNorthEastRays][square2] ^ kRays[kNorthEastRays][square1]) & ~kSquares[square1];
                } else {
                    result[square1][square2] = (kRays[kNorthEastRays][square1] ^ kRays[kNorthEastRays][square2]) & ~kSquares[square2];
                }
            } else if (anti_diagonal_of(Square(square1)) == anti_diagonal_of(Square(square2))) {
                if (square1 > square2) {
                    result[square1][square2] = (kRays[kNorthWestRays][square2] ^ kRays[kNorthWestRays][square1]) & ~kSquares[square1];
                } else {
                    result[square1][square2] = (kRays[kNorthWestRays][square1] ^ kRays[kNorthWestRays][square2]) & ~kSquares[square2];
                }
            }
        }
    }

    return result;
}

constexpr std::array<std::array<Bitboard, 64>, 64> kSquaresBetween = GenerateSquaresBetween();

// magic bitboards

size_t MagicIndex(MagicEntry entry, Bitboard blockers) {
    blockers &= entry.mask_;
    uint64_t hash = blockers * entry.magic_number_;
    return hash >> entry.index_bits_;
}

Bitboard RookAttack(Bitboard blockers, Square square) {
    return kRookAttacks[square][MagicIndex(kRookMagicEntries[square], blockers)];
}

Bitboard BishopAttack(Bitboard blockers, Square square) {
    return kBishopAttacks[square][MagicIndex(kBishopMagicEntries[square], blockers)];
}


}

}
