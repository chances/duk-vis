#pragma once

#include <string>
#include "duktape.h"
#include "cinder/Filesystem.h"

#include "Module.hpp"

using namespace std;
using namespace ci;

namespace Scripting {
  struct interface {
    fs::path path;
    string source;
    Module* module;

    static interface loadSourceFromFile(fs::path path) {
      interface i;

      i.path = path;
      i.module = NULL;

      try {
        fs::load_string_file(path, i.source);
      } catch(...) {
        cerr << "Could not read file: " << path.string() << endl;

        i.source = string(NULL);
      }

      return i;
    }
  };

  class JavaScriptContext {
    public:
      JavaScriptContext();
      ~JavaScriptContext();

      bool isValid();
      void makeCurrent();

      void handleFatalDuktapeError(duk_context *ctx, duk_errcode_t code, const char *msg);
    private:
      duk_context* ctx;
  };
}
