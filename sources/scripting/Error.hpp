#pragma once

#include <string>
#include "duktape.h"

using namespace std;

namespace Scripting {
  class Error {
    public:
      Error(duk_context* ctx);

      string getName();
      string getMessage();
      int getLineNumber();
      string getStackTrace();
    private:
      string name;
      string message;
      int lineNumber;
      string stack;

      static string parseName(duk_context* ctx);
      static string parseMessage(duk_context* ctx);
      static int parseLineNumber(duk_context* ctx);
      static string parseStack(duk_context* ctx);
  };
}
