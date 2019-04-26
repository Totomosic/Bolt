#include "Bolt.h"
#include "ChatManager.h"

namespace Chat
{

	class App : public Application
	{
	public:
		UIsurface* menuBar;
		UIsurface* createGroupMenu;
		UIsurface* groupMenu;
		UIsurface* contentPanel;

		ChatManager chatManager;

		UIsurface* currentTextBox = nullptr;

	public:
		void Init() override
		{
			GetWindow().SetClearColor(Color(225, 225, 225));
			GetWindow().EnableVSync();
			Scene& scene = SceneManager::Get().CreateScene();
			Camera* camera = scene.CreateCamera(Projection::Orthographic(0, 1920, 0, 1080, 0, 200));
			camera->transform().Translate(0, 0, 100);
			Layer& layer = scene.CreateLayer(camera);

			if (!Filesystem::FileExists("data/chats.txt"))
			{
				File f = Filesystem::Open("data/chats.txt", OpenMode::Write);
				Filesystem::Close(f);
			}
			if (!Filesystem::FileExists("data/user.txt"))
			{
				File f = Filesystem::Open("data/user.txt", OpenMode::Write);
				f.WriteText("DefaultUser");
				Filesystem::Close(f);
			}

			ChatUser me;
			File userFile = Filesystem::Open("data/user.txt", OpenMode::Read);
			me.Username = userFile.ReadText();
			Filesystem::Close(userFile);
			XMLfile xml = Filesystem::OpenXML("data/chats.txt");
			XMLnode node = xml.LoadXML();
			chatManager = ChatManager::FromDataFile(node);
			chatManager.SetMyUser(me);

			menuBar = &layer.UI().Rectangle(camera->ViewWidth(), 30, Color::Black, Transform({ camera->ViewWidth() / 2, camera->ViewHeight() - 15, -100 }));
			createGroupMenu = &layer.UI().Rectangle(600, 100, Color(210, 210, 210), Transform({ 300, camera->ViewHeight() - 80, -90 }));
			groupMenu = &layer.UI().Rectangle(600, camera->ViewHeight() - 30, Color::White, Transform({ 300, camera->ViewHeight() / 2 - 15, -90 }));
			contentPanel = &layer.UI().Rectangle(camera->ViewWidth() - 600, camera->ViewHeight() - 30, Color(225, 225, 225), Transform({ camera->ViewWidth() / 2 + 300, camera->ViewHeight() / 2 - 15, -90 }));

			File script = Filesystem::Open("script.txt");
			contentPanel->Object()->Components().AddComponent<LuaScript>(script.ReadText());
			Filesystem::Close(script);

			createGroupMenu->Text("Chats", ResourceManager::Get().Fonts().Arial(32), Color::Black, Transform({ 0, 0, 1 }));
			UIsurface& createGroupButton = createGroupMenu->Rectangle(30, 30, Color::Green, Transform({ 200, 0, 1 }));
			createGroupButton.EventHandler().OnClicked.Subscribe([this](UIClickedEvent & e)
				{
					LoadCreateGroupPanel();
					ListenerResponse response;
					return response;
				});
			UIsurface& closeContentPanelButton = createGroupMenu->Rectangle(30, 30, Color::Red, Transform({ -200, 0, 1 }));
			closeContentPanelButton.EventHandler().OnClicked.Subscribe([this](UIClickedEvent & e)
				{
					LoadDefaultContentPanel();
					ListenerResponse response;
					return response;
				});

			LoadDefaultContentPanel();
			LoadGroupSidePanel();

			RenderSchedule s(scene);
			s.AddRenderProcess({ });
			SceneRenderer::Get().AddRenderSchedule(s);
		}

		void Update() override
		{
			if (currentTextBox != nullptr)
			{
				Text& t = (Text&)currentTextBox->GetElement(0);
				for (char c : Input::Get().PressedCharacters())
				{
					t.SetText(t.String() + c);
				}
				if (Input::Get().KeyPressed(Keycode::Backspace) && t.String().size() > 0)
				{
					t.SetText(t.String().substr(0, t.String().size() - 1));
				}
				if (chatManager.ActiveGroupIndex() != -1 && Input::Get().KeyPressed(Keycode::Enter))
				{
					blt::string text = t.String();
					t.SetText("");
					PostChatMessage(chatManager.MyUser(), GetDateTimeString(), text);					
				}
			}
		}

		void Render() override
		{
			Graphics::Get().RenderScene();
		}

		void Exit() override
		{
			XMLfile xml = Filesystem::OpenXML("data/chats.txt", OpenMode::Write);
			chatManager.WriteDataFile(xml);
			xml.WriteXMLFile();
			Application::Exit();
		}

		void ClearContentPanel()
		{
			currentTextBox = nullptr;
			chatManager.SetActiveGroupIndex(-1);
			contentPanel->Clear();
		}

		void ClearGroupPanel()
		{
			groupMenu->Clear();
		}

		void LoadDefaultContentPanel()
		{
			ClearContentPanel();
			contentPanel->Text("No content to display.", ResourceManager::Get().Fonts().Arial(24), Color::Black, Transform({ 0, 0, 1 }));
			contentPanel->Text("Open a chat from the left menu to view content.", ResourceManager::Get().Fonts().Arial(24), Color::Black, Transform({ 0, -24, 1 }));
		}

		void LoadCreateGroupPanel()
		{
			ClearContentPanel();
			UIsurface& titleMenu = contentPanel->Rectangle(contentPanel->Width(), 100, Color::White, Transform({ 0, contentPanel->Height() / 2 - 50, 1 }));
			titleMenu.Text("Create new chat", ResourceManager::Get().Fonts().Arial(32), Color::Black, Transform({ 0, 0, 1 }));
			UIsurface& menu = contentPanel->Rectangle(600, 800, Color::White, Transform({ 0, -50, 1 }));
			menu.Text("Name: ", ResourceManager::Get().Fonts().Arial(28), Color::Black, Transform({ -150, 300, 1 }), AlignH::Right);
			menu.Text("IP: ", ResourceManager::Get().Fonts().Arial(28), Color::Black, Transform({ -150, 200, 1 }), AlignH::Right);
		}

		void LoadGroup(int groupIndex)
		{
			ClearContentPanel();
			chatManager.SetActiveGroupIndex(groupIndex);
			const ChatGroup& group = chatManager.ActiveGroup();			
			UIsurface& titleMenu = contentPanel->Rectangle(contentPanel->Width(), 100, Color::White, Transform({ 0, contentPanel->Height() / 2 - 50, 1 }));
			titleMenu.Text(group.Name(), ResourceManager::Get().Fonts().Arial(32), Color::Black, Transform({ 0, 0, 1 }));
			UIsurface& chatRegion = contentPanel->Rectangle(contentPanel->Width() - 50, 50, Color::White, Transform({ 0, -contentPanel->Height() / 2 + 25, 1 }));
			UIsurface& chatBox = chatRegion.Rectangle(chatRegion.Width() - 20, chatRegion.Height() - 10, Color(220, 220, 220), Transform({ 0, 0, 1 }));
			chatBox.Text("", ResourceManager::Get().Fonts().Arial(24), Color::Black, Transform({ -chatBox.Width() / 2 + 5, 0, 1 }), AlignH::Left);
			chatBox.EventHandler().OnClicked.Subscribe([this, &chatBox](UIClickedEvent & e)
				{
					currentTextBox = &chatBox;
					return ListenerResponse();
				});

			for (int i = 0; i < group.Messages().Messages.size(); i++)
			{
				float xOff = -40;
				Color c = Color::White;
				const ChatMessage& message = group.Messages().Messages.at(i);
				if (message.From.Username == chatManager.MyUser().Username)
				{
					xOff = 40;
					c = Color(150, 150, 255);
				}
				UIsurface& messageRect = contentPanel->Rectangle(contentPanel->Width() - 100, 70, c, Transform({ xOff, contentPanel->Height() / 2 - 110 - 35 - 80 * i, 1 }));
				messageRect.Text(message.From.Username + " (" + message.Message.Timestamp() + ")", ResourceManager::Get().Fonts().Arial(28), Color::Black, Transform({ -messageRect.Width() / 2 + 10, messageRect.Height() / 2 - 15, 1 }), AlignH::Left);
				messageRect.Text(message.Message.Message(), ResourceManager::Get().Fonts().Arial(24), Color::Black, Transform({ -messageRect.Width() / 2 + 10, messageRect.Height() / 2 - 15 - 28, 1 }), AlignH::Left);
			}
		}

		void LoadGroupSidePanel()
		{
			ClearGroupPanel();
			for (int i = 0; i < chatManager.Groups().size(); i++)
			{
				const ChatGroup& group = chatManager.Groups().at(i);
				UIsurface& groupButton = groupMenu->Rectangle(groupMenu->Width(), 150, Color(240, 240, 240), Transform({ 0, groupMenu->Height() / 2 - 175 - 155 * i, 1 }));
				groupButton.Text(group.Name(), ResourceManager::Get().Fonts().Arial(28), Color::Black, Transform({ 0, 0, 1 }));
				groupButton.EventHandler().OnClicked.Subscribe([this, i](UIClickedEvent& e)
					{
						LoadGroup(i);
						ListenerResponse response;
						return response;
					});
			}
		}

		void PostChatMessage(const ChatUser& user, const blt::string& timestamp, const blt::string& message)
		{
			ChatGroup& group = chatManager.ActiveGroup();
			ChatMessage msg;
			msg.From = user;
			msg.Message.SetTimestamp(timestamp);
			msg.Message.SetMessage(message);
			group.Messages().Messages.push_back(msg);
			LoadGroup(chatManager.ActiveGroupIndex());
		}

		blt::string GetDateTimeString()
		{
			time_t rawtime;
			struct tm* timeinfo;
			char buffer[80];

			time(&rawtime);
			timeinfo = localtime(&rawtime);

			strftime(buffer, sizeof(buffer), "%d/%m/%y %H:%M", timeinfo);
			return blt::string(buffer);
		}

	};

}

int main()
{
	EngineCreateInfo eInfo;
	WindowCreateInfo wInfo;
	wInfo.Title = "Chat";
	wInfo.Width = 1280;
	wInfo.Height = 720;
	eInfo.WindowInfo = wInfo;
	Engine e(eInfo);
	e.SetApplication<Chat::App>();
	e.Run();
	return 0;
}