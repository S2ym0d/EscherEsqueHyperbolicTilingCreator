cmake_minimum_required(VERSION 3.10)
project(Escher_esque_Hyperbolic_Tiling_Creator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include paths
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external/Eigen
    ${PROJECT_SOURCE_DIR}/external/STB_IMAGE
    ${PROJECT_SOURCE_DIR}/external/OGL/GLEW/include
    ${PROJECT_SOURCE_DIR}/external/OGL/GLFW/include
)

# Add GLEW and GLFW
set(GLEW_LIB_DIR ${PROJECT_SOURCE_DIR}/external/OGL/GLEW/lib/Release/x64)
set(GLFW_LIB_DIR ${PROJECT_SOURCE_DIR}/external/OGL/GLFW/lib-vc2022)

add_definitions(-DGLEW_STATIC)  # Use static GLEW

# Platform-specific OpenGL and linking
if(WIN32)
    set(OPENGL_LIBS opengl32)
	set(GLEW_LIB glew32s.lib)  # Static GLEW for Windows
    set(GLFW_LIB glfw3.lib)    # GLFW library for Windows
endif()

# Source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# If SOURCES is empty, warn the user
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found. Please check your source directory.")
endif()


# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries (GLEW and GLFW libraries)
target_link_libraries(${PROJECT_NAME}
    ${GLEW_LIB_DIR}/${GLEW_LIB}
    ${GLFW_LIB_DIR}/${GLFW_LIB}
    ${OPENGL_LIBS}
)
