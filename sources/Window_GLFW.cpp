#include <GL/Window/Window.hpp>

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

namespace GL {
  Window::Window(uint width, uint height, const std::string &title, WindowStyle::window_style_t style) {
    context = new Context;

    open = !glfwWindowShouldClose(glfwWindow);

    int left, top, right, bottom;
    glfwGetWindowFrameSize(glfwWindow, &left, &top, &right, &bottom);
    this->x = left;
    this->y = top;
    this->width = static_cast<uint>(left + right);
    this->height = static_cast<uint>(top + bottom);
  }

  Window::~Window() {
    if (!open) return;

    if (context) delete context;

    Close();
  }

  int Window::GetX() {
    return x;
  }

  int Window::GetY() {
    return y;
  }

  uint Window::GetWidth() {
    return width;
  }

  uint Window::GetHeight() {
    return height;
  }

  bool Window::IsOpen() {
    int left, top, right, bottom;
    glfwGetWindowFrameSize(glfwWindow, &left, &top, &right, &bottom);
    this->x = left;
    this->y = top;
    this->width = static_cast<uint>(left + right);
    this->height = static_cast<uint>(top + bottom);

    return open;
  }

  bool Window::HasFocus() {
    return glfwGetWindowAttrib(glfwWindow, GLFW_FOCUSED) != 0;
  }

  void Window::SetPos(int x, int y) {
    if (!open) return;
    glfwSetWindowPos(glfwWindow, x, y);
  }

  void Window::SetSize(uint width, uint height) {
    if (!open) return;
    glfwSetWindowSize(glfwWindow, width, height);
  }

  void Window::SetTitle(const std::string &title) {
    if (!open) return;
    glfwSetWindowTitle(glfwWindow, title.c_str());
  }

  void Window::SetVisible(bool visible) {
    glfwMakeContextCurrent(glfwWindow);
  }

  bool Window::GetEvent(Event &ev) {
    glfwPollEvents();

    return false;
  }

  int Window::GetMouseX() {
    return 0;
  }

  int Window::GetMouseY() {
    return 0;
  }

  bool Window::IsMouseButtonDown(MouseButton::mouse_button_t button) {
    return false;
  }

  bool Window::IsKeyDown(Key::key_t key) {
    return false;
  }

  void Window::Present() {
    glfwSwapBuffers(glfwWindow);
  }

  void Window::Close() {
    glfwDestroyWindow(glfwWindow);

    open = false;
  }

  Context& Window::GetContext(uchar color, uchar depth, uchar stencil, uchar antialias) {
    if (context)
      return *context;
    else
      return *context = Context::UseExistingContext();
  }
}
