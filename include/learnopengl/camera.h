#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ctime>
#include <sstream>
#include <vector>

#include <glab/export.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  3.0f;
const float SENSITIVITY =  0.2f;
const float ZOOM        =  45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
  // camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // euler Angles
  float Yaw;
  float Pitch;
  // camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // constructor with vectors
  Camera(GLFWwindow* window,
         glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = YAW, float pitch = PITCH) : window(window),
                                                 Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                 MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
  {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
  }
  // constructor with scalar values
  Camera(GLFWwindow* window,
         float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : window(window),
      Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
  {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
  }

  // returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix()
  {
    return glm::lookAt(Position, Position + Front, Up);
  }

  // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
  void ProcessKeyboard(Camera_Movement direction, float deltaTime)
  {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
      Position += Front * velocity;
    if (direction == BACKWARD)
      Position -= Front * velocity;
    if (direction == LEFT)
      Position -= Right * velocity;
    if (direction == RIGHT)
      Position += Right * velocity;
  }

  // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
  {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
      if (Pitch > 89.0f)
        Pitch = 89.0f;
      if (Pitch < -89.0f)
        Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
  }

  // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset)
  {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
      Zoom = 1.0f;
    if (Zoom > 45.0f)
      Zoom = 45.0f;
  }

  // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
  // ---------------------------------------------------------------------------------------------------------
  void processInput(float dt)
  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
      std::time_t t = std::time(0);
      std::tm* now = std::localtime(&t);
      // std::string filename = ".png";
      std::stringstream ss;
      ss << now->tm_year + 1900 << now->tm_mon + 1 << now->tm_mday << 'T'
         << now->tm_hour << now->tm_min << now->tm_sec << ".png";
      glab::export_to_png(window, ss.str().c_str());
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      this->ProcessKeyboard(FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      this->ProcessKeyboard(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      this->ProcessKeyboard(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      this->ProcessKeyboard(RIGHT, dt);
  }

private:
  GLFWwindow* window;

  // calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors()
  {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
  }
};

extern Camera camera;

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}

#endif
