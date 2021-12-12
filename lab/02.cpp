
/**
 * 02: Make a cube rotate with mouse input
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/norm.hpp>

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

glm::vec3 front = {0.0f, 0.0f, 1.0f};
glm::vec3 right = {1.0f, 0.0f, 0.0f};
glm::vec3    up = {0.0f, 1.0f, 0.0f};

double px, py;
double qx, qy;

// glm::vec3 p(0.0f), q(0.0f);
glm::vec3 axis(1.0f);
float theta = 0.0f;
// model = glm::rotate(model, 0.633081f, glm::vec3(0.999971,0.00760625,0));
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button != GLFW_MOUSE_BUTTON_LEFT) return;

  if (action == GLFW_PRESS)
    glfwGetCursorPos(window, &px, &py);
  else if (action == GLFW_RELEASE) {
    glfwGetCursorPos(window, &qx, &qy);

    // std::cout << qx-px << std::endl;
    // std::cout << qy-py << std::endl;
    auto move = right * (float) (qx-px) + up * (float) -(qy-py);

    // This is a guard
    // glm::normalize() returns (nan,nan,nan) when provided vectors were (0, 0, 0)
    if (move == glm::vec3(0.0f)) return;

    axis = glm::normalize(glm::cross(move, front));
    // axis.y = -axis.y;

    theta = -glm::atan(glm::l2Norm(move) / (2*400));
    
    // theta = -0.01;
  }
}

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
  glfwSetMouseButtonCallback(window, mouse_callback);

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
  glUseProgram(prog);

  glDeleteShader(vs);
  glDeleteShader(fs);

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

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  auto model = glm::mat4(1.0f);
  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::cout << "[theta] " << theta << std::endl;
    std::cout << "[axis]  " << '(' << axis.x << ',' << axis.y << ',' << axis.z << ')' << std::endl;
    model = glm::rotate(model, theta, axis);
    front = glm::rotate(front, -theta, axis);
    right = glm::rotate(right, -theta, axis);
       up = glm::rotate(up, -theta, axis);

    // create transformations
    glm::mat4 view       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    unsigned int mloc = glGetUniformLocation(prog, "model");
    unsigned int vloc = glGetUniformLocation(prog, "view");
    unsigned int ploc = glGetUniformLocation(prog, "projection");
    glUniformMatrix4fv(mloc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(vloc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(ploc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
