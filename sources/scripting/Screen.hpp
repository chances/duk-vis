#pragma once

#include <string>
#include <iostream>
#include <glm/vec2.hpp>
#include "duktape.h"

using namespace std;

namespace Scripting {
  class Screen {
    public:
      Screen(duk_context* ctx, glm::ivec2 size);
      Screen(duk_context* ctx, int width, int height);

      bool isReady();

      bool setSize(glm::ivec2 size);
      bool setSize(int width, int height);
    private:
      duk_context* ctx;
      bool ready;

      int width;
      int height;

      bool setSize();
  };
}
