#include "BoltEngine.h"

using namespace Bolt;

namespace NatureScene
{

	class App : public Application
	{
	public:
		float CAMERA_SPEED = 5;
		float MOUSE_X_SENSITIVITY = 0.005f;
		float MOUSE_Y_SENSITIVITY = 0.005f;
		Camera* perspectiveCamera;
		Camera* orthoCamera;

		ObjectFactory Factory;
		Text* fpsText;

	public:
		void Init() override
		{
			PrimaryWindow->SetClearColor(Color::SkyBlue);

			Scene* scene = SceneManager::CreateScene();
			Layer* layer = scene->CreateLayer<SceneArray>("Main");
			Layer* uiLayer = scene->CreateLayer<SceneArray>("UI");
			perspectiveCamera = scene->CreateCamera(Frustum::Perspective(PI / 3, PrimaryWindow->GetFramebuffer().Aspect(), 1, 1000), ProjectionType::Perspective);
			orthoCamera = scene->CreateCamera(PrimaryWindow->GetFramebuffer().ViewFrustum(), ProjectionType::Orthographic);
			layer->SetActiveCamera(perspectiveCamera);
			uiLayer->SetActiveCamera(orthoCamera);

			float HEIGHT = 10;

			ResourceManager::Register(std::make_unique<Font>("res/arial.ttf", 24));

			ResourcePack resources = ResourceManager::FetchPack("res/resources.pack");
			ResourceManager::LoadPack(resources);

			Factory.SetCurrentLayer(layer);
			GameObject* cube = Factory.Cuboid(2, 2, 2, Color::White, Transform({ 0, 0, -10 }));
			GameObject* water = Factory.Grid(50, 50, 2, 2, Color(50, 150, 255, 220));
			GameObject* ground = Factory.Grid(50, 50, 500, 500, Color::White);
			ground->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Textures.Textures.push_back(ResourceManager::Get<Texture2D>(resources.Resources.at("map").Id));
			ground->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].Shader = Shader::LightingTexture();
			water->Components().GetComponent<MeshRenderer>().Mesh.Materials[0].LightingOptions.Reflectivity = 1.0f;

			Factory.Rectangle(50, HEIGHT, Color::White, Transform({ 0, -HEIGHT / 2, 25 }, Quaternion::FromAngleAxis(0, Vector3f::Up())));
			Factory.Rectangle(50, HEIGHT, Color::White, Transform({ 0, -HEIGHT / 2, -25 }, Quaternion::FromAngleAxis(PI, Vector3f::Up())));
			Factory.Rectangle(50, HEIGHT, Color::White, Transform({ -25, -HEIGHT / 2, 0 }, Quaternion::FromAngleAxis(-PI / 2, Vector3f::Up())));
			Factory.Rectangle(50, HEIGHT, Color::White, Transform({ 25, -HEIGHT / 2, 0 }, Quaternion::FromAngleAxis(PI / 2, Vector3f::Up())));

			SimplexNoise noise(0.002f, 3, 1.25f, 1.0f / 1.25f);
			{
				VertexIterator it = ground->Components().GetComponent<MeshRenderer>().Mesh.Models[0].Model->Data().Vertices->Begin();
				for (int x = 0; x < 500; x++)
				{
					for (int y = 0; y < 500; y++)
					{
						float left = noise.Generate(8, x - 1, y) * HEIGHT;
						float right = noise.Generate(8, x + 1, y) * HEIGHT;
						float bottom = noise.Generate(8, x, y - 1) * HEIGHT;
						float top = noise.Generate(8, x, y + 1) * HEIGHT;
						it[0].Read<Vector3f>().y = noise.Generate(8, x, y) * HEIGHT;
						it[1] = Vector3f(left - right, 0.2f, bottom - top).Normalize();
						it++;
					}
				}
			}

			fpsText = uiLayer->UI().Text("fps ", Color::White, Transform({ 20, Height() - 20, -5 }), AlignH::Left, AlignV::Top);
			
			id_t rendererId = Graphics::AddRenderer(std::make_unique<Renderer>(std::make_unique<DefaultRenderMethod>()));
			Graphics::Schedule().AddPass({ Graphics::DefaultFramebuffer(), RenderPass::ALL_LAYERS, Graphics::GetRenderer(rendererId),{},{},{},{ LightSource{ Vector3f(0, 100, 0) } } });
		}

		void Update() override
		{
			fpsText->SetText("fps " + std::to_string((int)Time::FramesPerSecond()));
			if (Input::KeyDown(Keycode::W))
			{
				perspectiveCamera->transform().Translate(perspectiveCamera->transform().Forward() * CAMERA_SPEED * Time::DeltaTime());
			}
			if (Input::KeyDown(Keycode::S))
			{
				perspectiveCamera->transform().Translate(perspectiveCamera->transform().Forward() * -CAMERA_SPEED * Time::DeltaTime());
			}
			if (Input::KeyDown(Keycode::D))
			{
				perspectiveCamera->transform().Translate(perspectiveCamera->transform().Right() * CAMERA_SPEED * Time::DeltaTime());
			}
			if (Input::KeyDown(Keycode::A))
			{
				perspectiveCamera->transform().Translate(perspectiveCamera->transform().Right() * -CAMERA_SPEED * Time::DeltaTime());
			}
			if (Input::MouseButtonDown(MouseButton::Left))
			{
				float x = Input::RelMousePosition().x * MOUSE_X_SENSITIVITY;
				float y = Input::RelMousePosition().y * MOUSE_Y_SENSITIVITY;
				perspectiveCamera->transform().Rotate(-x, Vector3f::Up(), Space::World);			
				perspectiveCamera->transform().Rotate(y, Vector3f::Right(), Space::Local);				
			}

			if (Input::MouseButtonPressed(MouseButton::Right))
			{
				Ray ray = perspectiveCamera->NDCToWorldRay(Input::NormalizedMousePosition());
				GameObject* line = Factory.Line(ray.Origin, ray.Origin + ray.Direction * 100, Color::Black);
				Destroy(line, 5);
			}

		}

		void Render() override
		{
			Graphics::RenderScene();
		}

	};

}

void main()
{
	Engine e;
	WindowCreateInfo info;
	e.SetWindow(std::make_unique<Window>(1280, 720, "Scene", info));
	e.SetApplication(std::make_unique<NatureScene::App>());
	e.Run();
}