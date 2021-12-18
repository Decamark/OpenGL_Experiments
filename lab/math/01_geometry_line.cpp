/**
 * (Math) 01: Draw a line
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

#include <learnopengl/camera.h>

GLFWwindow* window = glab::setup(/* Width */ 1200, /* Height*/ 800, "glab");
Camera camera(window, glm::vec3(8.0f, 12.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -30.0f);

int main()
{
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  glab::Grid grid(100.0f);
  grid.setP(projection);

  glab::Line l(std::vector<float>({5, 5, 5}), {1, 0, 0}, {10, 10, 10}, {0, 1, 0});
  l.setP(projection);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    float dt = clock.tick();
    camera.processInput(dt);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.setV(camera.GetViewMatrix());
    grid.draw();

    l.setV(camera.GetViewMatrix());
    l.draw();
    l.drawGuide();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
