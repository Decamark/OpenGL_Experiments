#version 330 core
in vec2 TexPos;
out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
  vec4 texColor = texture(texture0, TexPos);
  if (texColor.a < 1.0)
    discard;
  FragColor = texColor;
  // FragColor = texture(texture0, TexPos) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
