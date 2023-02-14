#include <JACE/include/game/Color.h>

namespace jace {

namespace game {

Color operator~(const Color c) {
    return c == Color::kWhite ? Color::kBlack : Color::kWhite;
}

}  // namespace game

}  // namespace jace
