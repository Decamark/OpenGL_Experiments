/**
 * (Math) 09: rotation * translation
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

int main()
{
  glab::initWindow(/* Width */ 1200, /* Height*/ 800, "glab");

  Camera camera0 = Camera(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -30.0f);
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
  camera0 |= projection;
  camera  |= camera0;

  glab::Grid grid(100.0f);

  glab::Tetragon tetra1(15.0f, 15.0f, 15.0f, 10.0f, 10.0f);
  glab::Tetragon tetra2(30.0f, 15.0f, 15.0f, 10.0f, 10.0f);
  glab::Tetragon tetra3(45.0f, 15.0f, 15.0f, 10.0f, 10.0f);
  glab::Tetragon tetra4(60.0f, 15.0f, 15.0f, 10.0f, 10.0f);
  glab::Tetragon tetra5(15.0f, 30.0f, 15.0f, 10.0f, 10.0f);
  glab::Tetragon tetra6(30.0f, 30.0f, 15.0f, 10.0f, 10.0f);

  glab::clock.start();
  while (!glfwWindowShouldClose(window))
  {
    float dt = glab::clock.tick();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.draw();

    // Rotate on the spot
    tetra1.rotate(glm::radians(10.0f), {0.0f, 1.0f, 0.0f});
    tetra1.draw(/* needGuide = */ true);

    // This is the inverse of how GLM stores transformations
    // glm::mat4 rot = {
    //   glm::cos(glm::radians(10.0f*dt)), 0, glm::sin(glm::radians(10.0f*dt)), 0,
    //                                  0, 1,                                0, 0,
    //  -glm::sin(glm::radians(10.0f*dt)), 0, glm::cos(glm::radians(10.0f*dt)), 0,
    //                                  0, 0,                                0, 1
    // };
    glm::mat4 rot = {
      glm::cos(glm::radians(180.0f*dt)), 0, -glm::sin(glm::radians(180.0f*dt)), 0,
                                      0, 1,                                  0, 0,
      glm::sin(glm::radians(180.0f*dt)), 0,  glm::cos(glm::radians(180.0f*dt)), 0,
                                      0, 0,                                  0, 1
    };

    // Buggy rotation
    tetra2.model = rot * tetra2.model;
    tetra3.setM();
    tetra2.draw(/* needGuide = */ true);

    // Rotate on the spot
    tetra3.model = tetra3.model * rot;
    tetra3.setM();
    tetra3.draw(/* needGuide = */ true);

    // Rotate around the given point (y-axis)
    tetra4.translate({-5.0f, 0.0f, -5.0f});
    tetra4.model *= rot;
    tetra4.translate({5.0f, 0.0f, 5.0f});
    tetra4.setM();
    tetra4.draw(/* needGuide = */ true);

    // Rotate around the given point (freely)
    tetra5.translate({10.0f, 10.0f, 10.0f});
    tetra5.rotate(90.0f*dt, {1.0f, 1.0f, 0.0f});
    tetra5.translate({-10.0f, -10.0f, -10.0f});
    tetra5.draw(/* needGuide = */ true);

    tetra6.rotateAround(90.0f*dt, {0.0f, 1.0f, 1.0f}, {5.0f, 5.0f, 5.0f});
    tetra6.draw(/* needGuide = */ true);

    glfwSwapBuffers(window);
    glfwPollEvents();
    glab::getKeyWASD();
  }
}
