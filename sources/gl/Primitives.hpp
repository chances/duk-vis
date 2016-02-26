#pragma once

#include <algorithm>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

using namespace std;

namespace Primitives {
  struct geometry {
    uint numIndices;
    float* vertices;
    geometry(float* verts) : vertices(verts), numIndices(0) {}
    geometry(float* verts, uint count) : vertices(verts), numIndices(count) {}
  };

  geometry* triangle() {
    float rawGeometry[] {
      -0.5f,  0.5f, 0.0f,
       0.5f,  0.5f, 0.0f,
       0.5f, -0.5f, 0.0f
    };
    uint count = ARRAY_SIZE(rawGeometry);
    float* geom = new float[count];
    std::copy(rawGeometry, rawGeometry + count, geom);

    return new geometry(geom, count);
  }

  geometry* cube() {
    float rawGeometry[] {
      -1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
       1.0f, 1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
      1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f, 1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f,
      1.0f,-1.0f, 1.0f, -1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f, -1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f,
      1.0f,-1.0f,-1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f,-1.0f,
      1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, 1.0f
    };
    uint count = ARRAY_SIZE(rawGeometry);
    float* geom = new float[count];
    std::copy(rawGeometry, rawGeometry + count, geom);

    return new geometry(geom, count);
  }
}
