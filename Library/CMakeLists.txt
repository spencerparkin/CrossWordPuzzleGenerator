# CMakeLists.txt for CrossWordPuzzleGenerator library.

set(CROSSWORD_PUZZLE_GENERATOR_SOURCES
	Source/CrossWord/PuzzleMatrix.cpp
	Source/CrossWord/PuzzleMatrix.h
	Source/CrossWord/PuzzleGenerator.cpp
	Source/CrossWord/PuzzleGenerator.h
	Source/CrossWord/WordBank.cpp
	Source/CrossWord/WordBank.h
	Source/CrossWord/WordTree.cpp
	Source/CrossWord/WordTree.h
	Source/CrossWord/Random.cpp
	Source/CrossWord/Random.h
)

add_library(CrossWordPuzzleGenerator STATIC ${CROSSWORD_PUZZLE_GENERATOR_SOURCES})

target_include_directories(CrossWordPuzzleGenerator PUBLIC Source)