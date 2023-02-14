#ifndef JACE_INCLUDE_ARCH_ARM_H_
#define JACE_INCLUDE_ARCH_ARM_H_

#include <cstdint>
#include <bit>

#include <jace/include/game/Bitboard.h>
#include <jace/include/game/Tables.h>

namespace jace {

namespace arch {

uint16_t PopBitScanForward(game::Bitboard *);

uint32_t CountLeadingZeros(game::Bitboard);
game::Bitboard ReverseBits(game::Bitboard);

uint8_t BitScanForward(game::Bitboard);
uint8_t BitScanReverse(game::Bitboard);

uint16_t PopCount(game::Bitboard);

}  // namespace arch

}  // namespace jace

#endif  // JACE_INCLUDE_ARCH_ARM_H_