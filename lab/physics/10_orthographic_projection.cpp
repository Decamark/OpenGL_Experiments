/**
 * (Physics) 10: Orthographic projection
 */

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

#include <learnopengl/camera.h>

auto* window = glab::setup(1200, 800, "glab");
Camera camera(window, glm::vec3(25.0f, 25.0f, 25.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -35.0f);

int main()
{
  // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  // Each value (left/right/bottom/top) is decided under consideration of the measure in the eye space
  glm::mat4 projection = glm::ortho(-9.0f, 9.0f, -6.0f, 6.0f, 0.1f, 50.0f);
  // Specifying different aspect ratio from the one of display results in a squashed shape
  // glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 50.0f);

  glab::Grid grid(50);
  grid.setP(projection);

  glab::Cube cube(5.0f, 5.0f, 5.0f);
  cube.setP(projection);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.processInput(clock.tick());
    std::cout << clock.t << std::endl;

    cube.setV(camera.GetViewMatrix());
    cube.draw();
    cube.drawGuide();

    grid.setV(camera.GetViewMatrix());
    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
