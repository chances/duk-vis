#pragma once

#include <string>
#include <iostream>
#include "duktape.h"

using namespace std;

namespace Scripting {
  class Sandbox {
    public:
      static bool createSandbox(duk_context* ctx);
  };

  static bool sandboxProperty(duk_context* ctx, string prop) {
    bool reachedProp = false;
    try {
      duk_push_string(ctx, prop.c_str());
      reachedProp = true;
      duk_def_prop(ctx, -2,
                   DUK_DEFPROP_CLEAR_WRITABLE | DUK_DEFPROP_SET_ENUMERABLE |
                   DUK_DEFPROP_CLEAR_CONFIGURABLE | DUK_DEFPROP_FORCE
      );
      duk_pop(ctx);

      return true;
    } catch (...) {
      if (reachedProp)
        duk_pop(ctx);

      return false;
    }
  }

  static bool sandboxPropertyWritable(duk_context* ctx, string prop) {
    bool reachedProp = false;
    try {
      duk_push_string(ctx, prop.c_str());
      reachedProp = true;
      duk_def_prop(ctx, -2,
                   DUK_DEFPROP_SET_ENUMERABLE |
                   DUK_DEFPROP_CLEAR_CONFIGURABLE | DUK_DEFPROP_FORCE
      );
      duk_pop(ctx);

      return true;
    } catch (...) {
      if (reachedProp)
        duk_pop(ctx);

      return false;
    }
  }

  static bool sandboxProperty(duk_context* ctx, const char* object, const char* prop) {
    if (duk_get_prop_string(ctx, -1, object) && !duk_is_undefined(ctx, -1)) {
      return sandboxProperty(ctx, prop);
    }
  }

  static bool sandboxProperty(duk_context* ctx, const char* object, const char* childObj, const char* prop) {
    if (duk_get_prop_string(ctx, -1, object) && !duk_is_undefined(ctx, -1)) {
      if (duk_get_prop_string(ctx, -1, childObj) && !duk_is_undefined(ctx, -1)) {
        return sandboxProperty(ctx, prop);
      }
    }
  }

  static bool deleteProperty(duk_context* ctx, const char* prop) {
    duk_push_string(ctx, prop);
    return (duk_del_prop_string(ctx, -2, prop) != 0);
  }

  static bool deleteProperty(duk_context* ctx, const char* object, const char* prop) {
    bool reachedProp = false;

    if (duk_get_prop_string(ctx, -1, object) && !duk_is_undefined(ctx, -1)) {
      reachedProp = true;
      if (deleteProperty(ctx, prop)) {
        duk_pop_2(ctx);
        return true;
      }
    }

    if (!reachedProp)
      duk_pop(ctx);
    else
      duk_pop_2(ctx);

    cout << "problem deleting " << object << "." << prop << endl;
    cout << "top: " << duk_get_top(ctx) << endl;
    return false;
  }
}
