/**
 * (Physics) 11: Multiple cameras (Switch cameras by pressing numbers)
 */

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  glab::initWindow(/* Width */ 1200, /* Height*/ 800, "glab");

  Camera camera0 = Camera(glm::vec3( 15.0f, 15.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -35.0f);
  Camera camera1 = Camera(glm::vec3(-10.0f, 15.0f, 15.0f), glm::vec3(0.0f, 1.0f, 0.0f),  -45.0f, -35.0f);  

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection;
  camera1 |= projection;

  camera  |= camera0;
  camera  |= camera1;

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
