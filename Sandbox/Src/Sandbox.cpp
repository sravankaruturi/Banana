#include "Sandbox.h"
#define ENABLE_DOCKSPACE 0

static void ImGuiShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void EditorLayer::OnAttach()
{
	m_SimplePBRShader.reset(ee::re::Shader::Create("Assets/Shaders/simplepbr.glsl"));
	m_QuadShader.reset(ee::re::Shader::Create("Assets/Shaders/quad.glsl"));
	m_HDRShader.reset(ee::re::Shader::Create("Assets/Shaders/hdr.glsl"));
	
	m_Mesh.reset(new ee::re::Mesh("Assets/Meshes/cerberus.fbx"));
	m_SphereMesh.reset(new ee::re::Mesh("Assets/Models/Sphere.fbx"));

	m_CheckerBoardTex.reset(ee::re::Texture2D::Create("Assets/Editor/Checkerboard.tga"));

	m_EnvironmentCubeMap.reset(ee::re::TextureCube::Create("Assets/Textures/Environments/Arches_E_PineTree_Radiance.tga"));
	m_EnvironmentIrradiance.reset(ee::re::TextureCube::Create("Assets/Textures/Environments/Arches_E_PineTree_Irradiance.tga"));
	m_BRDFLUT.reset(ee::re::Texture2D::Create("Assets/Textures/BRDF_LUT.tga"));

	m_FrameBuffer.reset(ee::re::FrameBuffer::Create(1280, 720, ee::re::FrameBufferFormat::RGBA16F));
	m_FinalPresentBuffer.reset(ee::re::FrameBuffer::Create(1280, 720, ee::re::FrameBufferFormat::RGBA8));

	float x = -1, y = -1;
	float width = 2, height = 2;
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	QuadVertex* data = new QuadVertex[4];


	data[0].Position = glm::vec3(x, y, 0);
	data[0].TexCoord = glm::vec2(0, 0);

	data[1].Position = glm::vec3(x + width, y, 0);
	data[1].TexCoord = glm::vec2(1, 0);

	data[2].Position = glm::vec3(x + width, y + height, 0);
	data[2].TexCoord = glm::vec2(1, 1);

	data[3].Position = glm::vec3(x, y + height, 0);
	data[3].TexCoord = glm::vec2(0, 1);

	m_VertexBuffer.reset(ee::re::VertexBuffer::Create());
	m_VertexBuffer->SetData(data, 4 * sizeof(QuadVertex));

	uint32_t* indices = new uint32_t[6]{ 0, 1, 2, 3, 0, 2};
	m_IndexBuffer.reset(ee::re::IndexBuffer::Create());
	m_IndexBuffer->SetData(indices, 6 * sizeof(uint32_t));

	m_Light.Direction = { -0.5f, -0.5f, 1.0f };
	m_Light.Radiance = { 1, 1, 1 };

}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate()
{

	using namespace ee::re;
	using namespace glm;

	m_Camera.Update();
	auto viewProjection = m_Camera.GetProjcetionMatrix() * m_Camera.GetViewMatrix();

	m_FrameBuffer->Bind();
	Renderer::Clear(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]);

	UniformBufferDeclaration< sizeof(mat4), 1 > quadShaderUB;
	quadShaderUB.Push("u_InverseVP", inverse(viewProjection));
	m_QuadShader->UploadUniformBuffer(quadShaderUB);

	m_QuadShader->Bind();
	m_EnvironmentIrradiance->Bind(0);
	m_VertexBuffer->Bind();
	m_IndexBuffer->Bind();
	Renderer::DrawIndexed(m_IndexBuffer->GetCount(), false);

	UniformBufferDeclaration<sizeof(mat4) * 2 + sizeof(vec3) * 4 + sizeof(float) * 8, 14> simplePbrShaderUB;
	simplePbrShaderUB.Push("u_ViewProjectionMatrix", viewProjection);
	simplePbrShaderUB.Push("u_ModelMatrix", mat4(1.0f));
	simplePbrShaderUB.Push("u_AlbedoColor", m_AlbedoInput.Colour);
	simplePbrShaderUB.Push("u_Metalness", m_MetalnessInput.Value);
	simplePbrShaderUB.Push("u_Roughness", m_RoughnessInput.Value);
	simplePbrShaderUB.Push("lights.Direction", m_Light.Direction);
	simplePbrShaderUB.Push("lights.Radiance", m_Light.Radiance * m_LightMultiplier);
	simplePbrShaderUB.Push("u_CameraPosition", m_Camera.GetPosition());
	simplePbrShaderUB.Push("u_RadiancePrefilter", m_RadiancePreFilter ? 1.0f: 0.0f);
	simplePbrShaderUB.Push("u_AlbedoTexToggle", m_AlbedoInput.UseTexture ? 1.0f: 0.0f);
	simplePbrShaderUB.Push("u_NormalTexToggle", m_NormalInput.UseTexture ? 1.0f : 0.0f);
	simplePbrShaderUB.Push("u_MetalnessTexToggle", m_MetalnessInput.UseTexture ? 1.0f : 0.0f);
	simplePbrShaderUB.Push("u_RoughnessTexToggle", m_RoughnessInput.UseTexture ? 1.0f : 0.0f);
	simplePbrShaderUB.Push("u_EnvMapRotation", m_EnvMapRotation);
	m_SimplePBRShader->UploadUniformBuffer(simplePbrShaderUB);

	m_EnvironmentCubeMap->Bind(10);
	m_EnvironmentIrradiance->Bind(11);
	m_BRDFLUT->Bind(15);

	m_SimplePBRShader->Bind();
	if (m_AlbedoInput.TextureMap)
		m_AlbedoInput.TextureMap->Bind(1);
	if (m_NormalInput.TextureMap)
		m_NormalInput.TextureMap->Bind(2);
	if (m_MetalnessInput.TextureMap)
		m_MetalnessInput.TextureMap->Bind(3);
	if (m_RoughnessInput.TextureMap)
		m_RoughnessInput.TextureMap->Bind(4);

	if (m_Scene == Scene::Spheres)
	{
		// Metals
		float roughness = 0.0f;
		float x = -88.0f;
		for (int i = 0; i < 8; i++)
		{
			m_SimplePBRShader->SetMat4("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 0.0f, 0.0f)));
			m_SimplePBRShader->SetFloat("u_Roughness", roughness);
			m_SimplePBRShader->SetFloat("u_Metalness", 1.0f);
			m_SphereMesh->Render();

			roughness += 0.15f;
			x += 22.0f;
		}

		// Dielectrics
		roughness = 0.0f;
		x = -88.0f;
		for (int i = 0; i < 8; i++)
		{
			m_SimplePBRShader->SetMat4("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 22.0f, 0.0f)));
			m_SimplePBRShader->SetFloat("u_Roughness", roughness);
			m_SimplePBRShader->SetFloat("u_Metalness", 0.0f);
			m_SphereMesh->Render();

			roughness += 0.15f;
			x += 22.0f;
		}

	}
	else if (m_Scene == Scene::Model)
	{
		m_Mesh->Render();
	}

	m_FrameBuffer->Unbind();

	m_FinalPresentBuffer->Bind();
	m_HDRShader->Bind();
	m_HDRShader->SetFloat("u_Exposure", m_Exposure);
	m_FrameBuffer->BindTexture();
	m_VertexBuffer->Bind();
	m_IndexBuffer->Bind();
	Renderer::DrawIndexed(m_IndexBuffer->GetCount(), false);

	m_FinalPresentBuffer->Unbind();
	
}

void EditorLayer::OnImGuiRender()
{
	static bool p_open = true;

	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = opt_fullscreen_persistant;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Dockspace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	/*
	 * Editor Panel
	 */
	ImGui::Begin("Settings");
	ImGui::Separator();
	ImGui::RadioButton("Spheres", (int*)& m_Scene, (int)Scene::Spheres);
	ImGui::SameLine();
	ImGui::RadioButton("Model", (int*)& m_Scene, (int)Scene::Model);

	ImGui::Separator();
	ImGui::ColorEdit4("Clear Colour", m_ClearColour);

	ImGui::SliderFloat3("Light Dir", glm::value_ptr(m_Light.Direction), -1, 1);
	ImGui::ColorEdit3("Light Radiance", glm::value_ptr(m_Light.Radiance));
	ImGui::SliderFloat("Light Multiplier", &m_LightMultiplier, 0.0f, 5.0f);
	ImGui::SliderFloat("Exposure", &m_Exposure, 0.0f, 10.0f);

	auto cameraForward = m_Camera.GetForwardDirection();
	ImGui::Text("Camera Forward: %.2f, %.2f, %.2f", cameraForward.x, cameraForward.y, cameraForward.z);

	ImGui::Separator();
	{
		ImGui::Text("Mesh");
		std::string fullPath = m_Mesh ? m_Mesh->GetFilePath() : "None";
		auto found = fullPath.find_last_of("/\\");
		std::string path = (found != std::string::npos ? fullPath.substr(found + 1) : fullPath);
		ImGui::Text(path.c_str());
		ImGui::SameLine();
		if ( ImGui::Button("...##Mesh"))
		{
			std::string filename = ee::Application::GetInstance().OpenFile("");
			if ( filename != "")
			{
				m_Mesh.reset(new ee::re::Mesh(filename));
			}
		}
	}

	ImGui::Separator();
	
	if ( ImGui::TreeNode("Shaders"))
	{
		auto& shaders = ee::re::Shader::s_AllShaders;
		for(auto& shader: shaders)
		{
			if ( ImGui::TreeNode(shader->GetName().c_str()))
			{
				std::string buttonName = "Reload##" + shader->GetName();
				if ( ImGui::Button(buttonName.c_str()))
				{
					shader->Reload();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::Text("Shader Parameters");
	ImGui::Checkbox("Radiance Prefiltering", &m_RadiancePreFilter);
	ImGui::SliderFloat("Env Map Rotation", &m_EnvMapRotation, -360.f, 360.f);
	ImGui::Separator();

	/* Textures */
	{
		/* Albedo */
		if (ImGui::CollapsingHeader("Albedo", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(m_AlbedoInput.TextureMap ? (void*)m_AlbedoInput.TextureMap->GetRendererID() : (void*)m_CheckerBoardTex->GetRendererID(), ImVec2(64, 64));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (m_AlbedoInput.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(m_AlbedoInput.TextureMap->GetPath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image((void*)m_AlbedoInput.TextureMap->GetRendererID(), ImVec2(384, 384));
					ImGui::EndTooltip();
				}
				if (ImGui::IsItemClicked())
				{
					std::string filename = ee::Application::GetInstance().OpenFile("");
					if (filename != "")
						m_AlbedoInput.TextureMap.reset(ee::re::Texture2D::Create(filename, m_AlbedoInput.SRGB));
				}
			}
			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Checkbox("Use##AlbedoMap", &m_AlbedoInput.UseTexture);
			if (ImGui::Checkbox("sRGB##AlbedoMap", &m_AlbedoInput.SRGB))
			{
				if (m_AlbedoInput.TextureMap)
					m_AlbedoInput.TextureMap.reset(ee::re::Texture2D::Create(m_AlbedoInput.TextureMap->GetPath(), m_AlbedoInput.SRGB));
			}
			ImGui::EndGroup();
			ImGui::SameLine();
			ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(m_AlbedoInput.Colour), ImGuiColorEditFlags_NoInputs);
		}
	}
	{
		// Normals
		if (ImGui::CollapsingHeader("Normals", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(m_NormalInput.TextureMap ? (void*)m_NormalInput.TextureMap->GetRendererID() : (void*)m_CheckerBoardTex->GetRendererID(), ImVec2(64, 64));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (m_NormalInput.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(m_NormalInput.TextureMap->GetPath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image((void*)m_NormalInput.TextureMap->GetRendererID(), ImVec2(384, 384));
					ImGui::EndTooltip();
				}
				if (ImGui::IsItemClicked())
				{
					std::string filename = ee::Application::GetInstance().OpenFile("");
					if (filename != "")
						m_NormalInput.TextureMap.reset(ee::re::Texture2D::Create(filename));
				}
			}
			ImGui::SameLine();
			ImGui::Checkbox("Use##NormalMap", &m_NormalInput.UseTexture);
		}
	}
	{
		// Metalness
		if (ImGui::CollapsingHeader("Metalness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(m_MetalnessInput.TextureMap ? (void*)m_MetalnessInput.TextureMap->GetRendererID() : (void*)m_CheckerBoardTex->GetRendererID(), ImVec2(64, 64));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (m_MetalnessInput.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(m_MetalnessInput.TextureMap->GetPath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image((void*)m_MetalnessInput.TextureMap->GetRendererID(), ImVec2(384, 384));
					ImGui::EndTooltip();
				}
				if (ImGui::IsItemClicked())
				{
					std::string filename = ee::Application::GetInstance().OpenFile("");
					if (filename != "")
						m_MetalnessInput.TextureMap.reset(ee::re::Texture2D::Create(filename));
				}
			}
			ImGui::SameLine();
			ImGui::Checkbox("Use##MetalnessMap", &m_MetalnessInput.UseTexture);
			ImGui::SameLine();
			ImGui::SliderFloat("Value##MetalnessInput", &m_MetalnessInput.Value, 0.0f, 1.0f);
		}
	}
	{
		// Roughness
		if (ImGui::CollapsingHeader("Roughness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
			ImGui::Image(m_RoughnessInput.TextureMap ? (void*)m_RoughnessInput.TextureMap->GetRendererID() : (void*)m_CheckerBoardTex->GetRendererID(), ImVec2(64, 64));
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
			{
				if (m_RoughnessInput.TextureMap)
				{
					ImGui::BeginTooltip();
					ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
					ImGui::TextUnformatted(m_RoughnessInput.TextureMap->GetPath().c_str());
					ImGui::PopTextWrapPos();
					ImGui::Image((void*)m_RoughnessInput.TextureMap->GetRendererID(), ImVec2(384, 384));
					ImGui::EndTooltip();
				}
				if (ImGui::IsItemClicked())
				{
					std::string filename = ee::Application::GetInstance().OpenFile("");
					if (filename != "")
						m_RoughnessInput.TextureMap.reset(ee::re::Texture2D::Create(filename));
				}
			}
			ImGui::SameLine();
			ImGui::Checkbox("Use##RoughnessMap", &m_RoughnessInput.UseTexture);
			ImGui::SameLine();
			ImGui::SliderFloat("Value##RoughnessInput", &m_RoughnessInput.Value, 0.0f, 1.0f);
		}

	}
	ImGui::Separator();
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");
	auto viewportSize = ImGui::GetContentRegionAvail();
	m_FrameBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
	m_FinalPresentBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
	m_Camera.SetProjectionMatrix(
		glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 10000.0f)
	);
	ImGui::Image((void*)m_FinalPresentBuffer->GetColourAttachmentRendererID(), viewportSize, { 0, 1 }, { 1, 0 });
	ImGui::End();
	ImGui::PopStyleVar();

	
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Docking"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows, 
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Flag: NoSplit", "", (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 opt_flags ^= ImGuiDockNodeFlags_NoSplit;
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
			if (ImGui::MenuItem("Flag: NoResize", "", (opt_flags & ImGuiDockNodeFlags_NoResize) != 0))                opt_flags ^= ImGuiDockNodeFlags_NoResize;
			if (ImGui::MenuItem("Flag: PassthruDockspace", "", (opt_flags & ImGuiDockNodeFlags_PassthruDockspace) != 0))       opt_flags ^= ImGuiDockNodeFlags_PassthruDockspace;
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (opt_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          opt_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
			ImGui::Separator();
			if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
				p_open = false;
			ImGui::EndMenu();
		}
		ImGuiShowHelpMarker(
			"You can _always_ dock _any_ window into another by holding the SHIFT key while moving a window. Try it now!" "\n"
			"This demo app has nothing to do with it!" "\n\n"
			"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
			"ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
			"(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
		);

		ImGui::EndMenuBar();
	}

	ImGui::End();

}

void EditorLayer::OnEvent(ee::Event& event)
{
}

Sandbox::Sandbox()
{
	EE_TRACE("Hello!");
}

void Sandbox::OnInit()
{
	PushLayer(new EditorLayer());
}
