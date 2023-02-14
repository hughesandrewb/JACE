#ifndef JACE_INCLUDE_GAME_MAGICNUMBERS_H_
#define JACE_INCLUDE_GAME_MAGICNUMBERS_H_

#include <jace/include/game/Tables.h>
#include <jace/include/game/Bitboard.h>

namespace jace {

namespace game {

extern const Bitboard kRookAttacks[64][4096];
extern const MagicEntry kRookMagicEntries[64];

extern const Bitboard kBishopAttacks[64][4096];
extern const MagicEntry kBishopMagicEntries[64];

}

}

#endif  // JACE_INCLUDE_GAME_MAGICNUMBERS_H_
