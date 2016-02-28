cmake_minimum_required(VERSION 3.4)
project(duk_vis)

set(BUILD_TARGET_NAME "duk_vis")

file(GLOB SHADER_DATA_FILES
  "${CMAKE_SOURCE_DIR}/data/*.glsl"
)
file(MAKE_DIRECTORY ./data)
file(COPY ${SHADER_DATA_FILES} DESTINATION ./data)

file(GLOB SCRIPT_DATA_FILES
    "${CMAKE_SOURCE_DIR}/data/js/*.js"
    )
file(MAKE_DIRECTORY ./data/js)
file(COPY ${SHADER_DATA_FILES} DESTINATION ./data/js)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

include(cmake/cinder.cmake)
include(cmake/duktape.cmake)
include(cmake/rxcpp.cmake)

include_directories(vendor/semver-cpp/include)

# list(APPEND SOURCE_FILES
#     sources/gl/ShaderFile.cpp
#     sources/gl/Camera.cpp
# )

list(APPEND SOURCE_FILES sources/main.cpp)
add_executable(${BUILD_TARGET_NAME} ${SOURCE_FILES})

if (LINK_LIBS)
    target_link_libraries(${BUILD_TARGET_NAME} ${LINK_LIBS})
endif ()
