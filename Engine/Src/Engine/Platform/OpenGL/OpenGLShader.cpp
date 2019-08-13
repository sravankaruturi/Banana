#include "eepch.h"

#include "OpenGLShader.h"

namespace ee::re
{

	void OpenGLShader::ReadShaderFromFile(const std::string& filePath)
	{
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (!in)
		{
			EE_CORE_WARN("Could not read the shader at file: {0}", filePath);
			return;
		}

		in.seekg(0, std::ios::end);
		m_ShaderSource.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&m_ShaderSource[0], m_ShaderSource.size());
		in.close();
	}

	void OpenGLShader::CompileAndUploadShader()
	{

		std::unordered_map<GLenum, std::string> shader_sources;

		const char* typeToken = "#type";

		size_t typeTokenLength = strlen(typeToken);

		size_t pos = m_ShaderSource.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			const size_t eol = m_ShaderSource.find_first_of("\r\n", pos);
			EE_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			const size_t begin = pos + typeTokenLength + 1;
			std::string type = m_ShaderSource.substr(begin, eol - begin);
			EE_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid Shader Type.");

			const size_t nextLinePos = m_ShaderSource.find_first_not_of("\r\n", eol);
			pos = m_ShaderSource.find(typeToken, nextLinePos);
			shader_sources[ShaderTypeFromString(type)] = m_ShaderSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? m_ShaderSource.size() - 1 : nextLinePos));
		}

		std::vector<GLuint> shaderRendererIDs;

		GLuint program;
		PE_GL(program = glCreateProgram());

		for (auto& kv : shader_sources) {

			GLenum type = kv.first;
			std::string& source = kv.second;

			GLuint shaderRendererId;
			PE_GL(shaderRendererId = glCreateShader(type));

			const GLchar* source_cstr = static_cast<const GLchar*>(source.c_str());
			PE_GL(glShaderSource(shaderRendererId, 1, &source_cstr, 0));
			PE_GL(glCompileShader(shaderRendererId));

			GLint isCompiled = 0;
			PE_GL(glGetShaderiv(shaderRendererId, GL_COMPILE_STATUS, &isCompiled));

			if (GL_FALSE == isCompiled){
				GLint maxLength = 0;
				PE_GL(glGetShaderiv(shaderRendererId, GL_INFO_LOG_LENGTH, &maxLength));

				std::vector<GLchar> infolog(maxLength);
				PE_GL(glGetShaderInfoLog(shaderRendererId, maxLength, &maxLength, &infolog[0]));
				EE_CORE_ERROR("Shader Compilation Failed: \n {0}", &infolog[0]);
				PE_GL(glDeleteShader(shaderRendererId));
				EE_CORE_ASSERT(false, "Failed");

			}

			shaderRendererIDs.push_back(shaderRendererId);
			PE_GL(glAttachShader(program, shaderRendererId));

		}

		PE_GL(glLinkProgram(program));
		GLint isLinked = 0;
		PE_GL(glGetProgramiv(program, GL_LINK_STATUS, (int*)& isLinked));

		if (GL_FALSE == isLinked) {
			GLint maxLength = 0;
			PE_GL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));

			std::vector<GLchar> infolog(maxLength);
			PE_GL(glGetProgramInfoLog(program, maxLength, &maxLength, &infolog[0]));
			EE_CORE_ERROR("Shader Compilation Failed:\n{0}", &infolog[0]);

			PE_GL(glDeleteProgram(program));

			for (auto it : shaderRendererIDs) {
				PE_GL(glDeleteShader(it));
			}
		}

		for (auto it : shaderRendererIDs) {
			PE_GL(glDetachShader(program, it));
		}
		
		m_RendererID = program;

	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		ReadShaderFromFile(filePath);
		EE_RENDER_S({
			self->CompileAndUploadShader();
			});
		
	}

	void OpenGLShader::Bind()
	{
		EE_RENDER_S({
			PE_GL(glUseProgram(self->m_RendererID));
			});
	}

	GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
	{
		if ( type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}

		if ( type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		return GL_NONE;
	}
}
