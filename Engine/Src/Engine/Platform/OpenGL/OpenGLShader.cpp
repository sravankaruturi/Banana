#include "eepch.h"
#include "OpenGLShader.h"

namespace ee::re
{

#define UNIFORM_LOGGING 0
#if UNIFORM_LOGGING
#define EE_LOG_UNIFORM(...) EE_CORE_WARN(__VA_ARGS__)
#else
#define EE_LOG_UNIFORM(...) 
#endif

	void OpenGLShader::ReadShaderFromFile(const std::string& filePath)
	{
		std::ifstream in;

		std::ios_base::iostate exceptionMask = in.exceptions() | std::ios::failbit;
		in.exceptions(exceptionMask);

		try
		{
			in.open(filePath, std::ios::in | std::ios::binary);
		}catch (std::ios_base::failure& e)
		{
			EE_CORE_WARN("Could not read the shader at file: {0}, Error is {1}, Errno: {2}", filePath, e.what(), errno);
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
		program = glCreateProgram();

		for (auto& kv : shader_sources) {

			GLenum type = kv.first;
			std::string& source = kv.second;

			GLuint shaderRendererId = glCreateShader(type);

			const GLchar* source_cstr = static_cast<const GLchar*>(source.c_str());
			glShaderSource(shaderRendererId, 1, &source_cstr, 0);
			glCompileShader(shaderRendererId);

			GLint isCompiled = 0;
			glGetShaderiv(shaderRendererId, GL_COMPILE_STATUS, &isCompiled);

			if (GL_FALSE == isCompiled){
				GLint maxLength = 0;
				glGetShaderiv(shaderRendererId, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infolog(maxLength);
				glGetShaderInfoLog(shaderRendererId, maxLength, &maxLength, &infolog[0]);
				EE_CORE_ERROR("Shader Compilation Failed: \n {0}", &infolog[0]);
				glDeleteShader(shaderRendererId);
				EE_CORE_ASSERT(false, "Failed");

			}

			shaderRendererIDs.push_back(shaderRendererId);
			glAttachShader(program, shaderRendererId);

		}

		glLinkProgram(program);
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)& isLinked);

		if (GL_FALSE == isLinked) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infolog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infolog[0]);
			EE_CORE_ERROR("Shader Compilation Failed:\n{0}", &infolog[0]);

			glDeleteProgram(program);

			for (auto it : shaderRendererIDs) {
				glDeleteShader(it);
			}
		}

		for (auto it : shaderRendererIDs) {
			glDetachShader(program, it);
		}
		
		m_RendererID = program;

		// Bind default texture unit
		UploadUniformInt("u_Texture", 0);

		// PBR shader textures
		UploadUniformInt("u_AlbedoTexture", 1);
		UploadUniformInt("u_NormalTexture", 2);
		UploadUniformInt("u_MetalnessTexture", 3);
		UploadUniformInt("u_RoughnessTexture", 4);

		UploadUniformInt("u_EnvRadianceTex", 10);
		UploadUniformInt("u_EnvIrradianceTex", 11);

		UploadUniformInt("u_BRDFLUTTexture", 15);

	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
		: m_AssetPath(filePath)
	{

		auto found = filePath.find_last_of("/\\");
		m_Name = (found != std::string::npos ? filePath.substr(found + 1) : filePath);
		Reload();
		
	}

	void OpenGLShader::Reload()
	{
		ReadShaderFromFile(m_AssetPath);
		EE_RENDER_S({
			if ( self->m_RendererID)
			{
				glDeleteShader(self->m_RendererID);
			}
			self->CompileAndUploadShader();
			});
	}

	void OpenGLShader::Bind()
	{
		EE_RENDER_S({
			glUseProgram(self->m_RendererID);
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


	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		EE_RENDER_S2(name, value, {
			self->UploadUniformFloat(name, value);
			});
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		EE_RENDER_S2(name, value, {
			self->UploadUniformMat4(name, value);
			});
	}

	const std::string& OpenGLShader::GetName() const
	{
		return this->m_Name;
	}

	void OpenGLShader::UploadUniformBuffer(const UniformBufferBase& uniformBuffer)
	{

		for (euint i = 0; i < uniformBuffer.GetUniformCount(); i++) {

			const UniformDecl& decl = uniformBuffer.GetUniforms()[i];
			switch (decl.type)
			{

			case UniformType::Float: {
				const std::string& name = decl.name;
				float value = *(float*)(uniformBuffer.GetBuffer() + decl.offset);
				EE_RENDER_S2(name, value, {
					self->UploadUniformFloat(name, value);
					});
				break;
			}

			case UniformType::Float3:
			{
				const std::string& name = decl.name;
				glm::vec3& values = *(glm::vec3*)(uniformBuffer.GetBuffer() + decl.offset);
				EE_RENDER_S2(name, values, {
					self->UploadUniformFloat3(name, values);
					});
				break;
			}

			case UniformType::Float4:
			{
				const std::string& name = decl.name;
				glm::vec4& values = *(glm::vec4*)(uniformBuffer.GetBuffer() + decl.offset);
				EE_RENDER_S2(name, values, { 
					self->UploadUniformFloat4(name, values);
				});
				break;
			}

			case UniformType::Matrix4x4:
			{
				const std::string& name = decl.name;
				glm::mat4& values = *(glm::mat4*)(uniformBuffer.GetBuffer() + decl.offset);
				EE_RENDER_S2(name, values, {
					self->UploadUniformMat4(name, values);
					});
				break;
			}
				

			default:
				EE_CORE_WARN("Type, is not implemented");
				break;
			}

		}

	}

	void OpenGLShader::UploadUniformInt(const std::string& name, float value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if ( location != -1)
		{
			glUniform1i(location, value);
		}else
		{
			EE_LOG_UNIFORM("Uniform, {0} not found", name);
		}
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
		{
			glUniform1f(location, value);
		}
		else
		{
			EE_LOG_UNIFORM("Uniform, {0} not found", name);
		}
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
		{
			glUniform2f(location, value.x, value.y);
		}
		else
		{
			EE_LOG_UNIFORM("Uniform, {0} not found", name);
		}
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
		{
			glUniform3f(location, value.x, value.y, value.z);
		}
		else
		{
			EE_LOG_UNIFORM("Uniform, {0} not found", name);
		}
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
		{
			glUniform4f(location, value.x, value.y, value.z, value.w);
		}
		else
		{
			EE_LOG_UNIFORM("Uniform, {0} not found", name);
		}
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		glUseProgram(m_RendererID);
		auto location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location != -1)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, (const float*)&value);
		}
		else
		{
			EE_LOG_UNIFORM("Uniform, {0} not found", name);
		}
	}

}
