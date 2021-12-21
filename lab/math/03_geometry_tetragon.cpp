/**
 * (Math) 03: Tetragon
 */

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  Camera camera0 = Camera(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -35.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection;
  camera  |= camera0;

  glab::initWindow(/* Width */ 1200, /* Height*/ 800, "glab");

  glab::Grid grid(100.0f);

  glab::Tetragon tetra(5.0f, 5.0f, 5.0f, 5.0f, 5.0f, "resources/apple.png");
  // glab::Tetragon tetra(5.0f, 5.0f, 5.0f, 5.0f, 5.0f);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    float dt = clock.tick();
    glab::processInput(dt);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.draw();

    tetra.draw(/* needGuide = */ true);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
