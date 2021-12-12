/**
 * (Math) 09: rotation * translation
 */

#include <functional>

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

auto* window = glab::setup(1200, 800, "glab");
Camera camera(window, glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -30.0f);

int main()
{
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  glab::Grid grid(100.0f);
  grid.setP(projection);

  glab::Tetragon tetra1(10.0f, 10.0f, 15.0f, 15.0f, 15.0f);
  tetra1.setP(projection);

  glab::Tetragon tetra2(10.0f, 10.0f, 30.0f, 15.0f, 15.0f);
  tetra2.setP(projection);

  glab::Tetragon tetra3(10.0f, 10.0f, 45.0f, 15.0f, 15.0f);
  tetra3.setP(projection);

  glab::Tetragon tetra4(10.0f, 10.0f, 60.0f, 15.0f, 15.0f);
  tetra4.setP(projection);

  glab::Tetragon tetra5(10.0f, 10.0f, 15.0f, 30.0f, 15.0f);
  tetra5.setP(projection);
  // tetra5.translate({-0.0f, -30.0f, -15.0f});
  // glab::debug(tetra5.model);
  // std::cout << "-----" << std::endl;
  // tetra5.rotateAround(90.0f, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f});
  // glab::debug(tetra5.model);
  // std::cout << "-----" << std::endl;
  // tetra5.translate({10.0f, 30.0f, 0.0f});
  // glab::debug(tetra5.model);
  // tetra5.translate({0.0f, 30.0f, 15.0f});

  glab::Tetragon tetra6(10.0f, 10.0f, 30.0f, 30.0f, 15.0f);
  tetra6.setP(projection);

  // glab::Tetragon tetra7(10.0f, 10.0f, 45.0f, 30.0f, 15.0f);
  // tetra7.setP(projection);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    float dt = clock.tick();
    camera.processInput(dt);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grid.setV(camera.GetViewMatrix());
    grid.draw();

    // Rotate on the spot
    tetra1.setV(camera.GetViewMatrix());
    tetra1.rotate(glm::radians(10.0f), {0.0f, 1.0f, 0.0f});
    tetra1.draw();
    tetra1.drawGuide();

    // Buggy rotation
    tetra2.setV(camera.GetViewMatrix());
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

    tetra2.model = rot * tetra2.model;
    tetra2.setM();
    tetra2.draw();
    tetra2.drawGuide();

    // Rotate on the spot
    tetra3.setV(camera.GetViewMatrix());
    tetra3.model = tetra3.model * rot;
    tetra3.setM();
    tetra3.draw();
    tetra3.drawGuide();

    // Rotate around the given point (y-axis)
    tetra4.setV(camera.GetViewMatrix());
    tetra4.translate({-5.0f, 0.0f, -5.0f});
    tetra4.model *= rot;
    tetra4.translate({5.0f, 0.0f, 5.0f});
    tetra4.setM();
    tetra4.draw();
    tetra4.drawGuide();

    // Rotate around the given point (freely)
    tetra5.setV(camera.GetViewMatrix());
    tetra5.translate({10.0f, 10.0f, 10.0f});
    tetra5.rotate(90.0f*dt, {1.0f, 1.0f, 0.0f});
    tetra5.translate({-10.0f, -10.0f, -10.0f});
    tetra5.draw();
    tetra5.drawGuide();

    tetra6.setV(camera.GetViewMatrix());
    tetra6.rotateAround(90.0f*dt, {0.0f, 1.0f, 1.0f}, {5.0f, 5.0f, 5.0f});
    tetra6.draw();
    tetra6.drawGuide();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
