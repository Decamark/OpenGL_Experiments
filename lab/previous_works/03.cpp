
/**
 * 03: Linear motion
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void bind_tri(unsigned int* vbo, unsigned int* vao, float vertices[])
{
  glGenBuffers(1, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
  glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, vao);
  glBindVertexArray(*vao);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
}

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(800, 600, "Practice", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  char log[512];
  int success;
  const char* vss =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "}\n";
  unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vss, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vs, 512, NULL, log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
  }

  const char* fss =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";
  unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fss, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fs, 512, NULL, log);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log << std::endl;
  }

  unsigned int prog = glCreateProgram();
  glAttachShader(prog, vs);
  glAttachShader(prog, fs);
  glLinkProgram(prog);
  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(prog, 512, NULL, log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);

  Shader coord_shader("exp/03_coord_shader.vs", "exp/03_coord_shader.fs");

  float vertices[] = {
    // back
    -0.5,  0.5, -0.5,
     0.5,  0.5, -0.5,
     0.5, -0.5, -0.5,
    -0.5, -0.5, -0.5,
    // front
    -0.5,  0.5,  0.5,
     0.5,  0.5,  0.5,
     0.5, -0.5,  0.5,
    -0.5, -0.5,  0.5,
    // left 
    -0.5,  0.5, -0.5,
    -0.5,  0.5,  0.5,
    -0.5, -0.5,  0.5,
    -0.5, -0.5, -0.5,
    // right
     0.5,  0.5,  0.5,
     0.5,  0.5, -0.5,
     0.5, -0.5, -0.5,
     0.5, -0.5,  0.5,
    // top
    -0.5,  0.5, -0.5,
     0.5,  0.5, -0.5,
     0.5,  0.5,  0.5,
    -0.5,  0.5,  0.5,
    // bottom
    -0.5, -0.5, -0.5,
     0.5, -0.5, -0.5,
     0.5, -0.5,  0.5,
    -0.5, -0.5,  0.5
  };
  unsigned int indices[] = {
     0,  1,  2,
     0,  2,  3,
     4,  5,  6,
     4,  6,  7,
     8,  9, 10,
     8, 10, 11,
    12, 13, 14,
    12, 14, 15,
    16, 17 ,18,
    16, 18, 19,
    20, 21, 22,
    20, 22, 23
  };

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // std::cout << sizeof(vertices) << std::endl;

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  float tri_vs[] = {
    0.0,  0.0, 0.0,
   -0.5, -0.5, 0.0,
    0.5, -0.5, 0.0
  };
  unsigned int tri_vbo, tri_vao;
  bind_tri(&tri_vbo, &tri_vao, tri_vs);
    // std::cout << tri_vao << std::endl;

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

    glUseProgram(prog);
    // create transformations
    // glm::mat4 model      = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(v*delta, 0.0f, 0.0f));
    // glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    unsigned int mloc = glGetUniformLocation(prog, "model");
    unsigned int vloc = glGetUniformLocation(prog, "view");
    unsigned int ploc = glGetUniformLocation(prog, "projection");
    glUniformMatrix4fv(mloc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(vloc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(ploc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    coord_shader.use();
    coord_shader.setMat4("projection", projection);
    coord_shader.setMat4("view", view);
    for (float x=0.0f; x < 10.0f; x++)
    {
      auto coord_model = glm::translate(glm::mat4(1.0f), glm::vec3(x, -1.0f, 0.0f));
      coord_shader.setMat4("model", coord_model);
      glBindVertexArray(tri_vao);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      // std::cout << glGetError() << std::endl;
    }

    glfwSwapBuffers(window);
    glfwPollEvents();    
  }
}
