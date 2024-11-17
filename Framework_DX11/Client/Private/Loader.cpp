#include "stdafx.h"
#include "..\Public\Loader.h"
#include "ThreadPool.h"
#include <io.h>

#include "ColliderObject.h"

#include "Sky.h"
#include "Player.h"
#include "Terrain.h"
#include "FreeCamera.h"
#include "BackGround.h"
#include "StaticObj.h"
#include "NavDataObj.h"

#pragma region EFFECT
#include "Monster.h"
#include "CarcassBigA.h"
#include "SimonManusP1.h"
#pragma endregion

#pragma region EFFECT
#include "Effect_Manager.h"
#pragma endregion

#pragma region OBJECT
#include "Stargazer.h"
#include "Weapon_Rapier.h"
#include "Weapon_FlameSword.h"
#include "Weapon_Scissor.h"
#pragma endregion

#include "GameInstance.h"

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}


_uint APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if(FAILED(pLoader->Loading()))
		return 1;
		
	return 0;
}

_uint APIENTRY LoadingMap0(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->LoadingMapModel0()))
		return 1;

	return 0;
}

_uint APIENTRY LoadingMap1(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->LoadingMapModel1()))
		return 1;

	return 0;
}

_uint APIENTRY LoadingMonster(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->LoadingMonsterModel1()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	// ���߿� ���Ÿ� �������� �����
	//m_pThreadPool = CThreadPool::Get_Instance();

	//m_pThreadPool->Initialize();
	//m_pThreadPool->Start_Work();
	
	InitializeCriticalSection(&m_CriticalSection_Main);
	m_hThread_Main = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread_Main)
		return E_FAIL;

	//InitializeCriticalSection(&m_CriticalSection_Map0);
	//m_hThread_Map0 = (HANDLE)_beginthreadex(nullptr, 0, LoadingMap0, this, 0, nullptr);
	//if (0 == m_hThread_Map0)
	//	return E_FAIL;
	m_isFinished_Map0 = true;

	InitializeCriticalSection(&m_CriticalSection_Map1);
	m_hThread_Map1 = (HANDLE)_beginthreadex(nullptr, 0, LoadingMap1, this, 0, nullptr);
	if (0 == m_hThread_Map1)
		return E_FAIL;

	InitializeCriticalSection(&m_CriticalSection_Monster);
	m_hThread_Monster = (HANDLE)_beginthreadex(nullptr, 0, LoadingMonster, this, 0, nullptr);
	if (0 == m_hThread_Monster)
		return E_FAIL;

	if (m_eNextLevelID == LEVEL_LOGO)
	{
		m_isFinished_Map0 = true;
		m_isFinished_Map1 = true;
		m_isFinished_Monster = true;
	}

	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection_Main);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Ready_Resources_For_LogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		hr = Ready_Resources_For_GamePlayLevel();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection_Main);
	

	return hr;
}

HRESULT CLoader::LoadingMapModel0()
{
	EnterCriticalSection(&m_CriticalSection_Map0);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_GAMEPLAY:
		hr = Ready_Resources_For_GamePlayLevel_Map0();
		break;
	default:
		hr = S_OK;
	}

	if (hr == E_FAIL)
		MSG_BOX(TEXT("�� �ε�0 ���� ����"));

	LeaveCriticalSection(&m_CriticalSection_Map0);

	return hr;
}

HRESULT CLoader::LoadingMapModel1()
{
	EnterCriticalSection(&m_CriticalSection_Map1);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_GAMEPLAY:
		hr = Ready_Resources_For_GamePlayLevel_Map1();
		break;
	default:
		hr = S_OK;
	}

	if (hr == E_FAIL)
		MSG_BOX(TEXT("�� �ε�1 ���� ����"));

	LeaveCriticalSection(&m_CriticalSection_Map1);

	return hr;
}

HRESULT CLoader::LoadingMonsterModel1()
{
	EnterCriticalSection(&m_CriticalSection_Monster);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_GAMEPLAY:
		hr = Ready_Resources_For_GamePlayLevel_Monster();
		break;
	default:
		hr = S_OK;
	}

	if (hr == E_FAIL)
		MSG_BOX(TEXT("���� �ε�0 ���� ����"));

	LeaveCriticalSection(&m_CriticalSection_Monster);

	return hr;
}

void CLoader::Draw_LoadingText()
{
	if(isFinished())
		lstrcpy(m_szLoadingText, TEXT("�ε� �Ϸ�"));

	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Ready_Resources_For_LogoLevel()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����Դϴ�."));
	/* For. Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("��ü������(��) �ε����Դϴ�."));
	/* For. Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	m_isFinished_Main = true;

	return S_OK;
}

HRESULT CLoader::Ready_Resources_For_GamePlayLevel()
{
	///* For. Prototype_Component_Texture_Terrain*/
	//for (_uint i = 0; i < 200; ++i)
	//{
	//	_wstring ex = TEXT("Prototype_Component_Texture_Terrain");
	//	_wstring num = to_wstring(i);
	//	ex = ex + num;
	//	auto textureFuture = CThreadPool::Get_Instance()->Add_Work([this, ex]() {
	//		return m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, ex,
	//		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2));
	//		});
	//	if (textureFuture.get() == E_FAIL)
	//		return textureFuture.get();
	//}

	lstrcpy(m_szLoadingText, TEXT("����Ʈ �Ŵ����� �ε����Դϴ�."));
	if(FAILED(CEffect_Manager::Get_Instance()->Initialize(m_pDevice, m_pContext, TEXT("../../Tools/Bin/DataFiles/Effect"))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε����Դϴ�."));

	/* For. Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Brush*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Mask.bmp"), 1))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;


	/* For. Prototype_Component_Texture_Particle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Particle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_Explosion */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Explosion/Explosion%d.png"), 90))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("����(��) �ε����Դϴ�."));


	if (FAILED(Ready_Resources_For_Player()))
		return E_FAIL;

	if (FAILED(Ready_Resources_For_Monster()))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("�׺���̼���(��) �ε����Դϴ�."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Nav_Data.dat")))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("������(��) �ε����Դϴ�."));
	
	m_isFinished_Main = true;

	return S_OK;
}


HRESULT CLoader::Ready_Resources_For_GamePlayLevel_Map0()
{

	m_isFinished_Map0 = true;

	return S_OK;
}

HRESULT CLoader::Ready_Resources_For_GamePlayLevel_Map1()
{
	_matrix      PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f);

	// _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
	_finddata_t fd;

	// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�
	intptr_t handle = _findfirst("../Bin/ModelData/NonAnim/Map/Temp/*", &fd);

	//if (handle == -1)
	//	return E_FAIL;

	int iResult = 0;

	_wstring strPrototype = TEXT("");

	if (handle == -1)
		return E_FAIL;

	char szCurPath3[128] = "../Bin/ModelData/NonAnim/Map/Temp/";    // ��� ���
	char szFullPath3[128] = "";

	iResult = 0;

	while (iResult != -1)
	{
		strcpy_s(szFullPath3, szCurPath3);
		strcat_s(szFullPath3, fd.name);

		_char szFileName[MAX_PATH] = "";
		_char szExt[MAX_PATH] = "";
		_splitpath_s(szFullPath3, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

		if (!strcmp(fd.name, ".") || !strcmp(fd.name, "..")
			|| strcmp(szExt, ".dat"))
		{
			iResult = _findnext(handle, &fd);
			continue;
		}

		string strFileName = szFileName;
		_wstring strPrototypeName;

		strPrototypeName.assign(strFileName.begin(), strFileName.end());
		wprintf(strPrototypeName.c_str());

		PreTransformMatrix = XMMatrixIdentity();
		PreTransformMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f);

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, strPrototypeName,
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, szFullPath3, PreTransformMatrix))))
			return E_FAIL;

		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _findnext(handle, &fd);
	}

	handle = _findfirst("../Bin/ModelData/NonAnim/Map/Structure/*", &fd);

	if (handle == -1)
		return E_FAIL;

	iResult = 0;

	char szCurPath4[128] = "../Bin/ModelData/NonAnim/Map/Structure/";    // ��� ���
	char szFullPath4[128] = "";


	while (iResult != -1)
	{
		strcpy_s(szFullPath4, szCurPath4);
		strcat_s(szFullPath4, fd.name);

		_char szFileName[MAX_PATH] = "";
		_char szExt[MAX_PATH] = "";
		_splitpath_s(szFullPath4, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

		if (!strcmp(fd.name, ".") || !strcmp(fd.name, "..")
			|| strcmp(szExt, ".dat"))
		{
			iResult = _findnext(handle, &fd);
			continue;
		}

		string strFileName = szFileName;
		_wstring strPrototypeName;

		strPrototypeName.assign(strFileName.begin(), strFileName.end());
		wprintf(strPrototypeName.c_str());

		PreTransformMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f);

		//if (strPrototypeName == TEXT("SM_Monstery_Machine_01A"))
		//	int a = 0;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, strPrototypeName,
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, szFullPath4, PreTransformMatrix))))
			return E_FAIL;

		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _findnext(handle, &fd);
	}

	handle = _findfirst("../Bin/ModelData/NonAnim/Map/Interior/*", &fd);

	if (handle == -1)
		return E_FAIL;

	iResult = 0;

	char szCurPath[128] = "../Bin/ModelData/NonAnim/Map/Interior/";    // ��� ���
	char szFullPath[128] = "";


	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);

		_char szFileName[MAX_PATH] = "";
		_char szExt[MAX_PATH] = "";
		_splitpath_s(szFullPath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

		if (!strcmp(fd.name, ".") || !strcmp(fd.name, "..")
			|| strcmp(szExt, ".dat"))
		{
			iResult = _findnext(handle, &fd);
			continue;
		}

		string strFileName = szFileName;
		_wstring strPrototypeName;

		strPrototypeName.assign(strFileName.begin(), strFileName.end());
		wprintf(strPrototypeName.c_str());

		PreTransformMatrix = XMMatrixScaling(0.005f, 0.005f, 0.005f);

		//if (strPrototypeName == TEXT("SM_Monstery_Machine_01A"))
		//	int a = 0;
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, strPrototypeName,
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, szFullPath, PreTransformMatrix))))
			return E_FAIL;

		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _findnext(handle, &fd);
	}


	m_isFinished_Map1 = true;

	return S_OK;
}

HRESULT CLoader::Ready_Resources_For_GamePlayLevel_Monster()
{
	_matrix		PreTransformMatrix = XMMatrixIdentity();

	m_isFinished_Monster = true;

	return S_OK;
}

HRESULT CLoader::Ready_Resources_For_Player()
{
	_matrix		PreTransformMatrix = XMMatrixIdentity();

	/* Prototype_Component_Model_Player */
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/ModelData/Anim/Player/Player.dat", PreTransformMatrix, false))))
		return E_FAIL;

	/* Prototype_Component_Model_Rapier */
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Rapier"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/ModelData/NonAnim/Weapon/Rapier.dat", PreTransformMatrix, false))))
		return E_FAIL;

	/* Prototype_Component_Model_FlameSword */
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_FlameSword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/ModelData/NonAnim/Weapon/FlameSword.dat", PreTransformMatrix, false))))
		return E_FAIL;

	/* Prototype_Component_Model_Scissor_Combine */
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Scissor_Combine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/ModelData/NonAnim/Weapon/Scissor_Combine.dat", PreTransformMatrix, false))))
		return E_FAIL;

	/* Prototype_Component_Model_Scissor_Left */
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Scissor_Left"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/ModelData/NonAnim/Weapon/Scissor_Left.dat", PreTransformMatrix, false))))
		return E_FAIL;

	///* Prototype_Component_Model_Scissor_Right */
	//PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Scissor_Right"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/ModelData/NonAnim/Weapon/Scissor_Right.dat", PreTransformMatrix, false))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Resources_For_Monster()
{
	_matrix		PreTransformMatrix = XMMatrixIdentity();

	//Prototype_Component_Model_CarcassBigA
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CarcassBigA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/ModelData/Anim/CreatedBinFiles/CarcassBigA.dat", PreTransformMatrix, true))))
		return E_FAIL;

	/* For. Prototype_Component_Model_Stargazer*/
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stargazer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/ModelData/NonAnim/InteractObj/SK_DLV_Stargazer_01.dat", PreTransformMatrix))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Ready_Prototype()
{
	/* For. Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_FreeCamera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FreeCamera"),
		CFreeCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region PLAYER
	/* For. Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_Weapon_Rapier */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Rapier"),
		CWeapon_Rapier::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_Weapon_FlameSword */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_FlameSword"),
		CWeapon_FlameSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_Weapon_Scissor */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Scissor"),
		CWeapon_Scissor::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region MONSTER

	/* For. Prototype_GameObject_CarcassBigA */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CarcassBigA"),
		CCarcassBigA::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_SimonManusP1 */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SimonManusP1"),
		CSimonManusP1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region Collider
	/* For. Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ColliderObj"),
		CColliderObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

	/* For. Prototype_GameObject_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_StaticObj */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticObj"),
		CStaticObj::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_StaticObj */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NavDataObj"),
		CNavDataObj::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_Stargazer */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Stargazer"),
		CStargazer::Create(m_pDevice, m_pContext))))
		return E_FAIL;




	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread_Main, INFINITE);
	WaitForSingleObject(m_hThread_Map0, INFINITE);
	WaitForSingleObject(m_hThread_Map1, INFINITE);
	WaitForSingleObject(m_hThread_Monster, INFINITE);

	DeleteObject(m_hThread_Main);
	DeleteObject(m_hThread_Map0);
	DeleteObject(m_hThread_Map1);
	DeleteObject(m_hThread_Monster);

	CloseHandle(m_hThread_Main);
	CloseHandle(m_hThread_Map0);
	CloseHandle(m_hThread_Map1);
	CloseHandle(m_hThread_Monster);

	DeleteCriticalSection(&m_CriticalSection_Main);
	DeleteCriticalSection(&m_CriticalSection_Map0);
	DeleteCriticalSection(&m_CriticalSection_Map1);
	DeleteCriticalSection(&m_CriticalSection_Monster);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
