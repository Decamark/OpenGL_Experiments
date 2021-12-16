/**
 * (Misc) 04: Take a picture
 */

#include <glab/glab.hpp>
#include <glab/shape.hpp>
#include <glab/grid.hpp>
#include <glab/time.hpp>

#include <learnopengl/camera.h>

auto* window = glab::setup(1200, 800, "glab");
Camera camera(window, glm::vec3(25.0f, 25.0f, 25.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -35.0f);

int main()
{
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

  glab::Grid grid(50);
  grid.setP(projection);

  glab::Cube cube(5.0f, 5.0f, 5.0f);
  cube.setP(projection);

  glab::Clock clock;
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.processInput(clock.tick());
    std::cout << clock.t << std::endl;

    cube.setV(camera.GetViewMatrix());
    cube.draw();
    cube.drawGuide();

    grid.setV(camera.GetViewMatrix());
    grid.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glab::Line line(
  //   { 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  //     0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f
  //   });
  // line.setP(glm::mat4(1.0f));
  // line.setV(glm::mat4(1.0f));
  
  // glab::Trianglab tri(
  //   {  0.0f,  1.0f, 0.0f,
  //     -1.0f, -1.0f, 0.0f,
  //      1.0f, -1.0f, 0.0f
  //   });

  // while (!glfwWindowShouldClose(window))
  // {

  // float cframe = glfwGetTime();
  // delta = cframe - lframe;
  // lframe = cframe;

  // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  // glClear(GL_COLOR_BUFFER_BIT);

  // glm::mat4 model      = glm::mat4(1.0f);
  // glm::mat4 view       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
  // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  // shader.setMat4("model", model);
  // shader.setMat4("view", view);
  // shader.setMat4("projection", projection);

  // line.draw();
  // glFlush();
  // tri.draw();

  // usleep(1000000);

  // glfwSwapBuffers(window);
  // glfwPollEvents();    

  // glab::export_to_png(window, "./coord_tex.png");
  //   }
}
