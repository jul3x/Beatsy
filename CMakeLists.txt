# CMake entry point
cmake_minimum_required (VERSION 3.8)
project (Beatsy)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(third-party/kissfft)
add_subdirectory(third-party/Gist)
add_subdirectory(third-party/jsoncpp)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
	Gist
	jsoncpp_lib
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

file(GLOB_RECURSE SRCS "src/*.cpp")
add_executable(Beatsy main.cpp ${SRCS})
target_link_libraries(Beatsy
	${ALL_LIBS}
)
