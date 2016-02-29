# Include Duktape

include_directories(vendor/duktape-1.4.0/src)

# Configure Duktape
add_definitions("-DDUK_OPT_CPP_EXCEPTIONS")

list(APPEND SOURCE_FILES
    vendor/duktape-1.4.0/src/duk_config.h
    vendor/duktape-1.4.0/src/duktape.c

    sources/scripting/JavaScriptContext.cpp
    sources/scripting/Error.cpp
    sources/scripting/Sandbox.cpp
    sources/scripting/Module.cpp
    sources/scripting/Screen.cpp
)

SET_SOURCE_FILES_PROPERTIES(vendor/duktape-1.4.0/src/duktape.c PROPERTIES LANGUAGE CXX)
