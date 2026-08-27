cmake_minimum_required(VERSION 3.40)
project(Mamuna LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw 
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

FetchContent_Declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm.git
    GIT_TAG master
)

FetchContent_MakeAvailable(
    glfw
    cglm
    stb
)







add_library(Mamuna STATIC external/glad/src/gl.c 
	src/Mamuna_Window.c src/Mamuna_Renderer.c src/Mamuna_Files.c
	src/Mamuna_Hashmap.c src/Mamuna_Vector.c
)

target_include_directories(Mamuna 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include/

	PRIVATE ${stb_SOURCE_DIR}
)

find_package(Threads REQUIRED)
target_link_libraries(Mamuna PUBLIC Threads::Threads
	glfw
	cglm
	m
)
