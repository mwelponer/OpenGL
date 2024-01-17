cmake_minimum_required(VERSION 3.0.0)
project( errors VERSION 0.1.0 LANGUAGES C CXX)

# set the build variant Degub/Release
set(CMAKE_BUILD_TYPE "Debug")

# Set C++ standard
#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_library( OpenGL_LIBRARY OpenGL )
find_library( COCOA_LIBRARY Cocoa )
find_library( IOKit_LIBRARY IOKit )

# GLFW - https://www.glfw.org/download.html
set( GLFW_INCLUDE_DIRS ../dependencies/glfw/include )
set( GLFW_LIBRARIES ../dependencies/glfw/lib-x86_64 )
# GLEW - brew install glew
set( GLEW_INCLUDE_DIRS ../dependencies/glew/include )
set( GLEW_LIBRARIES ../dependencies/glew/lib )

set( OPENGL-SRC
    main.cpp
)

# Add include directories
include_directories( 
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS} 
    ${PROJECT_SOURCE_DIR}/include
)

# Add library directories
link_directories( 
    ${GLFW_LIBRARIES} 
    ${GLEW_LIBRARIES} 
)

# Add executable target
add_executable( ${PROJECT_NAME} WIN32 
    ${OPENGL-SRC} 
)

# Link against libraries
target_link_libraries( ${PROJECT_NAME}
    ${IOKit_LIBRARY}
    ${COCOA_LIBRARY}
    ${OpenGL_LIBRARY}
    glfw3
    GLEW
)

# include(CTest)
# enable_testing()

# add_executable( openGL main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
