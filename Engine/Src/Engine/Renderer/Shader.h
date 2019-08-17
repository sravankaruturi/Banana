﻿#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ee::re
{

	struct ShaderUniform
	{

	};

	struct ShaderUniformCollection
	{

	};

	enum class UniformType {

		None = 0,
		Float, Float2, Float3, Float4,
		Matrix3x3, Matrix4x4,
		Int32, Uint32

	};

	struct UniformDecl {

		UniformType type;
		std::ptrdiff_t offset;
		std::string name;

	};

	struct UniformBuffer {

	// TODO: This currently represents a byte buffer that has been
	// packed with uniforms. This was primarily created for OpenGL,
	// and needs to be revisted for other rendering APIs. Furthermore,
	// this currently does not assume any alignment. This also has
	// nothing to do with GL uniform buffers, this is simply a CPU-side
	// buffer abstraction.

		byte* Buffer;
		std::vector<UniformDecl> Uniforms;
	};

	struct UniformBufferBase {

		virtual const byte* GetBuffer() const = 0;
		virtual const UniformDecl* GetUniforms() const = 0;
		virtual euint GetUniformCount() const = 0;

	};

	template<euint N, euint U>
	struct UniformBufferDeclaration : public UniformBufferBase {

		byte Buffer[N];
		UniformDecl Uniforms[U];
		std::ptrdiff_t Cursor = 0;
		int Index = 0;

		virtual const byte* GetBuffer() const override { return Buffer; }
		virtual const UniformDecl* GetUniforms() const override { return Uniforms; }
		virtual	euint GetUniformCount() const override { return U; }

		template<typename T>
		void Push(const std::string& name, const T& data) {}

		template<>
		void Push(const std::string& name, const float& data) {

			Uniforms[Index++] = { UniformType::Float, Cursor, name };
			memcpy(Buffer + Cursor, &data, sizeof(float));
			Cursor += sizeof(float);

		}

		template<>
		void Push(const std::string& name, const glm::vec3& data)
		{
			Uniforms[Index++] = { UniformType::Float3, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec3));
			Cursor += sizeof(glm::vec3);
		}
		
		template<>
		void Push(const std::string& name, const glm::vec4& data) {

			Uniforms[0] = { UniformType::Float4, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec4));
			Cursor += sizeof(glm::vec4);

		}

		template<>
		void Push(const std::string& name, const glm::mat4& data)
		{
			Uniforms[Index++] = { UniformType::Matrix4x4, Cursor, name };
			memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::mat4));
			Cursor += sizeof(glm::mat4);
		}

	};

	class Shader
	{

	public:
		virtual void Reload() = 0;
		
		virtual void Bind() = 0;
		virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) = 0;

		/* TEMP */
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;

		static Shader* Create(const std::string& filePath);

		static std::vector<Shader*> s_AllShaders;

	};
}
