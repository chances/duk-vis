#pragma once

#include <string>
#include "duktape.h"
#include "semver/version.hpp"

using namespace std;

namespace Scripting {
  typedef enum UpdateType {
    INIT = 0,
    RESIZE = 1,
    AUDIO = 2,
    RENDER = 3
  };

  class Module {
    public:
      Module(duk_context* ctx);

      string name;
      semver::version version;

      bool isReady();

      void update(UpdateType updateType);
      void render();
     private:
      duk_context* ctx;
      bool ready;

      string callUpdate(UpdateType updateType);
  };
}
