#shader vertex
#version 330 core

in vec2 position;
in vec2 textureCoord;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
}

#shader fragment
#version 330 core

out vec4 color;

void main()
{
    color = vec4(0.9f, 0.9f, 0.1f, 1.0f);
}
