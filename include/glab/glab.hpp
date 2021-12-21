#ifndef _GLAB_GLAB
#define _GLAB_GLAB

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/camera.h>

GLFWwindow* window;
Containable<Camera> camera;

namespace glab
{
  // glfw: whenever the window size changed (by OS or user resize) this callback function executes
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
  }

  // glfw: whenever the mouse moves, this callback is called
  void mouse_callback(GLFWwindow* window, double xpos, double ypos)
  {
    static bool firstMove = true;
    static float lastX, lastY;
    if (firstMove)
    {
      lastX = xpos;
      lastY = ypos;
      firstMove = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    (*camera).ProcessMouseMovement(xoffset, yoffset);
  }

  // glfw: whenever the mouse scroll wheel scrolls, this callback is called
  // ----------------------------------------------------------------------
  void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
  {
    (*camera).ProcessMouseScroll(yoffset);
  }

  // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
  void processInput(float dt)
  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
      std::time_t t = std::time(0);
      std::tm* now = std::localtime(&t);
      std::stringstream ss;
      ss << now->tm_year + 1900 << now->tm_mon + 1 << now->tm_mday << 'T'
         << now->tm_hour << now->tm_min << now->tm_sec << ".png";
      glab::export_to_png(window, ss.str().c_str());
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      (*camera).ProcessKeyboard(FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      (*camera).ProcessKeyboard(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      (*camera).ProcessKeyboard(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      (*camera).ProcessKeyboard(RIGHT, dt);
  }

  void initWindow(int width, int height, const char* name)
  {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      std::exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to hide our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
  }
}

#endif
