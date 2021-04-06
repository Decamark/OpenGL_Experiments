#ifndef _GLE_TIME
#define _GLE_TIME

namespace gle
{
  class Clock
  {
  public:
    float t;
    Clock() { glfwSetTime(0.0f); t = glfwGetTime(); }
    float tick()
    {
      float ct = glfwGetTime();
      float dt = ct - t;
      t = ct;
      return dt;
    }
  };
}

#endif
