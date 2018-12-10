#include "GameServer.h"
#include "GameManager.h"

namespace Blockstream
{

	GameServer::GameServer()
	{
		
	}

	void GameServer::RunAsServer(const blt::string& host)
	{
		m_IsServer = true;
		Init();
		std::thread thread = std::thread(std::bind(&GameServer::RunServerPrivate, this, std::placeholders::_1), host);
		thread.detach();
	}

	void GameServer::RunAsClient(const blt::string& host)
	{
		m_IsServer = false;
		Init();
		std::thread thread = std::thread(std::bind(&GameServer::RunClientPrivate, this, std::placeholders::_1), host);
		thread.detach();
	}

	void GameServer::Shutdown()
	{
		m_IsRunning = false;
		m_Socket.Close();
		if (m_IsServer)
		{
			m_ClientSocket.Close();
		}
	}

	void GameServer::MoveBlock(MapTile* tile, bool isPush)
	{
		auto pair = GameManager::Map().GetCoordOfTile(tile);
		byte message[4];
		message[0] = MESSAGE_TYPE_MOVE;
		message[1] = pair.first;
		message[2] = pair.second;
		message[3] = (isPush) ? MOVE_TYPE_PULL : MOVE_TYPE_PUSH;
		GetSocket().Send(message, 4);
	}

	void GameServer::SendWin()
	{
		byte message[4];
		message[0] = MESSAGE_TYPE_WIN;
		GetSocket().Send(message, 4);
	}

	void GameServer::ListenForMessages()
	{
		while (m_IsRunning)
		{
			byte message[4];
			int received = GetSocket().Recv(message, 4);
			if (received == 4)
			{
				byte type = message[0];
				switch (type)
				{
				case MESSAGE_TYPE_MOVE:
				{
					MapTile* tile = GameManager::Map().GetTile(message[1], message[2]);
					(message[3] == MOVE_TYPE_PUSH) ? GameManager::DecrementTile(tile) : GameManager::IncrementTile(tile);
					break;
				}
				case MESSAGE_TYPE_WIN:
				{
					std::unique_ptr<ServerEvent> args = std::make_unique<ServerEvent>();
					args->m0 = 1; // loss
					OnPlayerWin.Post(std::move(args));
					break;
				}
				}
			}
			else if (received < 0 && m_IsRunning)
			{
				OnClientDisconnected.Post();
				break;
			}
		}
	}

	void GameServer::Init()
	{
		m_IsRunning = true;
		OnClientConnected = EventDispatcher<ServerEvent>(SERVER_CLIENT_CONNECTED);
		OnClientDisconnected = EventDispatcher<ServerEvent>(SERVER_CLIENT_DISCONNECTED);
		OnBlockMoved = EventDispatcher<ServerEvent>(SERVER_TILE_MOVE_EVENT);
		OnPlayerWin = EventDispatcher<ServerEvent>(SERVER_PLAYER_WIN_EVENT);

		m_Socket = TCPsocket();
	}

	void GameServer::Terminate()
	{
		OnClientConnected.Destroy();
		OnClientDisconnected.Destroy();
		OnBlockMoved.Destroy();
		OnPlayerWin.Destroy();
	}

	void GameServer::RunServerPrivate(const blt::string& host)
	{
		m_Socket.Bind(SocketAddress(host, PORT));
		m_Socket.Listen(5);
		SocketAddress clientAddr;
		m_ClientSocket = m_Socket.Accept(&clientAddr);
		if (m_ClientSocket.IsValid())
		{
			GameManager::SetStartTile(GameManager::Map().GetTile(4, 0));
			GameManager::SetEndTile(GameManager::Map().GetTile(4, 8));

			byte message[4];
			message[0] = 0;
			message[1] = 4;
			message[2] = 8;
			message[3] = 4;
			GetSocket().Send(message, 4);

			OnClientConnected.Post();
			std::thread thread = std::thread(std::bind(&GameServer::ListenForMessages, this));
			thread.detach();
		}
	}

	void GameServer::RunClientPrivate(const blt::string& host)
	{
		m_Socket.Connect(SocketAddress(host, PORT));

		byte message[4];
		GetSocket().Recv(message, 4);

		GameManager::SetStartTile(GameManager::Map().GetTile(message[0], message[1]));
		GameManager::SetEndTile(GameManager::Map().GetTile(message[2], message[3]));
		OnClientConnected.Post();
		std::thread thread = std::thread(std::bind(&GameServer::ListenForMessages, this));
		thread.detach();
	}

	TCPsocket& GameServer::GetSocket()
	{
		return (m_IsServer) ? m_ClientSocket : m_Socket;
	}

}