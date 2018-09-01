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

			Camera* mainCamera = scene->CreateCamera(Projection::Orthographic(0, Width(), 0, Height(), -100, 100));
			layer->SetActiveCamera(mainCamera);

			Mesh textMesh;
			textMesh.Models.push_back({ ResourceManager::Register("TextModel", std::make_unique<Model>(TextFactory(currentText, arial, Color::Black, AlignH::Left, AlignV::Bottom))) });
			textMesh.Materials[0].Shader = Shader::DefaultFont();
			textMesh.Materials[0].Textures.Textures.push_back(arial);

			ObjectFactory factory(layer);

			GameObject* historyPanel = factory.Rectangle(Width() - 10, Height() - 50 - 10, Color::White, Transform(Vector3f(Width() / 2, Height() / 2 + 25, 0)));
			GameObject* chatPanel = factory.Rectangle(Width() - 200 - 10, 46, Color::White, Transform(Vector3f(Width() / 2 - 100, 27, 5)));

			factory.SetCurrentParent(chatPanel);
			currentTextObject = factory.Instantiate(textMesh);
			currentTextObject->transform().SetLocalPosition(-(Width() - 200 - 10) / 2 + 10, 0, 2);

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
				Model* newModel = ResourceManager::Register("TextModel", std::make_unique<Model>(TextFactory(newString, ResourceManager::Get<Font>("Arial"), Color::Black, AlignH::Left)));
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
	WindowCreateInfo info;
	info.Resizable = false;
	info.Samples = 1;
	e.SetWindow(std::make_unique<Window>(600, 800, "Chatroom", info));
	e.SetApplication(std::make_unique<Chatroom::ChatroomApp>());
	while (!e.ShouldClose())
	{
		e.UpdateApplication();
	}
}