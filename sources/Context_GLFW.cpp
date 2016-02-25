#include <GL/GL/Context.hpp>

namespace GL {

  Context::Context() {
    owned = false;

    glGetIntegerv(GL_VIEWPORT, (GLint*)&defaultViewport);
  }

  Context::~Context() {
    if (!owned) return;
  }
}
