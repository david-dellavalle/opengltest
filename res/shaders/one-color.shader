#shader vertex
#version 330 core

in vec2 position;

uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(position.x, position.y, 0.0f, 1.0f);
}

#shader fragment
#version 330 core

uniform vec4 u_Color;

out vec4 out_Color;

void main()
{
	out_Color = u_Color;
}
