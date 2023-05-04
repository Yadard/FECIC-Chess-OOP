cmake_minimum_required(VERSION 3.19)

project(Chess-FECIC)

include(FetchContent)
set(SFML_VERSION "2.5.1")

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

message("A: ${sfml_SOURCE_DIR}")
include_directories(${sfml_SOURCE_DIR}/include)
find_package(OpenCV REQUIRED)

add_executable(
    Chess-FECIC
    main.cpp
    Chess.cpp
    History.cpp
    KillZone.cpp
    Move.cpp
    ${PROJECT_SOURCE_DIR}/Pieces/Pawn.cpp
    ${PROJECT_SOURCE_DIR}/Pieces/Piece.cpp
    ${PROJECT_SOURCE_DIR}/Pieces/Bishop.cpp
    ${PROJECT_SOURCE_DIR}/Pieces/Rook.cpp
    ${PROJECT_SOURCE_DIR}/Pieces/Knight.cpp
    ${PROJECT_SOURCE_DIR}/Pieces/Queen.cpp
    ${PROJECT_SOURCE_DIR}/Pieces/King.cpp
)

target_compile_features(Chess-FECIC PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Chess-FECIC POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Chess-FECIC> $<TARGET_FILE_DIR:Chess-FECIC> COMMAND_EXPAND_LISTS)
endif()

set_target_properties( 
    Chess-FECIC
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set (source "${CMAKE_SOURCE_DIR}/assets")
set (destination "${CMAKE_BINARY_DIR}")


target_link_libraries(Chess-FECIC
    PRIVATE
        sfml-audio
        sfml-graphics
        sfml-system
        sfml-window
)


file(COPY ${source} DESTINATION ${destination})
install(TARGETS Chess-FECIC DESTINATION ${CHESS_FECIC_INSTALL_BIN_DIR})