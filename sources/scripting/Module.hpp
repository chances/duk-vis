#pragma once

#include <string>
#include "duktape.h"
#include "semver/version.hpp"

using namespace std;

namespace Scripting {
  typedef enum UpdateType {
    RESIZE = 0,
    AUDIO = 1,
    RENDER = 2
  };

  class Module {
  public:
    Module(duk_context* ctx);

    string name;
    semver::version version;

    void update(UpdateType updateType);
    void render();
   private:
    duk_context* ctx;
  };
}
