#pragma once

class VertexBuffer
{
private:
	unsigned int m_RenderingID; // OpenGL needs a uint as an ID
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
