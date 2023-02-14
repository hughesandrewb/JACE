#ifndef JACE_INCLUDE_GAME_COLOR_H_
#define JACE_INCLUDE_GAME_COLOR_H_

namespace jace {

namespace game {

enum class Color : int {
    kWhite,
    kBlack,
};

Color operator~(const Color c);

}  // namespace game

}  // namespace jace

#endif  // JACE_INCLUDE_GAME_PIECE_H_
