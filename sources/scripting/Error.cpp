#include <iostream>

#include "Error.hpp"

namespace Scripting {
  Error::Error(duk_context *ctx) :
      name("Unknown"), message(""), lineNumber(-1), stack("") {
    if (duk_is_error(ctx, -1)) {
      name = parseName(ctx);
      message = parseMessage(ctx);
      lineNumber = parseLineNumber(ctx);
      stack = parseStack(ctx);
//      int errorCode = duk_get_error_code(ctx, -1);
//      if (errorCode > 0) {
//        switch (errorCode) {
//          case DUK_ERR_EVAL_ERROR:
//            break;
//          case DUK_ERR_RANGE_ERROR:
//            break;
//          case DUK_ERR_REFERENCE_ERROR:
//            break;
//          case DUK_ERR_SYNTAX_ERROR:
//            break;
//          case DUK_ERR_TYPE_ERROR:
//            break;
//          case DUK_ERR_URI_ERROR:
//            break;
//          case DUK_ERR_ERROR:
//            break;
//          default:
//            cerr << "Unknown error\n";
//            break;
//        }
//      }
    } else {
      cerr << "Unknown error\n";
    }
  }

  string Error::getName() {
    return name;
  }

  string Error::getMessage() {
    return message;
  }

  int Error::getLineNumber() {
    return lineNumber;
  }

  string Error::getStackTrace() {
    return stack;
  }

  string Error::parseName(duk_context *ctx) {
    duk_get_prop_string(ctx, -1, "name");
    return string(duk_to_string(ctx, -1));
  }

  string Error::parseMessage(duk_context* ctx) {
    duk_get_prop_string(ctx, -1, "message");
    return string(duk_to_string(ctx, -1));
  }

  int Error::parseLineNumber(duk_context* ctx) {
    duk_get_prop_string(ctx, -1, "name");
    int lineNum = duk_get_int(ctx, -1);
    duk_pop(ctx);
    return lineNum;
  }

  string Error::parseStack(duk_context* ctx) {
    duk_get_prop_string(ctx, -1, "stack");
    return string(duk_to_string(ctx, -1));
  }
}
