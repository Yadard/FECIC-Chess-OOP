enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

function(CreateTest test)
    message("config for test_${test}")

    target_link_libraries(
        ${test}
        GTest::gtest_main
        sfml-audio
        sfml-graphics
        sfml-system
        sfml-window
    )

    add_test(
        NAME test_${test}
        COMMAND ${test}
    )

    add_test(GoogleTest ${test})

    target_include_directories(
        ${test}
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    )

    if (WIN32)
        add_custom_command(
            TARGET ${test} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${test}> $<TARGET_FILE_DIR:${test}>
            COMMAND_EXPAND_LISTS
        )
    endif ()

    gtest_discover_tests(${test} PROPERTIES TEST_DISCOVERY_TIMEOUT 600)
endfunction()

include(GoogleTest)
add_executable(
        Preset
        ${PROJECT_SOURCE_DIR}/test/test_preset.cpp
        ${PROJECT_SOURCE_DIR}/src/Preset.cpp
        ${PROJECT_SOURCE_DIR}/src/Scenes/Chess/Pieces/Piece.cpp
    )
CreateTest(Preset)
add_executable(
        Replay
        ${PROJECT_SOURCE_DIR}/test/test_replay.cpp
        ${PROJECT_SOURCE_DIR}/src/Preset.cpp
        ${PROJECT_SOURCE_DIR}/src/Replay.cpp
        ${PROJECT_SOURCE_DIR}/src/Scenes/Chess/Pieces/Piece.cpp
        ${PROJECT_SOURCE_DIR}/src/Scenes/Chess/Move.cpp
    )
CreateTest(Replay)


