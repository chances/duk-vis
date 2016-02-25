# Include OOGL

include_directories(vendor/OOGL/include)

# Link with OOGL
set(OOGL_LIBRARY ${CMAKE_SOURCE_DIR}/vendor/OOGL/lib/OOGL.a)
list(APPEND LINK_LIBS ${OOGL_LIBRARY})
