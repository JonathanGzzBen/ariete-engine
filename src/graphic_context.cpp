#include "ariete-engine/graphic_context.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include <print>

static GraphicContext *g_graphic_context = nullptr;

void window_size_callback(GLFWwindow *window, const int new_width,
                          const int new_height) {
  const auto window_state =
      static_cast<GraphicContext *>(glfwGetWindowUserPointer(window));

  const float target_aspect_ratio =
      window_state->virtual_width / window_state->virtual_height;

  int viewport_width = new_width;
  int viewport_height =
      static_cast<int>(new_width / target_aspect_ratio + 0.5F);
  if (viewport_height > new_height) {
    // Window is too wide - pillarbox
    viewport_height = new_height;
    viewport_width = viewport_height * target_aspect_ratio + 0.5F;
  }

  const int viewport_x = (new_width / 2) - (viewport_width / 2);
  const int viewport_y = (new_height / 2) - (viewport_height / 2);

  glViewport(viewport_x, viewport_y, viewport_width, viewport_height);
}

// This function should only be called once per program
auto graphic_context_create(const GraphicContextConfig &config)
    -> GraphicContext {
  if (g_graphic_context != nullptr && g_graphic_context->valid) {
    std::println(std::cerr, "Graphic context already exists");
    return *g_graphic_context;
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

  GLFWwindow *window = glfwCreateWindow(config.initial_window_width,
                                        config.initial_window_height,
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

  const auto context =
      new GraphicContext{.valid = true,
                         .window = window,
                         .virtual_width = config.virtual_width,
                         .virtual_height = config.virtual_height};
  g_graphic_context = context;
  glfwSetWindowUserPointer(window, context);
  return *g_graphic_context;
}

auto graphic_context_destroy(GraphicContext *graphic_context) -> void {
  if (!graphic_context->valid) {
    return;
  }
  glfwDestroyWindow(graphic_context->window);
  glfwTerminate();
  graphic_context->valid = false;
  delete g_graphic_context;
}
