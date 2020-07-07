#include "ShaderControler.h"

namespace FluidEngine {

	ShaderControler::ShaderControler()
	{

	}

	ShaderControler::~ShaderControler()
	{
		GL_CHECK_ERROR(glDeleteProgram(m_RenderID));
	}

	unsigned int ShaderControler::LoadCompileShader(Shader shader)
	{
		unsigned int result = 0;
		FILE* fp;
		size_t filesize;
		char* data;
		fp = fopen(shader.filename, "rb");
		if (!fp)
			return 0;
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		data = new char[filesize + 1];
		if (!data)
			return 0;
		fread(data, 1, filesize, fp);
		data[filesize] = 0;
		fclose(fp);
		result = glCreateShader(shader.shaderType);
		if (!result)
			return 0;
		GL_CHECK_ERROR(glShaderSource(result, 1, &data, NULL));
		delete[] data;
		GL_CHECK_ERROR(glCompileShader(result));
		if (shader.checkError)
		{
			int status = 0;
			GL_CHECK_ERROR(glGetShaderiv(result, GL_COMPILE_STATUS, &status));
			if (status == GL_FALSE)
			{
				int length;
				GL_CHECK_ERROR(glGetShaderiv(result, GL_INFO_LOG_LENGTH, &length));
				char* message = (char*)alloca(length * sizeof(char));
				GL_CHECK_ERROR(glGetShaderInfoLog(result, length, &length, message));
				std::cout << "failed to compile a shader !" << std::endl;
				std::cout << message << std::endl;
				GL_CHECK_ERROR(glDeleteShader(result));
				return 0;
			}
		}
		return result;
	}

	void ShaderControler::AddShader(Shader shader)
	{
		shaders.push_back(LoadCompileShader(shader));
	}

	void ShaderControler::CreateShaderProgram()
	{
		m_RenderID = glCreateProgram();
		for (unsigned int shader : shaders) 
		{
			GL_CHECK_ERROR(glAttachShader(m_RenderID, shader));
		}
		GL_CHECK_ERROR(glLinkProgram(m_RenderID));
		GL_CHECK_ERROR(glValidateProgram(m_RenderID));
		for (unsigned int shader : shaders)
		{
			GL_CHECK_ERROR(glDeleteShader(shader));
		}
	}

	void ShaderControler::UseShaderProgram()
	{
		GL_CHECK_ERROR(glUseProgram(m_RenderID));
	}
}