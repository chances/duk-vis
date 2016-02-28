# Include Cinder

include_directories(vendor/Cinder/include)

# file(GLOB CINDER_SRC
#     ${PROJECT_SOURCE_DIR}/vendor/Cinder/src/cinder/app/*.cpp
# )
# list(REMOVE_ITEM CINDER_SRC "${PROJECT_SOURCE_DIR}/vendor/Cinder/src/cinder/app/AppScreenSaver.cpp")
# list(REMOVE_ITEM CINDER_SRC "${PROJECT_SOURCE_DIR}/vendor/Cinder/src/cinder/app/RendererDx.cpp")

# Link with Cinder's library
set(CINDER_LIBRARY "${CMAKE_SOURCE_DIR}/vendor/Cinder/lib/libcinder.a")
list(APPEND LINK_LIBS ${CINDER_LIBRARY})

# Link with other libs
macro(link_lib libName)
    unset(LIBRARY_TO_LINK CACHE)
    find_library(LIBRARY_TO_LINK ${libName})
    list(APPEND LINK_LIBS ${LIBRARY_TO_LINK})
endmacro()

link_lib(OpenGL)
link_lib(glfw3)

link_lib(Foundation)
link_lib(Cocoa)
link_lib(IOKit)

link_lib(CoreMedia)
link_lib(CoreAudio)
link_lib(AudioUnit)
link_lib(AudioToolbox)
link_lib(CoreVideo)
link_lib(IOSurface)
link_lib(AVFoundation)
link_lib(Accelerate)
