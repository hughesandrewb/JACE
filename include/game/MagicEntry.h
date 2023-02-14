#ifndef JACE_INCLUDE_GAME_MAGICENTRY_H_
#define JACE_INCLUDE_GAME_MAGICENTRY_H_

#include <JACE/include/game/Bitboard.h>

#include <cstdint>

namespace jace {

namespace game {

typedef class MagicEntry {
 public:
  MagicEntry();
  MagicEntry(Bitboard, uint64_t, uint8_t);
  Bitboard mask_;
  uint64_t magic_number_;
  uint8_t index_bits_;
} MagicEntry;

}

}

#endif  // JACE_INCLUDE_GAME_MAGICENTRY_H_