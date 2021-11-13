/**
 * 07: Time
 */

#include <functional>

#include <gle/gle.hpp>
#include <gle/export.hpp>
#include <gle/shape.hpp>
#include <gle/coord.hpp>
#include <gle/time.hpp>
// #include <gle/shader.hpp>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

auto* window = gle::setup(800, 600, "Practice");
Camera camera(window, glm::vec3(25.0f, 25.0f, 25.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -45.0f);

int main()
{
  Shader shader("exp/07.vs", "exp/07.fs");
  shader.use();
  // Shader shader = GLE_SHADER_3D_COLOR;
  // shader.use();

  gle::Cartesian system(&shader, 50);

  gle::Cube cube;
  cube.translate({5.0f, 5.0f, 5.0f});

  gle::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    camera.processInput(clock.tick());
    std::cout << clock.t << std::endl;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    cube.shader->setMat4("view", camera.GetViewMatrix());
    cube.shader->setMat4("projection", projection);

    std::function<void(double)> motion =
      [&cube](double t) {
        cube.translate({0.01f, glm::sin(t)*0.1, 0.01f});
      };
    cube.move(motion, clock.t);
    cube.draw();

    system.draw();
    system.draw_guide({cube.model[3][0], cube.model[3][1], cube.model[3][2]});

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
