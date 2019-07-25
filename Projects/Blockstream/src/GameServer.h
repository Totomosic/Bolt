#pragma once
#include "BoltInclude.h"
#include "Events.h"
#include "MapTile.h"

namespace Blockstream
{

	constexpr byte MESSAGE_TYPE_MOVE = 0;
	constexpr byte MESSAGE_TYPE_WIN = 1;
	constexpr byte MOVE_TYPE_PUSH = 0;
	constexpr byte MOVE_TYPE_PULL = 1;

	class GameServer
	{
	public:
		static constexpr int PORT = 3000;

	private:
		TCPsocket m_Socket;
		TCPsocket m_ClientSocket;
		bool m_IsServer;
		bool m_IsRunning;

	public:
		EventDispatcher<ServerEvent> OnClientConnected;
		EventDispatcher<ServerEvent> OnClientDisconnected;
		EventDispatcher<ServerEvent> OnBlockMoved;
		EventDispatcher<ServerEvent> OnPlayerWin;

	public:
		GameServer();

		void RunAsServer(const blt::string& host);
		void RunAsClient(const blt::string& host);
		void Shutdown();

		void MoveBlock(MapTile* tile, bool isPush);
		void SendWin();

		void ListenForMessages();

		void RunServerPrivate(const blt::string& host);
		void RunClientPrivate(const blt::string& host);

		friend class GameManager;

	private:
		void Init();
		void Terminate();
		TCPsocket& GetSocket();

	};

}