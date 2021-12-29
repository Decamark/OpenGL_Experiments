/**
 * (Physics > Mechanics) Westerlies
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  glab::initWindow(/* Width */ 1200, /* Height*/ 800, "glab", true, false);

  Camera camera0 = Camera(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection;
  camera  |= camera0;

  glab::Grid grid(100.0f);

  glab::Sphere earth(0, 0, 0, 10, 25, 25);
  glab::Sphere  ball(0, 0, 10, 1, 25, 25);

  glab::clock.start();
  while (!glfwWindowShouldClose(window))
  {
    float dt = glab::clock.tick();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    (*camera).Position =
      glm::rotate(glm::mat4(1.0f), glm::radians(30.0f*dt), {0.0f, 1.0f, 0.0f}) *
      glm::vec4((*camera).Position, 1.0f);
    (*camera).Front = -(*camera).Position;

    earth.rotate(30.0f*dt, {0.0f, 1.0f, 0.0f});
    earth.draw();

    ball.rotateAround(30.0f*dt, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
    ball.draw();

    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
    glab::getKeyWASD();
  }
}
