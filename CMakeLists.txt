cmake_minimum_required(VERSION 3.40)
project(Mamuna LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

FetchContent_MakeAvailable(stb)

FetchContent_Declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm.git
    GIT_TAG master
)

FetchContent_MakeAvailable(cglm)



set(CGLM_SHARED OFF CACHE BOOL "" FORCE)
set(CGLM_STATIC OFF CACHE BOOL "" FORCE)
set(CGLM_USE_C99 ON CACHE BOOL "" FORCE)


add_library(Mamuna STATIC external/glad/src/gl.c 
	src/Mamuna_Window.c src/Mamuna_Renderer.c src/Mamuna_Files.c
	src/Mamuna_Hashmap.c src/Mamuna_Vector.c
)

target_include_directories(Mamuna 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include/

	"${stb_SOURCE_DIR}"
	"${cglm_SOURCE_DIR}/include"
)

find_package(Threads REQUIRED)
if(UNIX)
	target_link_libraries(Mamuna PUBLIC Threads::Threads
		X11
		dl
		cglm
		m
	)
endif()
if(WIN32)
	target_link_libraries(Mamuna PUBLIC Threads::Threads
		cglm
		m
	)

endif()
