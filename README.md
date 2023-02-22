
# JACE

Just Another Chess Engine

## About

JACE is a chess engine based around a combination of bitboards and 8x8 array of pieces. A bitboard take advantage of the fact that a chess board has 64 pieces and a modern processor happens to have 64 bit registers. JACE uses 12 bitboards to keep track of each piece and color, i.e. 1 keeping track of black pawns, 1 keeping track of black knights, etc. It is then able to take advantage of how quickly modern processors can complete bitwise operations. For example, it can quickly perform a lookup for a positions knight attacks and bitwise and with enemy pieces to find all squares that a knight can attack.
```Bitboard attacks |= kKinghtAttacks[a4] & enemy_pieces```

It also implements the use of Magic Bitboards. Magic Bitboards are a way to quickly perform lookups for slider pieces(rooks, bishops, and queens). It uses a hash table and a perfect hash function to perform such lookups. The function takes in the boards relevant occupancy squares, the squares that the slider piece can "see" through pieces, and the square that the slider piece is located and returns the squares that the slider can move or attack. It is a much faster approach than what is called the classical approach which is a much more intuitive way to do the same thing.

## Usage

**Warning**
JACE is currently only known to be building on apple silicon processors due to the fact that it has some assembly to greatly speed up certain bitwise functions, such as population count and bit scan forward.

To compile, just clone the repo and run make in the JACE directory, this will place the *jace* executable in the build directory. From there you can run the *build/jace* executable with any integer as a single argument depicting how deep you want the perft test to run.

### Perft

Currently, JACE is really only useful for getting Perft results. Perft is a common chess PERFormance Test, it simply outputs the count of possible moves after *n* moves. Any values above 6 will take a long time and any above 8 or 9 might take a very very long time.

## Future Features

I would like JACE to become a proper chess engine eventually with an evalutation and search function, but for now I am focusing on creating the move generator as fast as possible. Not because I think it will help as a chess engine, because I know for a fact there is deminishing returns, but because this was meant to be an exercise in optimization.

- [ ] Universal Chess Interface
- [ ] A Terminal User Interface(TUI) to use the chess engine in the terminal (like GNU Chess)
- [ ] Implement a legal move only generation function, rather than pseudolegal move generation
- [ ] Add other architecture support, mainly x86

## Contact

Andrew Hughes - hughesandrewb@gmail.com
