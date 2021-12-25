/**
 * (Math) 12: Gimbal lock
 */

#include <functional>

#include <glm/gtx/euler_angles.hpp>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  glab::initWindow(/* Width */ 1200, /* Height*/ 800, "glab");

  Camera camera0 = Camera(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -35.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection;
  camera  |= camera0;

  glab::Grid grid(100.0f);

  // Gimbal lock occurs
  glab::Cube cube1(5.0f, 5.0f, 5.0f, 5.0f);
  glm::mat4 rotX(1.0f);
  glm::mat4 rotY = glm::eulerAngleY(glm::radians(90.0f));
  glm::mat4 rotZ(1.0f);

  glab::clock.start();
  while (!glfwWindowShouldClose(window))
  {
    float dt = glab::clock.tick();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.draw();

    rotX *= glm::eulerAngleX(glm::radians(10.0f));
    rotZ *= glm::eulerAngleZ(glm::radians(10.0f));
    cube1.rotation = rotX * rotY * rotZ;
    cube1.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
    glab::getKeyWASD();
  }
}
