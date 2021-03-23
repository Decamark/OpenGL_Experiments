/**
 * 04: Draw a line
 */

#include <gle/gle.hpp>
#include <gle/shape.hpp>
#include <learnopengl/shader_m.h>

int main()
{
  auto* window = gle::setup(800, 800, "Practice");

  Shader shader("exp/04.vs", "exp/04.fs");
  shader.use();

  gle::Line line(
    { 0.0f,  1.0f, 0.0f,
      0.0f, -1.0f, 0.0f
    });
  gle::Triangle tri(
    {  0.0f,  1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f
    });

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  float lframe = glfwGetTime();
  float delta = 0.0f;
  float v = 2.0f;
  glm::mat4 model = glm::mat4(1.0f);
  while (!glfwWindowShouldClose(window))
  {

    float cframe = glfwGetTime();
    delta = cframe - lframe;
    lframe = cframe;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    line.draw();
    // tri.draw();

    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
}
