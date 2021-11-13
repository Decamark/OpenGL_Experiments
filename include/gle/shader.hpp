#ifndef _GLE_SHADER
#define _GLE_SHADER

#include <iostream>
#include <learnopengl/shader_m.h>

Shader GLE_SHADER_3D_COLOR;
Shader GLE_SHADER_3D_TEXTURE;

void buildShaders()
{
  GLE_SHADER_3D_COLOR = Shader("shaders/GLE_SHADER_3D_COLOR.vs", "shaders/GLE_SHADER_3D_COLOR.fs");
}

#endif
