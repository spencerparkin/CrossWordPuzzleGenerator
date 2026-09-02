# CMakeLists.txt for CrossWordPuzzleGenerator project.

cmake_minimum_required(VERSION 3.27)

project(CrossWordPuzzleGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CROSSWORD_BUILD_TEST)
	set(CROSSWORD_BUILD_TEST ON)
endif()

add_subdirectory(Library)

if(CROSSWORD_BUILD_TEST)
	add_subdirectory(Test)
endif()