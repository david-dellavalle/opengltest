#pragma once

class IndexBuffer
{
private:
	unsigned int m_RenderingID; // OpenGL needs a uint as an ID
	unsigned int m_Count; // How many indices this has
public:
	IndexBuffer(const unsigned int* data, const unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
