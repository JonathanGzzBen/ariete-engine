#include "ariete-engine/graphic_context.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <print>

static GraphicContext g_graphic_context = {.valid = false};

void window_size_callback(GLFWwindow *window, const int width,
                          const int height) {
  const auto window_state =
      static_cast<WindowState *>(glfwGetWindowUserPointer(window));
  window_state->width = width;
  window_state->height = height;
  glfwSetWindowSize(window, width, height);
  glViewport(0, 0, width, height);

  const auto projection_view_matrix =
      glm::ortho(0.0F, 1280.0F, 0.0F, static_cast<float>(height));
  program_set_uniform(*window_state->program_manager,
                      window_state->shader_program_handle,
                      "projection_view_matrix", projection_view_matrix);
}

// This function should only be called once per program
auto graphic_context_create(const GraphicContextConfig &config)
    -> GraphicContext {
  if (g_graphic_context.valid) {
    std::println(std::cerr, "Graphic context already exists");
    return g_graphic_context;
  }
  if (MAX_WINDOW_HINTS < config.glfw_window_hints_count) {
    std::println(std::cerr,
                 "Number of glfw window hints exceeds MAX_WINDOW_HINTS({})",
                 MAX_WINDOW_HINTS);
    return GraphicContext{.valid = false};
  }
  // Initialize glfw
  if (glfwInit() != GLFW_TRUE) {
    std::println(std::cerr, "Could not initialize glfw");
    return GraphicContext{.valid = false};
  }
  // Set glfw error callback
  glfwSetErrorCallback(config.glfw_error_callback);

  // Set glfw window hints
  for (size_t i = 0; i < config.glfw_window_hints_count; ++i) {
    glfwWindowHint(config.glfw_window_hints[i].key,
                   config.glfw_window_hints[i].value);
  }

  GLFWwindow *window =
      glfwCreateWindow(config.window_width, config.window_height,
                       config.window_title, nullptr, nullptr);
  if (window == nullptr) {
    std::println(std::cerr, "Could not create GLFW window");
    glfwTerminate();
    return GraphicContext{.valid = false};
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::println(std::cerr, "Could not initialize GLEW");
    glfwTerminate();
    return GraphicContext{.valid = false};
  }

  // Configure OpenGL debugging if Debug context was created
  if (config.gl_enable_debug) {
    int context_flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
    if ((context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0) {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(config.gl_debug_callback, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                            nullptr, GL_TRUE);
    }
  }

  glfwSetWindowSizeCallback(window, window_size_callback);

  const auto context = GraphicContext{.valid = true,
                                      .window = window,
                                      .window_width = config.window_width,
                                      .window_height = config.window_height};
  g_graphic_context = context;
  return g_graphic_context;
}

auto graphic_context_destroy(GraphicContext *graphic_context) -> void {
  if (!graphic_context->valid) {
    return;
  }
  glfwDestroyWindow(graphic_context->window);
  glfwTerminate();
  graphic_context->valid = false;
}
