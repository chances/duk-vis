# Include Duktape

include_directories(vendor/duktape-1.4.0/src)

list(APPEND SOURCE_FILES
    vendor/duktape-1.4.0/src/duk_config.h
    vendor/duktape-1.4.0/src/duktape.c

    sources/scripting/JavaScriptContext.cpp
    sources/scripting/Module.cpp
)
