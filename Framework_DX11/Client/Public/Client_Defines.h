#pragma once

#include <process.h>

namespace Client
{
	const unsigned int			g_iWinSizeX = { 1280 };
	const unsigned int			g_iWinSizeY = { 720 };

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

	struct FSM_INIT_DESC
	{
		double* pPrevTrackPos{ nullptr };
	};

	// 24-11-09 �輺�� : GameInterface �̱��� ���� ��ũ�� 
	#define GET_GAMEINTERFACE	CGameInterface_Controller::Get_Instance()
	// 24-11-23 �輺�� : UI �ؽ��� �ε��� �ִ�ġ
	// �ؽ��� ���� ���� ������ �ݵ�� ������ �� 
	#define MAX_TEXTURES_UI 653
	#define MAX_TEXTURES_ITEM 334

	#define NONE_TEXT TEXT("None")

}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;
using namespace std;
