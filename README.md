# Sushi Engine - Chess Engine Project

## Overview

**Sushi Engine** is a chess engine written in C, accompanied by a user interface (UI) developed in Python. The primary goal of this project is to create a fully functional chess engine capable of making intelligent moves based on chess algorithms.

The project is designed to evolve further, with a potential future feature being a Raspberry Pi-hosted bot that will play against other bots on platforms like [lichess.org](https://lichess.org). This allows for integration with the broader online chess community, where the engine can challenge other bots in real-time matches.

## Features
- **Chess Engine**: Written in C for efficiency, designed to evaluate positions and make decisions.
- **User Interface**: A Python-based UI for interacting with the engine.
- **Future Bot Integration**: Plans to deploy the engine as a bot on a Raspberry Pi to play games on lichess.org.

## Build Instructions

To build the chess engine, you can use the provided `Makefile` for easy compilation and management of the project.

### Makefile Commands

- **Build the Engine**:
  To compile the project, simply run:
  ```bash
  make
  ```
  This will compile the C source files in the `src` directory, optimize them, and produce an executable named `chess`.

- **Run Memory Leak Check**:
  To check for memory leaks using `valgrind`, you can run:
  ```bash
  make leak
  ```
  This will run `valgrind` with detailed leak checking on the chess engine.

- **Clean Build Files**:
  If you want to clean up the object files and executable, use:
  ```bash
  make clean
  ```
  
## Getting Started

Follow these steps to build and run the Sushi Engine:

### Prerequisites
To get started, ensure you have the following tools installed on your system:
- **C Compiler**: GCC or any C compiler that supports C11 standard.
- **Python 3.x**: Required for the user interface.
- **Valgrind**: Optional, but recommended for memory leak checking.

### Steps to Build and Run

1. **Clone the Repository**:
   First, clone the Sushi Engine project from the repository to your local machine:
   `git clone https://github.com/your-repo/sushi-engine.git`
   `cd sushi-engine`

2. **Build the Chess Engine**:
   Compile the C source files using the `Makefile`. This will generate an optimized executable named `chess`:
   `make`

3. **Run the Chess Engine**:
   After the successful build, you can run the chess engine with:
   `./chess`

4. **Run Memory Leak Check (Optional)**:
   If you want to test for memory leaks, you can run the engine through `valgrind` using:
   `make leak`
   This will perform a detailed memory analysis and report any potential issues.

5. **Clean Up Build Files**:
   To remove the object files and the compiled `chess` executable, run:
   `make clean`

## Future Development

### Raspberry Pi Bot Integration
The next step for this project is to deploy the chess engine as a bot running on a Raspberry Pi. The bot will be configured to play against other chess engines or players on platforms like [lichess.org](https://lichess.org) using the Lichess API.

This will allow the Sushi Engine to engage in online chess matches and improve by competing against a wide range of opponents.

## Creators
- [Adam Szweda](https://github.com/4d4mszweda)
- [Mikołaj Czyżak](https://github.com/mikolajczy01)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.
