cmake_minimum_required(VERSION 3.4)
project(duk_vis)

set(BUILD_TARGET_NAME "duk_vis")

file(GLOB SHADER_DATA_FILES
  "data/*.glsl"
)
file(COPY ${SHADER_DATA_FILES} DESTINATION .)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(cmake/oogl.cmake)
include(cmake/rxcpp.cmake)

list(APPEND SOURCE_FILES
    sources/gl/ShaderFile.cpp
    sources/gl/Triangle.cpp
)

list(APPEND SOURCE_FILES sources/main.cpp)
add_executable(${BUILD_TARGET_NAME} ${SOURCE_FILES})

macro(link_lib libName)
    unset(LIBRARY_TO_LINK CACHE)
    find_library(LIBRARY_TO_LINK ${libName})
    list(APPEND LINK_LIBS ${LIBRARY_TO_LINK})
endmacro()

link_lib(OpenGL)
link_lib(Cocoa)
link_lib(IOKit)
link_lib(CoreVideo)
link_lib(glfw3)

if (LINK_LIBS)
    target_link_libraries(${BUILD_TARGET_NAME} ${LINK_LIBS})
endif ()
