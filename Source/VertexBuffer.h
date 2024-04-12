#pragma once
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class VertexBuffer
{
private:
	GLuint m_RendererID;

public:
	VertexBuffer(const void* data, GLuint size);
	~VertexBuffer();

	void Bind();
	void Unbind();
};
#endif