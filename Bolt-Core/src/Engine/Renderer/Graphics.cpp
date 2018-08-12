#include "Graphics.h"

namespace Bolt
{

	Model* Graphics::s_Rectangle = nullptr;
	Model* Graphics::s_Line = nullptr;

	RenderSchedule Graphics::s_Schedule = RenderSchedule();
	std::vector<std::unique_ptr<Renderer>> Graphics::s_Renderers = std::vector<std::unique_ptr<Renderer>>();

	void Graphics::Rectangle(float x, float y, float z, float w, float h, const Color& color, const Quaternion& orientation, id_t layerId)
	{
		GameObject* object = SceneManager::CurrentScene().GetLayer(layerId).AddTemporaryGameObject(GameObject());
		object->transform().SetLocalPosition(x, y, z);
		object->transform().SetLocalScale(w, h, 1);
		object->transform().SetLocalOrientation(orientation);

		Mesh mesh;
		mesh.Models.push_back({ s_Rectangle, Matrix4f::Identity(), { 0 } });
		mesh.Materials[0].BaseColor = color;
		object->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
	}

	void Graphics::Line(float x0, float y0, float z0, float x1, float y1, float z1, const Color& color, id_t layerId)
	{
		Vector3f p0 = Vector3f(x0, y0, z0);
		Vector3f p1 = Vector3f(x1, y1, z1);
		Line(p0, p1, color, layerId);
	}

	void Graphics::Line(const Vector3f& p0, const Vector3f& p1, const Color& color, id_t layerId)
	{
		float distance = Vector3f::Distance(p0, p1);
		if (distance == 0.0f)
		{
			return;
		}
		GameObject* object = SceneManager::CurrentScene().GetLayer(layerId).AddTemporaryGameObject(GameObject());
		object->transform().SetLocalPosition((p0 + p1) / 2.0f);
		object->transform().SetLocalScale(distance, 1, 1);

		Vector3f direction = p1 - p0;
		direction = direction.Normalize();
		Vector3f normal = Vector3f::Cross(Vector3f::Right(), direction);
		float dot = direction.Dot(Vector3f::Right());
		Quaternion orientation = Quaternion::FromAngleAxis(acos(dot), normal);
		object->transform().SetLocalOrientation(orientation);

		Mesh mesh;
		mesh.Models.push_back({ s_Line, Matrix4f::Identity(),{ 0 } });
		mesh.Materials[0].BaseColor = color;
		object->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
	}

	void Graphics::Text(const blt::string& text, const Font* font, float x, float y, float z, const Color& color, TextAlignmentH horizontalAlign, id_t layerId)
	{
		GameObject* object = SceneManager::CurrentScene().GetLayer(layerId).AddTemporaryGameObject(GameObject());
		object->transform().SetLocalPosition(x, y, z);

		Mesh mesh;
		mesh.Models.push_back({ new Model(TextFactory(text, font, Color::White, horizontalAlign)), Matrix4f::Identity(), { 0 } });
		mesh.Materials[0].BaseColor = color;
		mesh.Materials[0].Textures.Textures.push_back(font);
		mesh.Materials[0].Shader = Shader::DefaultFont();
		mesh.Materials[0].RenderOptions.DepthFunc = DepthFunction::Lequal;
		object->Components().AddComponent(std::make_unique<MeshRenderer>(mesh, [](Mesh& mesh) { 
			for (Mesh::ModelGroup& model : mesh.Models)
			{
				Model* m = (Model*)model.Model;
				delete m;
			}
		}));
	}

	void Graphics::Image(float x, float y, float z, float w, float h, const Texture2D* texture, const Quaternion& orientation, id_t layerId)
	{
		GameObject* object = SceneManager::CurrentScene().GetLayer(layerId).AddTemporaryGameObject(GameObject());
		object->transform().SetLocalPosition(x, y, z);

		Mesh mesh;
		mesh.Models.push_back({ s_Rectangle, Matrix4f::Scale(w, h, 1), { 0 } });
		mesh.Materials[0].BaseColor = Color::White;
		mesh.Materials[0].Textures.Textures.push_back(texture);
		mesh.Materials[0].Shader = Shader::DefaultTexture();
		object->Components().AddComponent(std::make_unique<MeshRenderer>(mesh));
	}

	RenderSchedule& Graphics::Schedule()
	{
		return s_Schedule;
	}

	id_t Graphics::AddRenderer(std::unique_ptr<Renderer>&& renderer)
	{
		id_t id = s_Renderers.size();
		s_Renderers.push_back(std::move(renderer));
		return id;
	}

	const Renderer* Graphics::GetRenderer(id_t id)
	{
		return s_Renderers.at(id).get();
	}

	void Graphics::RenderScene()
	{
		std::vector<const Framebuffer*> clearedFramebuffers;
		for (RenderPass& pass : s_Schedule.RenderPasses)
		{
			pass.PassRenderer->Begin(&pass);
			if (std::find(clearedFramebuffers.begin(), clearedFramebuffers.end(), pass.RenderTarget) == clearedFramebuffers.end())
			{
				pass.RenderTarget->Clear();
				clearedFramebuffers.push_back(pass.RenderTarget);
			}
			pass.PassRenderer->Render(&pass);
			pass.PassRenderer->End(&pass);
		}
	}

	void Graphics::Initialize()
	{
		s_Rectangle = new Model(RectangleFactory(1, 1));
		s_Line = new Model(LineFactory(Vector3f::Right(), 1));
	}

}