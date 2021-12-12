/**
 * (Math) 02: Draw a triangle
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

auto* window = glab::setup(1200, 800, "glab");
Camera camera(window, glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -35.0f);

int main()
{
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  glab::Grid grid(100.0f);
  grid.setP(projection);

  glab::Triangle tri(5.0f, 5.0f, 5.0f, 5.0f);
  tri.setP(projection);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    float dt = clock.tick();
    camera.processInput(dt);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.setV(camera.GetViewMatrix());
    grid.draw();

    tri.setV(camera.GetViewMatrix());
    tri.draw();
    tri.drawGuide();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
