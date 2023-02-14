// Copyright 2023 Andrew Hughes
// jace/src/main.cc

#include <jace/include/game/Board.h>
#include <jace/include/game/Color.h>
#include <jace/include/game/Move.h>
#include <jace/include/game/rng.h>
#include <jace/include/game/MagicNumbers.h>

#include <iostream>
#include <chrono>

const char* SQSTR[65] = {
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"None"
};

uint64_t movs = 0;

uint64_t Perft(jace::game::Board *board, int depth) {
    int nodes = 0;
    
    if (depth == 0) {
        return (uint64_t) 1;
    }

    jace::game::Move *moves = (jace::game::Move*) malloc(sizeof(jace::game::Move) * 218);
    size_t n_moves = board->GenerateLegalMoves(moves);
    for (size_t index = 0; index < n_moves; index++) {
        board->MakeMove(moves[index]);
        if (!board->IsInCheck(~board->current_player_)) {
            nodes += Perft(board, depth - 1);
        }
        board->UnmakeMove(moves[index]);
    }
    free(moves);
    return nodes;
}

void PerftDiv(jace::game::Board* board, int depth) {
    uint64_t nodes = 0, pf = 0;
    jace::game::Move *moves = (jace::game::Move*) malloc(sizeof(jace::game::Move) * 218);
    size_t n_moves = board->GenerateLegalMoves(moves);
    for (size_t index = 0; index < n_moves; index++) {
        board->MakeMove(moves[index]);
        if (!board->IsInCheck(~board->current_player_)) {
            std::cout << SQSTR[moves[index].GetFrom()] << SQSTR[moves[index].GetTo()];
            pf = Perft(board, depth - 1);
            std::cout << ": " << pf << " moves\n";
            nodes += pf;
        }
        board->UnmakeMove(moves[index]);
    }
    free(moves);
    std::cout << "\nTotal: " << nodes << " moves\n";
}

int main(int argc, char *argv[]) {
    int depth = 6;
    if (argc > 1) {
        depth = std::stoi(argv[1]);
    }
    jace::game::Board *board = new jace::game::Board();
    board->Display();

    std::chrono::milliseconds start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    PerftDiv(board, depth);
    std::chrono::milliseconds end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::cout << (end - start).count() << "msec" << '\n';

    delete board;

    std::cout << movs << '\n';

    return 0;
}
