#ifndef JACE_INCLUDE_GAME_DIRECTION_H_
#define JACE_INCLUDE_GAME_DIRECTION_H_

namespace jace {

namespace game {

enum Direction {
    kEast = 1,
    kNorthEast = 9,
    kNorth = 8,
    kNorthWest = 7,
    kWest = -1,
    kSouthWest = -9,
    kSouth = -8,
    kSouthEast = -7,
};

}

}  // namespace jace

#endif  // JACE_INCLUDE_GAME_DIRECTION_H_