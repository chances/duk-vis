#include "Module.hpp"

namespace Scripting {
  Module::Module(duk_context *ctx) : ctx(ctx), version(""), ready(false) {
    string moduleEval =
      "({ name: null, version: null, render: null, update: null })";

    duk_push_global_object(ctx);

    if (duk_peval_string(ctx, moduleEval.c_str()) != 0) {
      cerr << "problem setting global object\n";
      duk_set_top(ctx, 0);

      return;
    }

    // Assign eval'd object to <global>.module

    try {
      if (!duk_put_prop_string(ctx, -2, "module")) {
        cerr << "problem setting <global>.module\n";
        duk_set_top(ctx, 0);

        return;
      }
    } catch (...) {
      cerr << "problem setting <global>.module\n";
      duk_set_top(ctx, 0);

      return;
    }

    // Pop off the global object
    duk_pop(ctx);

    ready = true;
  }

  bool Module::isReady() {
    return ready;
  }

  void Module::render() {

  }

  void Module::update(UpdateType updateType) {
    duk_push_global_object(ctx);

    if (duk_get_prop_string(ctx, -1, "module") && !duk_is_undefined(ctx, -1)) {
      string response;

      switch (updateType) {
        case UpdateType::INIT:
          // The script should have exec'd, get it's metadata
          if (duk_get_prop_string(ctx, -1, "name") && !duk_is_undefined(ctx, -1)) {
            name = string(duk_to_string(ctx, -1));
            duk_pop(ctx);
          }
          if (duk_get_prop_string(ctx, -1, "version") && !duk_is_undefined(ctx, -1)) {
            semver::version visVersion(string(duk_to_string(ctx, -1)));
            version = visVersion;
            duk_pop(ctx);
          }

          // Call the script's update function
          response = callUpdate(updateType);

//        if (duk_get_prop_string(ctx, -1, "module") && !duk_is_undefined(ctx, -1)) {
//          try {
//            duk_push_string(ctx, "")
//          } catch (...) {
//            cerr << "problem setting <global>.module.name\n";
//            cerr << "top: " << duk_get_top(ctx) << endl;
//
//            return;
//          }
//        } else { return; }

          break;
        case UpdateType::RESIZE:
          // Call the script's update function
          response = callUpdate(updateType);
          break;
        default:
          break;
      }

      // Pop off module property
      duk_pop(ctx);
    }


    // Pop off global object
    duk_pop(ctx);
  }

  string Module::callUpdate(UpdateType updateType) {
    if (duk_get_prop_string(ctx, -1, "update") && !duk_is_undefined(ctx, -1)) {
      if (duk_is_ecmascript_function(ctx, -1)) {
        // Bind the global object as 'this'
        duk_push_global_object(ctx);
        // Push updateType argument
        duk_push_number(ctx, (int) updateType);

        duk_call_method(ctx, 1);

        return string(duk_to_string(ctx, -1));
      }

      duk_pop(ctx);
    }

    return NULL;
  }
}
