#include "Sandbox.hpp"

namespace Scripting {
  bool Sandbox::createSandbox(duk_context* ctx) {
    string globalPassthroughEval =
      "({ Math: this.Math, JSON: this.JSON, console: new this.Duktape.Logger(null),"
        "Object: this.Object, Function: this.Function, Boolean: this.Boolean,"
        "Number: this.Number, String: this.String, RegExp: this.RegExp,"
        "Array: this.Array, Date: this.Date,"
        "Error: this.Error, InternalError: this.InternalError,"
        "RangeError: this.RangeError, ReferenceError: this.ReferenceError,"
        "SyntaxError: this.SyntaxError, TypeError: this.TypeError,"
        "Infinity: this.Infinity, NaN: this.NaN, undefined: this.undefined,"
        "isFinite: this.isFinite, isNaN: this.isNaN,"
        "parseFloat: this.parseFloat, parseInt: this.parseInt,"
        "Duktape: {"
          "version: this.Duktape.version, env: this.Duktape.env,"
          "encode: this.Duktape.enc, enc: this.Duktape.enc,"
          "decode: this.Duktape.dec, dec: this.Duktape.dec,"
          "print: this.print, alert: this.alert"
        "}"
      "})";

    if (duk_peval_string(ctx, globalPassthroughEval.c_str()) != 0) {
      cerr << "problem setting global object\n";

      return false;
    }
    duk_set_global_object(ctx);

    duk_push_global_object(ctx);
    if (duk_get_prop_string(ctx, -1, "console") && !duk_is_undefined(ctx, -1)) {
      if (duk_get_prop_string(ctx, -2, "Duktape") && !duk_is_undefined(ctx, -1)) {
        if (duk_get_prop_string(ctx, -1, "print") && !duk_is_undefined(ctx, -1)) {
          try {
            if (duk_put_prop_string(ctx, -3, "print")) {
              duk_pop_n(ctx, 2);
            } else {
              cerr << "problem setting console.print\n";
              duk_set_top(ctx, 0);

              return false;
            }
          } catch (...) {
            cerr << "exception setting console.print\n";
            duk_set_top(ctx, 0);

            return false;
          }
        }
      }
    }

    if (duk_get_prop_string(ctx, -1, "console") && !duk_is_undefined(ctx, -1)) {
      if (duk_get_prop_string(ctx, -1, "info") && !duk_is_undefined(ctx, -1)) {
        try {
          if (duk_put_prop_string(ctx, -2, "log")) {
            duk_pop_n(ctx, 2);
          } else {
            cerr << "problem setting console.log\n";
            duk_set_top(ctx, 0);

            return false;
          }
        } catch (...) {
          cerr << "exception setting console.log\n";
          duk_set_top(ctx, 0);

          return false;
        }
      }
    }

    duk_push_global_object(ctx);

    // Restrict modification of important props

    sandboxProperty(ctx, "console", "raw");
    sandboxProperty(ctx, "console", "fmt");
    sandboxProperty(ctx, "console", "trace");
    sandboxProperty(ctx, "console", "debug");
    sandboxProperty(ctx, "console", "info");
    sandboxProperty(ctx, "console", "warn");
    sandboxProperty(ctx, "console", "error");
    sandboxProperty(ctx, "console", "fatal");
    sandboxProperty(ctx, "console", "print");
    sandboxProperty(ctx, "console", "log");

    sandboxProperty(ctx, "Duktape", "version");
    sandboxProperty(ctx, "Duktape", "env");
    sandboxProperty(ctx, "Duktape", "encode");
    sandboxProperty(ctx, "Duktape", "enc");
    sandboxProperty(ctx, "Duktape", "decode");
    sandboxProperty(ctx, "Duktape", "dec");

    // Delete duplicate props

    deleteProperty(ctx, "Duktape", "print");
    deleteProperty(ctx, "Duktape", "alert");

    // Alias the global object in its own property

    if (duk_peval_string_noresult(ctx, "this.dukvis = this") != 0) {
      cerr << "problem binding dukvis global\n";
      duk_set_top(ctx, 0);

      return false;
    }

    duk_pop(ctx);
//    cout << "top: " << duk_get_top(ctx) << endl;

    return true;
  }
}
