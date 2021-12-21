/**
 * (Physics) 10: Orthographic projection (Play with different projection matrices by pressing "P")
 */

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  glab::initWindow(/* Width */ 1200, /* Height*/ 800, "glab");

  Camera camera0 = Camera(glm::vec3(25.0f, 25.0f, 25.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -35.0f);  

  // Each value (left/right/bottom/top) is decided under consideration of the measure in the eye space
  glm::mat4 projection0 = glm::ortho(-9.0f, 9.0f, -6.0f, 6.0f, 0.1f, 50.0f);
  camera0 |= projection0;

  // Specifying different aspect ratio from the one of display results in a squashed shape
  glm::mat4 projection1 = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 50.0f);
  camera0 |= projection1;

  // Normal perspective matrix
  glm::mat4 projection2 = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection2;

  camera  |= camera0;

  glab::Grid grid(50);

  glab::Cube cube(5.0f, 5.0f, 5.0f);

  glab::clock.start();
  while (!glfwWindowShouldClose(window))
  {
    glab::clock.tick();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.draw();

    cube.draw(/* needGuide */ true);

    glfwSwapBuffers(window);
    glfwPollEvents();
    glab::getKeyWASD();
  }
}
