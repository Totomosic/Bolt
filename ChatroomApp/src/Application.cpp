#include "BoltEngine.h"

using namespace Bolt;

namespace Chatroom
{

	class ChatroomApp : public Application
	{
	public:
		GameObject* currentTextObject;
		blt::string currentText = "";

	public:
		void Init() override
		{
			PrimaryWindow->SetClearColor(Color(220, 220, 220));

			Font* arial = ResourceManager::Register("Arial", std::make_unique<Font>("res/arial.ttf", 16));

			Scene* scene = SceneManager::CreateScene();
			Layer* layer = scene->CreateLayer<SceneArray>("Default");

			Camera* mainCamera = scene->CreateCamera(Projection::Orthographic(0, PrimaryWindow->Width(), 0, PrimaryWindow->Height(), -100, 100));
			layer->SetActiveCamera(mainCamera);

			Mesh panelMesh;
			panelMesh.Models.push_back({ ResourceManager::Register("PanelModel", std::make_unique<Model>(RectangleFactory(1, 1))) });

			Mesh textMesh;
			textMesh.Models.push_back({ ResourceManager::Register("TextModel", std::make_unique<Model>(TextFactory(currentText, arial, Color::Black, TextAlignmentH::Left))) });
			textMesh.Materials[0].Shader = Shader::DefaultFont();
			textMesh.Materials[0].Textures.Textures.push_back(arial);

			GameObject* historyPanel = GameObject::Instantiate(layer);
			historyPanel->transform().SetLocalPosition(PrimaryWindow->Width() / 2, PrimaryWindow->Height() / 2 + 25, 0);
			historyPanel->transform().SetLocalScale(PrimaryWindow->Width() - 10, PrimaryWindow->Height() - 50 - 10, 1);
			historyPanel->Components().AddComponent(std::make_unique<MeshRenderer>(panelMesh));

			GameObject* chatPanel = GameObject::Instantiate(layer);
			chatPanel->transform().SetLocalPosition(PrimaryWindow->Width() / 2 - 100, 27, 5);
			chatPanel->transform().SetLocalScale(PrimaryWindow->Width() - 200 - 10, 46, 1);
			chatPanel->Components().AddComponent(std::make_unique<MeshRenderer>(panelMesh));

			currentTextObject = GameObject::Instantiate(layer);
			currentTextObject->MakeChildOf(chatPanel);
			currentTextObject->transform().SetLocalPosition(-0.5f + 0.01f, 0, 2);
			currentTextObject->transform().SetLocalScale(1.0f / chatPanel->transform().Scale());
			currentTextObject->Components().AddComponent(std::make_unique<MeshRenderer>(textMesh));

			id_t rendererId = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().RenderPasses.push_back({ &PrimaryWindow->GetFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(rendererId) });
		}

		void Tick() override
		{
		
		}

		void Update() override
		{
			blt::string newString = currentText;
			for (char c : Input::PressedCharacters())
			{
				newString += c;
			}
			if (Input::KeyPressed(Keycode::Backspace) && newString.size() > 0)
			{
				newString.pop_back();
			}
			if (newString != currentText)
			{
				ResourceManager::FreeResource("TextModel");
				Model* newModel = ResourceManager::Register("TextModel", std::make_unique<Model>(TextFactory(newString, ResourceManager::Get<Font>("Arial"), Color::Black, TextAlignmentH::Left)));
				currentTextObject->Components().GetComponent<MeshRenderer>().Mesh.Models[0].Model = newModel;
				currentText = newString;
			}
		}

		void Render() override
		{
			Graphics::RenderScene();
		}
	};

}

int main()
{
	Engine e;
	e.SetWindow(std::make_unique<Window>(600, 800, "Chatroom"));
	e.SetApplication(std::make_unique<Chatroom::ChatroomApp>());
	while (!e.ShouldClose())
	{
		e.UpdateApplication();
	}
}