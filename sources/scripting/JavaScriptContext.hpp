#pragma once

#include <string>
#include <vector>
#include "duktape.h"
#include "cinder/Filesystem.h"

#include "Sandbox.hpp"
#include "Module.hpp"
#include "Screen.hpp"

using namespace std;
using namespace ci;

namespace Scripting {
  struct interface {
    fs::path path;
    string source;
    Module* module;
    Screen* screen;

    static interface loadSourceFromFile(fs::path path) {
      interface i;

      i.path = path;
      i.module = NULL;
      i.screen = NULL;

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
      JavaScriptContext(fs::path initialScript);
      ~JavaScriptContext();

      bool isValid();
      bool isCurrent();

      void makeCurrent();

      bool bindInterface(interface* interfaceToBind);

      void handleFatalDuktapeError(duk_context *ctx, duk_errcode_t code, const char *msg);
    private:
      duk_context* ctx;
      interface* currentInterface;
  };
}
