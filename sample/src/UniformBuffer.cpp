#include "UniformBuffer.h"
#include "Renderer.h"

namespace FluidEngine {
	UniformBuffer::UniformBuffer(const void* data, int size) : m_RenderID(0)
	{
		GL_CHECK_ERROR(glGenBuffers(1, &m_RenderID));
		GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, m_RenderID));
		GL_CHECK_ERROR(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW));
		GL_CHECK_ERROR(glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_RenderID));
	}

	UniformBuffer::~UniformBuffer() 
	{
		GL_CHECK_ERROR(glDeleteBuffers(1, &m_RenderID));
		//std::cout << "Uniform Buffer is deleted." << std::endl;
	}

	void UniformBuffer::Bind()
	{
		GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, m_RenderID));
	}

	void UniformBuffer::Unbind()
	{
		GL_CHECK_ERROR(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}

}