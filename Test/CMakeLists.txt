# CMakeLists.txt for CrossWordPuzzleGenerator test program.

set(CROSSWORD_PUZZLE_TEST_SOURCES
	Source/Main.cpp
	Source/Main.h
)

add_executable(CrossWordPuzzleTest ${CROSSWORD_PUZZLE_TEST_SOURCES})

target_link_libraries(CrossWordPuzzleTest PRIVATE CrossWordPuzzleGenerator)