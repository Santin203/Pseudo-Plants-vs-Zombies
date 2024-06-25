# Plants vs. Zombies Tower Defense Game

## Overview
Welcome to the Plants vs. Zombies Tower Defense Game. This game aims to recreate the popular Plants vs. Zombies game mechanics using fundamental programming concepts.

## Objective
The main objective of this project is to implement a tower defense game where players defend against waves of zombies using various plants with unique abilities. The game features different levels of increasing difficulty, each requiring strategic placement and management of resources.

## Features
- **Game Mechanics**: Includes planting various types of plants on a grid-based board to defend against zombies.
- **Levels**: Three levels of increasing difficulty are implemented.
- **Custom Data Types**: Uses structs and enums to manage game entities and states.
- **Dynamic Data Structures**: Utilizes linked lists to manage and display game levels.
- **Function Hierarchy**: Organizes functionalities into a structured hierarchy for efficient game flow.
- **Input/Output**: Supports menu selection, level selection, and displays game instructions and credits.
- **Testing**: Includes a comprehensive test plan covering different game scenarios and edge cases.

## Structure Chart
The project follows a structured approach detailed in a flowchart or structure diagram, illustrating the relationships between functions and components within the game.

## Function Descriptions
The project includes several key functions:
- **Print Board**: Displays the game board.
- **Display Menu**: Shows the main menu and collects player inputs.
- **Display Instructions**: Loads and displays game instructions from a text file.
- **Create Dynamic Levels**: Constructs game levels using a linked list structure.
- **Zombie and Plant Actions**: Functions for placing plants, managing zombie movement, and attacking mechanics.
- **Win/Loss Conditions**: Checks win or loss conditions based on game objectives.

## Installation and Usage
To run the game locally:
1. Clone the repository:
   ```
   git clone https://github.com/SantiagoJimenez/PlantsVsZombiesTowerDefense.git
   ```
2. Compile the source code files.
3. Execute the compiled program to start the game.

## Testing
The project includes a detailed test plan to ensure functionality across various scenarios:
- **Bug Testing**: Validates handling of invalid inputs and edge cases.
- **Level Completion**: Tests winning and losing conditions for all levels and mini-games.
- **Menu Options**: Verifies functionality for all main menu options.

## Observations and Future Improvements
### What Worked Well
- Successfully implemented core game mechanics and objectives.
- Managed to avoid significant bugs with input validation.
- Effective utilization of linked lists for dynamic level creation.

### Challenges Faced
- Complexity in designing game logic and function interactions.
- Ensuring comprehensive variable management across different game components.

### Future Enhancements
- Addition of animations and improved graphics.
- Introduction of new game modes or extended storylines.
- Optimization of code for better performance and scalability.
