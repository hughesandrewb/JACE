#include <JACE/include/game/Square.h>

#include <iostream>

namespace jace {

namespace game {

Square operator+(Square square1, Square square2) {
	return Square(int(square1) + int(square2));
}

Square RelativeDirection(Color color, Direction direction) {
	return Square(static_cast<int>(direction) * (color == Color::kWhite ? 1 : -1));
}

}  // namespace game

}  // namespace jace

