/**
 * (Math) 02: Draw a triangle
 */

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  Camera* camera = new Camera(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -35.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  // Initial setup
  glab::setup(/* Width */ 1200, /* Height*/ 800, "glab", camera, projection);

  glab::Grid grid(100.0f);

  glab::Triangle tri(5.0f, 5.0f, 5.0f, 5.0f);
  glab::debug(tri.model);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    float dt = clock.tick();
    glab::processInput(dt);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tri.draw(/* needGuide = */ true);

    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
