#pragma once

#include <string>
#include <map>
#include <vector>

#include "Element.hpp"
#include <GL/OOGL.hpp>

using namespace std;

namespace Textures {
  const GL::Color colorTransparent(0, 0, 0, 0);

  struct texture {
    GL::Image* img = NULL;
    GL::Texture* texture = NULL;
    void dispose() {
      delete texture;
      delete img;
    }
    void penLine(int y, const GL::Color color) {
      for (size_t x = 0; x < img->GetWidth(); x++) {
        img->SetPixel(x, y, color);
      }
      texture->Image2D(img->GetPixels(), GL::DataType::UnsignedByte, GL::Format::RGBA, img->GetWidth(), img->GetHeight(), GL::InternalFormat::RGBA);
    }
    void bind(uint unit) {
      glActiveTexture(GL_TEXTURE0 + unit);
  		glBindTexture(GL_TEXTURE_2D, *texture);
    }
  };

  typedef pair<const char*, texture*> namedTexture;

  texture* empty(ushort width, ushort height) {
    texture* tex = new texture;

    tex->img = new GL::Image(width, height, colorTransparent);
    tex->texture = new GL::Texture(*tex->img);

    return tex;
  }

  texture* filled(ushort width, ushort height, GL::Color& color) {
    texture* tex = new texture;

    tex->img = new GL::Image(width, height, color);
    tex->texture = new GL::Texture(*tex->img);

    return tex;
  }

  texture* fromFile(const std::string& filename) {
    texture* tex = new texture;

    tex->img = new GL::Image(filename);
    tex->texture = new GL::Texture(*tex->img);

    return tex;
  }
}
