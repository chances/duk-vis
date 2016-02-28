#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <glm/glm.hpp>

using namespace ci;
using namespace ci::app;

int width = 800, height = 600;

class DukVisApp : public AppMac {
  public:
    void setup() override;
    void draw() override;

    CameraPersp		mCamera;
    gl::BatchRef	mCube;
};

void DukVisApp::setup() {
  RendererRef renderer = getRenderer();

  auto lambert = gl::ShaderDef().lambert().color();
  gl::GlslProgRef shader = gl::getStockShader(lambert);
  mCube = gl::Batch::create(geom::Cube(), shader);

  mCamera.lookAt(vec3(5, 3, 2), vec3(0, 0, 0));

  gl::enableDepthRead();
  gl::enableDepthWrite();
}

void DukVisApp::draw() {
  gl::clear();

  gl::setMatrices(mCamera);

  float anim = getElapsedFrames() / getFrameRate();
  anim *= 0.3;

  gl::rotate(glm::radians(anim * 360), vec3(0, 1, 0));
  gl::color(Color(CM_RGB, 1, 0, 0));
  mCube->draw();

  vec2 windowSize = getWindowSize();
  gl::setMatricesWindow(windowSize.x, windowSize.y, true);

  // TODO: Render the 2D overlay
}

CINDER_APP(
  DukVisApp,

  RendererGl(
    RendererGl::Options().version(3, 3).coreProfile(true).msaa(4)
  ),

  []( App::Settings *settings ) {
    settings->setTitle(std::string("DukVis"));
    settings->setWindowSize(800, 600);
    settings->setResizable(true);
    settings->setFrameRate(60.0f);
  }
)
