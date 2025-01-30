# Yahtzee Game in C

This is a Yahtzee game implemented in C, where players can roll five dice and attempt to get various combinations like Three of a Kind, Four of a Kind, Full House, Small Straight, Large Straight, and Yahtzee. The program also includes logic for the computer to make optimal choices and score points based on the best available category.

## Features
- **Dice Rolling**: The game simulates the rolling of five dice with random values between 1 and 6.
- **Category Scoring**: Implements traditional Yahtzee scoring categories (e.g., Ones, Threes, Full House, etc.).
- **Computer AI**: The computer automatically chooses the best available scoring category based on dice values.
- **Game Patterns**: Detects patterns such as Three of a Kind, Full House, Small Straight, and Large Straight.
- **Modular Design**: Code is divided into smaller functions for easier testing, debugging, and extension.

## Requirements
- C Compiler (e.g., GCC)
- No external libraries required (uses standard libraries only)

## How the Game Works
- The game begins by rolling the dice.
- The player (or AI in this case) is given a list of available categories to score points based on the dice values.
- The computer will automatically select the best category and update the score.
- The game continues until all 13 categories have been used.

## Key Functions
- `rollDice()`: Rolls five dice and generates random values between 1 and 6.
- `getPatterns()`: Identifies patterns in the dice roll (e.g., Three of a Kind, Full House).
- `updateRepArray()`: Updates the frequency of each dice value.
- `printPoints()`: Displays the current score after each round.
- `printDice()`: Prints the dice values to the console.

## Future Improvements
- Implementing a player-vs-player mode.
- Adding a graphical user interface (GUI) using a library like SDL or ncurses.
- Improving the computer's decision-making algorithm to account for more advanced strategies.
