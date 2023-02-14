#include <jace/include/arch/arm.h>

namespace jace {

namespace arch {

uint16_t PopBitScanForward(game::Bitboard *in) {
    uint16_t bit = BitScanReverse(*in);
    *in ^= game::kSquares[bit];
    return bit;
}

uint32_t CountLeadingZeros(game::Bitboard in) {
    asm(
        "clz %0, %0"
        : "+r"(in)
    );
    return in;
}

game::Bitboard ReverseBits(game::Bitboard in) {
    asm(
        "rbit %0, %0"
        : "+r"(in)
    );
    return in;
}

uint8_t BitScanForward(game::Bitboard in) {
    return CountLeadingZeros(ReverseBits(in));
}

uint8_t BitScanReverse(game::Bitboard in) {
    return 63 - CountLeadingZeros(in);
}

uint16_t PopCount(game::Bitboard in) {
    return __builtin_popcountll(in);
}

}  // namespace arch

}  // namespace jace
