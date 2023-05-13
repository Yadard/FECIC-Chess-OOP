#  FECIC Chess

It's a game based on chess, but it has expanded on chess basic mechanics, with the piece DLL architecture you can create custom pieces that can have special abilities, totally customizable movement.

## Building
To build the project following the steps for your dev environment:

### Linux

To build the project, you will need CMake and a C++ compiler. On Linux, you can install CMake and a C++ compiler with the following command:

```
sudo apt-get install cmake g++
```

To build the project, follow these steps:

1. Create a build directory inside the project directory:
   ```
   mkdir build
   cd build
   ```

2. Generate the Makefile using CMake:
   ```
   cmake ..
   ```

3. Build the project:
   ```
   make
   ```

The executable will be created in the `bin` directory.
### Windows (MSVC)

To build the project on Windows using MSVC, you will need:

1. Install CMake: Download and install the latest version of CMake from the official website: https://cmake.org/download/

2. Install Visual Studio: Download and install Visual Studio Community or Visual Studio Professional from the official website: https://visualstudio.microsoft.com/downloads/

to build the source code do:

1. Open cmd and do the following commands
   ```
    git clone https://github.com/Yadard/FECIC-Chess-OOP.git
    cd FECIC-Chess-OOP
    mkdir build-win
    cd build-win
    cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install
    cmake --build . --config Release
   ```


The executable will be created in the `bin` directory.

## Usage

To run the executable, simply execute the following command:

```
./bin/Chess-FECIC
```
## Creation of Custom Pieces
This code allows the creation of custom pieces for the matches, this is done by the use of DLLs that holds the Piece logic, the sprites are stored in the assets/sprites folder. You can add a piece by just adding its DLL file in the ``./build/pieces`` folder and its sprites in the ``./assets/sprites/``. 
**But only do this if you know the DLL is trustworthy, I recommend compiling the DLL along side the project using the following guide**

To make a custom Piece you have to implement the [Piece.hpp interface](https://github.com/Yadard/FECIC-Chess-OOP/blob/main/include/Pieces/Piece.hpp) and you have access to the following [interface to influence the match](https://github.com/Yadard/FECIC-Chess-OOP/blob/main/include/Scenes/Chess/Match.hpp). to create a new piece you need to:
 
1.  add to ``./src/CMakeLists.txt`` the following line ``CreatePiece(${PieceName})`` like this:
    ```
    CreatePiece(Knight)
    ```
2. make a header file for your piece, you need to create a header file in the folder ``./include/Scenes/Chess/Pieces/`` the filename needs to be ``${PieceName}.hpp`` and then you create a implementation of the class ``Piece`` and create a ``createPiece`` function, example:
    ```c++
    // ./include/Scenes/Chess/Pieces/Knight.hpp
    #ifndef KNIGHT_HPP
    #define KNIGHT_HPP

    #include "Piece.hpp"

    class Knight : public Piece {
    public:
        Knight(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture);

        auto getMoves(Match *match) -> std::vector<Move> & override;
        auto doSpecialMove(Match *match) -> void override;
    };

    extern "C" PIECE_API_EXPORT Piece *createPiece(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) {
        return new Knight(t_team, t_position, t_texture);
    }

    #endif // KNIGHT_HPP
    ```
3. make the cpp implemation of the header created above in the folder ``./src/Scenes/Chess/Pieces/`` with the following name ``${PieceName}.cpp``. Example:
    ```c++
    // ./src/Scenes/Chess/Pieces/Knight.cpp
    // clang-format off
    #include "pch.hpp"
    // clang-format on

    #include "Scenes/Chess/Pieces/Knight.hpp"

    Knight::Knight(Team t_team, Move::BoardPos t_position, const sf::Texture &t_texture) : Piece(t_team, t_position, t_texture) {}

    auto Knight::getMoves(Match *match) -> std::vector<Move> & {
        this->m_move_list_data.clear();

        Move::BoardPos new_pos = this->goRight(this->goForwards(this->position, 2));
        if (this->inBounds(new_pos, match->getBoardSize()) && !isAlly(match->hasPiece(new_pos))) {
            this->m_move_list_data.emplace_back(new_pos, this->position);
        }
        //...

        return this->m_move_list_data;
    }

    auto Knight::doSpecialMove(Match *match) -> void {}
    ```
4. in the ``./assets/sprites`` folder you need to provide two .png files one for the sprite for the black team in the folder ``./assets/sprites/black`` and for the white team in the ``./assets/sprites/white``, in both the name needs to be ``${PieceName}.png``. Example:
```
./assets/sprites
        |
        ├───black
        |
        │       Knight.png
        |       ...
        │
        └───white
                Knight.png
                ...
```

5. Now you piece should appear as a DLL in ``./build/pieces``, and when you start the game it will be loaded automatically, if this didn't work [open a issue](https://github.com/Yadard/FECIC-Chess-OOP/issues).

## Development

The current state of the development of this game:
- **Features**
    - [x] dynamic system for creation of pieces
    - [x] working gameplay
    - [x] save board configuration for next matches
    - [x] save match replay
    - [ ] logging system
    - [ ] replay player
    - [ ] Improve piece's special abilities capabilities
    - [ ] Stalemate checker to end matches


- **Possible Features**
    - [ ] CLI version
    - [ ] Support online matches
    - [ ] Surrender option.



## Contributing

If you would like to contribute to the project, please follow these steps:

1. Fork the project on GitHub.
2. Clone your fork of the project to your local machine.
3. Create a new branch for your changes:
   ```
   git checkout -b my-new-feature
   ```
4. Make your changes and commit them:
   ```
   git commit -m "Add some feature"
   ```
5. Push your changes to your fork:
   ```
   git push origin my-new-feature
   ```
6. Create a pull request on GitHub.
