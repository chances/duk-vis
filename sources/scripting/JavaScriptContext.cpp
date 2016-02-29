#include <iostream>
#include "duktape.h"

#include "JavaScriptContext.hpp"
#include "Error.hpp"

using namespace std;

Scripting::JavaScriptContext* jsContext = NULL;

void handleFatalDuktapeError_wrap(duk_context *ctx, duk_errcode_t code, const char *msg) {
  jsContext->handleFatalDuktapeError(ctx, code, msg);
}

namespace Scripting {
  JavaScriptContext::JavaScriptContext() {
    ctx = NULL;
    currentInterface = NULL;
  }
  JavaScriptContext::JavaScriptContext(fs::path initialScript) : JavaScriptContext() {
    ctx = duk_create_heap(NULL, NULL, NULL, NULL, handleFatalDuktapeError_wrap);
    if (!ctx || !Sandbox::createSandbox(ctx)) {
      cerr << "Failed to recreate a Duktape context.\n";

      ctx = NULL;

      return;
    }

    makeCurrent();

    auto initial = interface::loadSourceFromFile(initialScript);
    bindInterface(&initial);
  }

  JavaScriptContext::~JavaScriptContext() {
    if (ctx != NULL)
      duk_destroy_heap(ctx);
  }

  bool JavaScriptContext::isValid() {
    return ctx != NULL;
  }

  bool JavaScriptContext::isCurrent() {
    return jsContext == this;
  }

  void JavaScriptContext::makeCurrent() {
    // TODO: Actually lock around the jsContext?
    jsContext = this;
  }

  bool JavaScriptContext::bindInterface(interface *interfaceToBind) {
    if (ctx != NULL) {
      duk_destroy_heap(ctx);
      ctx = NULL;
    }

    ctx = duk_create_heap(NULL, NULL, NULL, NULL, handleFatalDuktapeError_wrap);
    if (!ctx || !Sandbox::createSandbox(ctx)) {
      cerr << "Failed to recreate a Duktape context.\n";

      return false;
    }

    makeCurrent();

    // Bind context to a new Module interface
    interfaceToBind->module = new Module(ctx);
    if (!interfaceToBind->module->isReady()) {
      cerr << "Failed to bind Module to context.\n";

      return false;
    }

    // TODO: Bind Screen and Graphics interfaces to context

    if (duk_peval_string(ctx, interfaceToBind->source.c_str()) != 0) {
      cerr << "Failed to initialize program:\n";

      Error error(ctx);
      cerr << error.getName() << ": " << error.getMessage();
      cerr << " on line " << error.getLineNumber() << endl;
      cerr << error.getStackTrace() << endl;

      return false;
    }

    interfaceToBind->module->update(UpdateType::INIT);

    currentInterface = interfaceToBind;

    return true;
  }

  void JavaScriptContext::handleFatalDuktapeError(duk_context *ctx, duk_errcode_t code, const char *msg) {
    cerr << "Fatal error in JavaScript context\n";
    cerr << code << ": " << msg;

    // TODO: If error code is >= 100, it's JS error, not some duktape error; handle that
    // http://duktape.org/api.html#defines.5

    duk_dump_context_stderr(ctx);
    duk_destroy_heap(ctx);

    this->ctx = ctx = duk_create_heap(NULL, NULL, NULL, NULL, handleFatalDuktapeError_wrap);
    if (!ctx || !Sandbox::createSandbox(ctx)) {
      cerr << "Failed to recreate Duktape context.\n";
    }

    makeCurrent();
  }
}
