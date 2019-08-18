#pragma once
#include "Engine.h"
#include "Engine/Core/Layers/ImGui/ImGuiLayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

class EditorLayer : public ee::Layer
{

private:
	
	std::unique_ptr<ee::re::Shader> m_Shader;
	std::unique_ptr<ee::re::Shader> m_PBRShader;
	std::unique_ptr<ee::re::Shader> m_SimplePBRShader;
	std::unique_ptr<ee::re::Shader> m_QuadShader;
	std::unique_ptr<ee::re::Shader> m_HDRShader;
	std::unique_ptr<ee::re::Mesh> m_Mesh;
	std::unique_ptr<ee::re::Mesh> m_SphereMesh;

	std::unique_ptr<ee::re::Texture2D> m_BRDFLUT;

	struct AlbedoInput
	{
		// Silver
		glm::vec3 Colour = { 0.972f, 0.96f, 0.915f };
		std::unique_ptr<ee::re::Texture2D> TextureMap;
		bool SRGB = true;
		bool UseTexture = false;
	};
	AlbedoInput m_AlbedoInput;

	struct NormalInput
	{
		std::unique_ptr<ee::re::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	NormalInput m_NormalInput;

	struct MetalnessInput
	{
		float Value = 1.0f;
		std::unique_ptr<ee::re::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	MetalnessInput m_MetalnessInput;

	struct RoughnessInput
	{
		float Value = 0.5f;
		std::unique_ptr<ee::re::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	RoughnessInput m_RoughnessInput;

	std::unique_ptr<ee::re::FrameBuffer> m_FrameBuffer, m_FinalPresentBuffer;

	std::unique_ptr<ee::re::VertexBuffer> m_VertexBuffer;
	std::unique_ptr<ee::re::IndexBuffer> m_IndexBuffer;
	std::unique_ptr<ee::re::TextureCube> m_EnvironmentCubeMap, m_EnvironmentIrradiance;

	ee::re::Camera m_Camera;

	struct Light
	{
		glm::vec3 Direction;
		glm::vec3 Radiance;
	};
	Light m_Light;
	float m_LightMultiplier = 0.3f;

	float m_Exposure = 1.0f;

	bool m_RadiancePreFilter = false;
	float m_EnvMapRotation = 0.0f;

	enum class Scene : uint32_t
	{
		Spheres = 0,
		Model = 1
	};
	Scene m_Scene;

	std::unique_ptr<ee::re::Texture2D> m_CheckerBoardTex;
	
	
public:
	EditorLayer()
		: m_Camera(glm::perspectiveFov(glm::radians(45.0f), 1290.0f, 720.0f, 0.1f, 10000.f)),
	m_Scene(Scene::Spheres)
	{
		
	}

	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(ee::Event& event) override;
};


class Sandbox : public ee::Application
{

public:
	Sandbox();

	virtual void OnInit() override;
};

inline ee::Application* ee::CreateApplication()
{
	return new Sandbox();
}