cmake_minimum_required(VERSION 3.16)
project(pong C)

include(FetchContent)
FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG	       release-3.2.22
)

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL3)

add_executable(pong src/pong.c)
target_link_libraries(pong PRIVATE SDL3::SDL3)