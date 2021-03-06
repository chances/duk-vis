#include <vector>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "scripting/JavaScriptContext.hpp"

using namespace ci;
using namespace ci::app;

typedef Scripting::interface visualization;

class DukVisApp : public AppMac {
  public:
    void setup() override;
    void resize() override;
    void draw() override;

    CameraPersp mCamera;
    gl::BatchRef mCube;

    gl::BatchRef mOverlay;

    Scripting::JavaScriptContext* mJSContext;
    vector<Scripting::interface> mVisualizations;
    int currentVisIndex = 0;
};

void DukVisApp::setup() {
  RendererRef renderer = getRenderer();

  fs::path dataPath = Platform::get()->getExecutablePath();
  dataPath /= fs::path("data");

  addAssetDirectory(dataPath);

  mJSContext = new Scripting::JavaScriptContext();
  auto vis = visualization::loadSourceFromFile(
    dataPath.append(std::string("js/spectrum.vis.js"))
  );
  if (mJSContext->bindInterface((&vis))) {
    cout << "Loaded visualization: " << vis.module->name << " v" << vis.module->version << endl;
    mVisualizations.push_back(vis);
  } else {
    cerr << "Could not create JS context, aborting...\n";
    exit(1);
  }

  auto lambert = gl::ShaderDef().lambert().color();
  gl::GlslProgRef shader = gl::getStockShader(lambert);
  mCube = gl::Batch::create(geom::Cube(), shader);

  auto overlayShader = gl::ShaderDef().color();
  shader = gl::getStockShader(overlayShader);
  mOverlay = gl::Batch::create(geom::Rect(Rectf(0, 0, 1, 1)), shader);

  mCamera.lookAt(vec3(5, 3, 2), vec3(0, 0, 0));

  gl::enableDepthRead();
  gl::enableDepthWrite();
}

void DukVisApp::resize() {
  if (mJSContext->isValid()) {
    if (!mJSContext->isCurrent())
      mJSContext->makeCurrent();

    if (!mVisualizations.at(currentVisIndex).screen->setSize(getWindowSize()))
      cerr << "Could not update screen size for scripts\n";

    mVisualizations.at(currentVisIndex).module->update(Scripting::UpdateType::RESIZE);
  }
}

void DukVisApp::draw() {
  gl::clear();

  gl::setMatrices(mCamera);

  float anim = getElapsedFrames() / getFrameRate();
  anim *= 0.3;

  gl::rotate(glm::radians(anim * 360), vec3(0, 1, 0));
  gl::color(Color(CM_RGB, 1, 0, 0));
  mCube->draw();

  auto windowSize = getWindowSize();
  gl::setMatricesWindow(windowSize);

  {
    gl::ScopedBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl::scale(windowSize.x, windowSize.y);
    gl::color(0.2, 0.2, 0.75, 0.25);
    mOverlay->draw();
  }
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
    settings->getDefaultWindowFormat().enableFullScreenButton();
    settings->setFrameRate(50.0f);
  }
)
