#ifndef _GLAB_TIME
#define _GLAB_TIME

namespace glab
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
