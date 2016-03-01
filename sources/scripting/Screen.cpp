#include "Screen.hpp"

namespace Scripting {
  Screen::Screen(duk_context *ctx, glm::ivec2 size) : Screen::Screen(ctx, size.x, size.y) {}
  Screen::Screen(duk_context *ctx, int width, int height) : ctx(ctx),
      width(width), height(height)
  {
    string screenEval =
      "({ width: null, height: null, graphics: null })";

    duk_push_global_object(ctx);

    if (duk_peval_string(ctx, screenEval.c_str()) != 0) {
      cerr << "problem evaluating screen shell\n";
      duk_set_top(ctx, 0);

      return;
    }

    // Assign object to <global>.screen

    try {
      if (!duk_put_prop_string(ctx, -2, "screen")) {
        cerr << "problem setting <global>.screen\n";
        duk_set_top(ctx, 0);

        return;
      }
    } catch (...) {
      cerr << "problem setting <global>.screen\n";
      duk_set_top(ctx, 0);

      return;
    }

    // Pop off the global object
    duk_pop(ctx);

    // Assign width and height to screen object

    ready &= setSize();

    ready = true;
  }

  bool Screen::isReady() {
    return ready;
  }

  bool Screen::setSize(glm::ivec2 size) {
    this->width = size.x;
    this->height = size.y;

    return setSize();
  }

  bool Screen::setSize(int width, int height) {
    this->width = width;
    this->height = height;

    return setSize();
  }

  bool Screen::setSize() {
    // TODO: Abstract assignment, this is messy...

    duk_push_global_object(ctx);

    if (duk_get_prop_string(ctx, -1, "screen") && !duk_is_undefined(ctx, -1)) {
      try {
        duk_push_number(ctx, width);
        if (!duk_put_prop_string(ctx, -2, "width")) {
          cerr << "problem setting <global>.screen.width\n";
          duk_set_top(ctx, 0);

          return false;
        }
      } catch (...) {
        cerr << "problem setting <global>.screen.width\n";
        duk_set_top(ctx, 0);

        return false;
      }

      try {
        duk_push_number(ctx, height);
        if (!duk_put_prop_string(ctx, -2, "height")) {
          cerr << "problem setting <global>.screen.height\n";
          duk_set_top(ctx, 0);

          return false;
        }
      } catch (...) {
        cerr << "problem setting <global>.screen.height\n";
        duk_set_top(ctx, 0);

        return false;
      }
    }

    // Pop off the global object
    duk_pop(ctx);

    return true;
  }
}
