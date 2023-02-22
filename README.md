
# JACE

Just Another Chess Engine

## About

JACE is a chess engine based around a combination of bitboards and 8x8 array of pieces. A bitboard take advantage of the fact that a chess board has 64 pieces and a modern processor happens to have 64 bit registers. JACE uses 12 bitboards to keep track of each piece and color, i.e. 1 keeping track of black pawns, 1 keeping track of black knights, etc.

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
