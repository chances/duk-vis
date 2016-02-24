# Include OOGL

include_directories(vendor/OOGL/include)

list(APPEND SOURCE_FILES
    # sources/Window_GLFW.cpp
    sources/Context_GLFW.cpp
)

# Link with OOGL
set(OOGL_LIBRARY ${CMAKE_SOURCE_DIR}/vendor/OOGL/lib/OOGL.a)
list(APPEND LINK_LIBS ${OOGL_LIBRARY})
