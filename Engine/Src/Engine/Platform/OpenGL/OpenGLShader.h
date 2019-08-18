#pragma once

#include "Engine/Renderer/Shader.h"
#include <glad/glad.h>

namespace ee::re
{

	class OpenGLShader : public Shader
	{
		
	private:
		RendererID m_RendererID;
		std::string m_ShaderSource;
		std::string m_Name, m_AssetPath;

	public:

		OpenGLShader(const std::string& filePath);
		
		virtual void Bind() override;
		virtual void Reload() override;
		
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		
		virtual const std::string& GetName() const override;
		
		virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) override;

		static GLenum ShaderTypeFromString(const std::string& type);

	private:
		void UploadUniformInt(const std::string& name, float value);
		
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat4(const std::string& name, const glm::mat4& value);
		
		void ReadShaderFromFile(const std::string& filePath);
		void CompileAndUploadShader();

	};
	
}