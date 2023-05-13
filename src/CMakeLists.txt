cmake_minimum_required(VERSION 3.19)
project(Chess-FECIC LANGUAGES CXX)

include(FetchContent)
set(SFML_VERSION "2.5.1")

function(CreatePiece arg)
    message("config for ${arg}")

    add_library(
        ${arg} 
        SHARED 
        ${PROJECT_SOURCE_DIR}/Scenes/Chess/Pieces/${arg}.cpp
        ${PROJECT_SOURCE_DIR}/Scenes/Chess/Pieces/Piece.cpp
        ${PROJECT_SOURCE_DIR}/Scenes/Chess/Move.cpp
        ${PROJECT_SOURCE_DIR}/Preset.cpp
        ${PROJECT_SOURCE_DIR}/AssetManager.cpp
    )

    target_link_libraries(
        ${arg} 
        PRIVATE 
            sfml-audio
            sfml-graphics
            sfml-system
            sfml-window
    )

    set_target_properties( 
        ${arg}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/pieces"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/pieces"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/pieces"
    )

    target_include_directories(${arg} PUBLIC include)
    target_precompile_headers(${arg} REUSE_FROM Chess-FECIC)

endfunction()

FetchContent_Declare(
    sfml
    GIT_REPOSITORY "https://github.com/SFML/SFML.git"
    GIT_TAG        "${SFML_VERSION}"
)

FetchContent_GetProperties(sfml)
if(NOT sfml_POPULATED)
    FetchContent_Populate(sfml)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()
FetchContent_MakeAvailable(sfml)

find_package(OpenCV REQUIRED)



add_executable(
    Chess-FECIC

    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/Game.cpp
    ${PROJECT_SOURCE_DIR}/Preset.cpp
    ${PROJECT_SOURCE_DIR}/Replay.cpp
    ${PROJECT_SOURCE_DIR}/AssetManager.cpp
    ${PROJECT_SOURCE_DIR}/ThreadDispatcher.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Button.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/TextInput.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Replay/ReplayPlayer.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/MainMenu/MainMenu.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/MainMenu/Logo.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/MainMenu/Popup/Play.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/MainMenu/Popup/Replay.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Chess/Chess.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Chess/History.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Chess/KillZone.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Chess/Move.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Chess/Board.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Chess/MatchResult/MatchResult.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Chess/MatchResult/StallMate.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Chess/MatchResult/Victory.cpp
    ${PROJECT_SOURCE_DIR}/Scenes/Chess/Pieces/Piece.cpp
)

target_include_directories(
    Chess-FECIC
    PRIVATE
    ${sfml_SOURCE_DIR}/include
)

target_precompile_headers(Chess-FECIC PUBLIC
${CHESS_FECIC_INSTALL_INCLUDE_DIR}/pch.hpp
)


CreatePiece(Pawn)
CreatePiece(Bishop)
CreatePiece(Rook)
CreatePiece(Knight)
CreatePiece(Queen)
CreatePiece(King)

target_compile_features(Chess-FECIC PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Chess-FECIC POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Chess-FECIC> $<TARGET_FILE_DIR:Chess-FECIC> COMMAND_EXPAND_LISTS)
endif()

set_target_properties( 
    Chess-FECIC
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/pieces"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(
    Chess-FECIC
        PRIVATE 
            sfml-audio
            sfml-graphics
            sfml-system
            sfml-window
    )



install(TARGETS Chess-FECIC DESTINATION ${CHESS_FECIC_INSTALL_BIN_DIR})