#include <iostream>
#include <string>
#include "duktape.h"

#include "JavaScriptContext.hpp"

using namespace std;

Scripting::JavaScriptContext* jsContext = NULL;

void handleFatalDuktapeError_wrap(duk_context *ctx, duk_errcode_t code, const char *msg) {
  jsContext->handleFatalDuktapeError(ctx, code, msg);
}

namespace Scripting {
  JavaScriptContext::JavaScriptContext() {
    ctx = duk_create_heap(NULL, NULL, NULL, NULL, handleFatalDuktapeError_wrap);
    if (!ctx) {
      cerr << "Failed to create a Duktape context.\n";
    }

    makeCurrent();
  }

  JavaScriptContext::~JavaScriptContext() {
    if (ctx != NULL)
      duk_destroy_heap(ctx);
  }

  bool JavaScriptContext::isValid() {
    return ctx != NULL;
  }

  void JavaScriptContext::makeCurrent() {
    jsContext = this;
  }

  void JavaScriptContext::handleFatalDuktapeError(duk_context *ctx, duk_errcode_t code, const char *msg) {
    cerr << "Fatal error in JavaScript context\n";
    cerr << code << ": " << msg;

    // TODO: If error code is >= 100, it's JS error, not some duktape error; handle that

    duk_dump_context_stderr(ctx);
    duk_destroy_heap(ctx);

    this->ctx = ctx = duk_create_heap(NULL, NULL, NULL, NULL, handleFatalDuktapeError_wrap);
    if (!ctx) {
      cerr << "Failed to recreate Duktape context.\n";
    }
  }
}
