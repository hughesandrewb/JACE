#ifndef JACE_INCLUDE_GAME_MOVEFLAGS_H_
#define JACE_INCLUDE_GAME_MOVEFLAGS_H_

namespace jace {

namespace game {

enum class MoveFlags {
    kQuietMove = 0,
    kDoublePawnPush,
    kKingCastle,
    kQueenCastle,
    kCapture,
    kEnPassent,
    kKnightPromotion,
    kBishopPromotion,
    kRookPromotion,
    kQueenPromotion,
    kKnightPromotionCapture,
    kBishopPromotionCapture,
    kRookPromotionCapture,
    kQueenPromotionCapture,
};

}  // game

}  // jace

#endif  // JACE_INCLUDE_GAME_MOVEFLAGS_H_
