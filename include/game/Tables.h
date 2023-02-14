#ifndef JACE_INCLUDE_GAME_TABLES_H_
#define JACE_INCLUDE_GAME_TABLES_H_

#include <JACE/include/game/Bitboard.h>
#include <JACE/include/game/rng.h>
#include <JACE/include/game/Square.h>
#include <JACE/include/game/MagicEntry.h>
#include <JACE/include/game/MagicNumbers.h>
#include <JACE/include/arch/arm.h>

#include <bit>
#include <iostream>

#include <array>

namespace jace {

namespace game {

enum RayDirections : int {
    kEastRays = 0,
    kNorthEastRays,
    kNorthRays,
    kNorthWestRays,
    kWestRays,
    kSouthWestRays,
    kSouthRays,
    kSouthEastRays,
};

constexpr int rank_of(Square s) { return (s >> 3); }
constexpr int file_of(Square s) { return (s & 0b111); }
constexpr int diagonal_of(Square s) { return 7 + rank_of(s) - file_of(s); }
constexpr int anti_diagonal_of(Square s) { return rank_of(s) + file_of(s); }

extern const std::array<std::array<Bitboard, 64>, 8> kRays;
extern const std::array<Bitboard, 65> kSquares;
extern const std::array<Bitboard, 64> kKingMoves;
extern const std::array<std::array<Bitboard, 64>, 2> kPawnAttacks;
extern const std::array<std::array<Bitboard, 64>, 2> kPawnMoves;
extern const std::array<Bitboard, 64> kKnightMoves;
extern const std::array<std::array<Bitboard, 64>, 64> kSquaresBetween;
extern const std::array<Bitboard, 64> kRookBlockerMasks;

std::array<MagicEntry, 64> GenerateRookMagicEntries();

size_t MagicIndex(MagicEntry entry, Bitboard blockers);
void InitializeMagicBitboards();
Bitboard RookAttack(Bitboard, Square);
Bitboard BishopAttack(Bitboard, Square);

}  // namespace game

}  // namespace jace

#endif  // JACE_INCLUDE_GAME_TABLES_H_
