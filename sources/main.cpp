#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <glm/glm.hpp>

using namespace ci;
using namespace ci::app;

int width = 800, height = 600;

class DukVisApp : public App {
  public:
    void draw() override;
};

void DukVisApp::draw() {
  gl::clear();
}

CINDER_APP( DukVisApp, RendererGl )
