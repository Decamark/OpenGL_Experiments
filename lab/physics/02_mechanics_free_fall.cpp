/**
 * (Physics > Mechanics) Simulation of free fall
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  glab::initWindow(/* Width */ 1200, /* Height*/ 800, "glab");

  Camera camera0 = Camera(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -45.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection;
  camera  |= camera0;

  glab::Grid grid(100.0f);

  glab::Cube cube1(10.0f, 100.0f, 10.0f, 5.0f);
  glab::Cube cube2(20.0f, 100.0f, 10.0f, 5.0f);

  glab::clock.start();
  while (!glfwWindowShouldClose(window))
  {
    float dt = glab::clock.tick();
    std::cout << glab::clock.t << std::endl;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Precise simulation
    std::function<void(double)> motion = [&cube1](double t) {
      float h = cube1.y0-9.8/2*t*t;
      if (h >= 0)
        cube1.setPos(cube1.x0,    h, cube1.z0);
      else
        cube1.setPos(cube1.x0, 0.0f, cube1.z0);
    };
    cube1.move(motion, glab::clock.t);
    cube1.draw(/* needGuide */ true);

    // Assume that the instantaneous velocity is constant for dt
    if (cube2.getPos().y >= 0)
      cube2.translate({0.0f, -9.8f*glab::clock.t*dt, 0.0f});
    cube2.draw(/* needGuide */ true);

    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
    glab::getKeyWASD();
  }
}
