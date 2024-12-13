#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include <io.h>
#include<fstream>

#include "FreeCamera.h"
#include "PlayerCamera.h"
#include "GameInstance.h"
#include "GameInterface_Controller.h"

#include "Effect_Container.h"
#include "Effect_Manager.h"
#include "Camera_Manager.h"
#include "StaticObj.h"
#include "Player.h"
#include "Ladder.h"
#include "CutScene.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Paticle()))
		return E_FAIL;	

	if (FAILED(Read_Map_Data()))
		return E_FAIL;	

	if (FAILED(Ready_CutScene_Data()))
		return E_FAIL;

	// 24-11-19 �輺��
	// ���� �������̽��� �ΰ��� ���� ��ȯ
	GET_GAMEINTERFACE->SetIngame(true);


	m_pGameInstance->Set_Listener(m_pPlayer);

	m_pGameInstance->Play_BGM(TEXT("MU_MS_MonasteryA_YK1_Fix.wav"), &g_fBGMVolume);
	m_pGameInstance->Play_ENV(TEXT("AMB_SS_Monastery_Inside_01.wav"), &g_fEnvVolume);

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (KEY_TAP(KEY::F2))
	{
		CCamera_Manager::Get_Instance()->Change_Camera(TEXT("Camera_Free"));
	}

	if (KEY_TAP(KEY::F3))
	{
		CCamera_Manager::Get_Instance()->Change_Camera(TEXT("Camera_Player"));
	}

	if (KEY_TAP(KEY::F4))
	{
		HDR_DESC* tDesc = m_pGameInstance->Get_HDRDesc();
		if (nullptr == tDesc)
			return;

		tDesc->isOnHDR = !tDesc->isOnHDR;

	}

	if (KEY_TAP(KEY::F5))
	{
		CEffect_Manager::Get_Instance()->Add_Effect_ToLayer(LEVEL_GAMEPLAY, TEXT("Player_Attack_Rapier_StormStab_First"),_Vec3(0.f, 5.f, 0.f), _Vec3(0.f, 0.f, 0.f), _Vec3(1.f, 1.f, 1.f));
	}
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("�����÷��̷����Դϴ�."));
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	/* �����÷��� ������ �ʿ��� ������ �غ��Ѵ�. */
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(-1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = _float4(0.5f, 0.5f, 0.5f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof LightDesc);
	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(8.4f, 1.f, 20.5f, 1.f);
	//LightDesc.fRange = 7.f;	
	//LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	//LightDesc.vAmbient = /*_float4(0.4f, 0.2f, 0.2f, 1.f);*/_float4(0.f, 0.f, 0.f, 0.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
	//	return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof LightDesc);
	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(0.f, 10.f, 0.f, 1.f);
	//LightDesc.fRange = 60.f;
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = /*_float4(0.2f, 0.4f, 0.2f, 1.f);*/_float4(0.f, 0.f, 0.f, 0.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera()
{
	CPlayerCamera::CAMERA_PLAYER_DESC PlayerCameraDesc{};
	PlayerCameraDesc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	PlayerCameraDesc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	PlayerCameraDesc.fFovy = XMConvertToRadians(60.0f);
	PlayerCameraDesc.fNear = 0.1f;
	PlayerCameraDesc.fFar = 500.f;
	PlayerCameraDesc.fSpeedPerSec = 30.f;
	PlayerCameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	PlayerCameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	PlayerCameraDesc.pPlayer = m_pPlayer;
	PlayerCameraDesc.fSpeed = 5.f;

	CPlayerCamera* pPlayerCamera = dynamic_cast<CPlayerCamera*>(m_pGameInstance->Get_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_PlayerCamera"), &PlayerCameraDesc));
	if(nullptr == pPlayerCamera)
		return E_FAIL;
	CCamera_Manager::Get_Instance()->Add_Camera(TEXT("Camera_Player"), pPlayerCamera);
	m_pPlayer->Set_Camera(pPlayerCamera);

	CFreeCamera::CAMERA_FREE_DESC		Desc{};

	Desc.fSensor = 0.2f;
	Desc.vEye = _float4(0.f, 0.f, 0.f, 1.f);
	Desc.vAt = _float4(0.f, 0.f, 1.f, 1.f);
	Desc.fFovy = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 500.f;
	Desc.fSpeedPerSec = 30.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	Desc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;

	CCamera* pCamera = dynamic_cast<CCamera*>(m_pGameInstance->Get_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_FreeCamera"), &Desc));
	if (nullptr == pCamera)
		return E_FAIL;
	CCamera_Manager::Get_Instance()->Add_Camera(TEXT("Camera_Free"), pCamera);

	CCamera_Manager::Get_Instance()->Change_Camera(TEXT("Camera_Player"));

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Layer_BackGround()
{

//	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Terrain"))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_NavDataObj"))))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_ForkLift"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect()
{

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster()
{
	/*for (size_t i = 0; i < 10; i++)
	{
		if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Monster"))))
			return E_FAIL;
	}*/
	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_CarcassBigA"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_CarcassTail"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_SimonManus"), TEXT("Prototype_GameObject_SimonManus"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Raxasia"), TEXT("Prototype_GameObject_Raxasia"))))
		//return E_FAIL;

	// �ذ� �����ұ�????>???????
	//for (_uint i = 1; i < 2; ++i)
	//{
	//	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_CutScene_SimonManus_EnvHand"), &i)))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_CutScene_SimonManus_GodHand"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Paticle()
{
	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Particle_Expolosion"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Particle_Snow"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player()
{
	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player")));
	Safe_AddRef(m_pPlayer);

	return S_OK;
}

HRESULT CLevel_GamePlay::Read_Map_Data()
{
	_Vec3	vMinPos = { FLT_MAX ,FLT_MAX ,FLT_MAX };		//World_Octree�� �ּ� ��ǥ, �ִ��� ũ�� �ʱ�ȭ
	_Vec3	vMaxPos = { -FLT_MAX ,-FLT_MAX ,-FLT_MAX };	//World_Octree�� �ִ� ��ǥ, �ִ��� �۰� �ʱ�ȭ

	const char cFile[128] = "../Bin/DataFiles/Map_Data.dat";
	ifstream fin(cFile, ios::in | ios::binary);

	//	fin.open("../Bin/Map_Data.txt");
	if (!fin.is_open())    // ���� �����ٸ�
	{
		MSG_BOX(TEXT("���� �б⸦ �����߾��.."));
		return E_FAIL;
	}

	string line;
	_uint LayerCount = { 0 };
	fin.read(reinterpret_cast<char*>(&LayerCount), sizeof(LayerCount));	//���̾� ����

	string strLayerTag = {};
	std::string::size_type iStrSize = { 0 };
	_uint iObjectCount = 0;
	int i = 0;
	int iObjectType = 0;
	int iObjectListIndex = 0;
	_float3 fPos = {};
	_float3 fScaled = {};
	_float3 fRot = {};
	_int iCellNum = {};
	_uint iCellRoomNum = {};

	while (i < (_int)LayerCount)
	{
		fin.read(reinterpret_cast<char*>(&iStrSize), sizeof(iStrSize));
		strLayerTag.resize(iStrSize);
		fin.read(&strLayerTag[0], iStrSize);
		fin.read(reinterpret_cast<char*>(&iObjectCount), sizeof(iObjectCount));

		for (_int j = 0; j < (_int)iObjectCount; ++j)
		{
			if (strLayerTag != "Layer_Light")	//���� ���� ��� ������Ʈ �ҷ�����
			{
				OBJECT_DEFAULT_DESC pDesc = {};
				fin.read(reinterpret_cast<char*>(&pDesc), sizeof(pDesc));

				if (strLayerTag == "Layer_Map" || strLayerTag == "Layer_Etc")
				{
					if (wcscmp(pDesc.szModelTag, TEXT("SM_Monastery_Lift_01_Bottom")) == 0)
					{
						if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Lift_Floor"), TEXT("Prototype_GameObject_Lift_Floor"), &pDesc)))
							return E_FAIL;
						continue;
					}

					if (wcscmp(pDesc.szModelTag, TEXT("SM_Monastery_SteppingStone_01")) == 0)
					{
						if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_SteppingStone"), TEXT("Prototype_GameObject_SteppingStone"), &pDesc)))
							return E_FAIL;
						continue;
					}

					if (wcscmp(pDesc.szModelTag, TEXT("Sophia_Stoned")) == 0)
					{
						if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Map"), TEXT("Prototype_GameObject_Sophia"), &pDesc)))
							return E_FAIL;
						continue;
					}

					CStaticObj::STATICOBJ_DESC staticObjDesc = {};
					int bufferSize = WideCharToMultiByte(CP_ACP, 0, pDesc.szModelTag, -1, NULL, 0, NULL, NULL);
					WideCharToMultiByte(CP_ACP, 0, pDesc.szModelTag, -1, staticObjDesc.szModelTag, bufferSize, NULL, NULL);

					staticObjDesc.vPosition = pDesc.vPosition;
					staticObjDesc.vScale = pDesc.vScale;
					staticObjDesc.vRotation = pDesc.vRotation;
					staticObjDesc.isInstance = pDesc.isInstance;
					staticObjDesc.iRenderGroupID = pDesc.iID;
					staticObjDesc.bShadow = pDesc.bShadow;
					staticObjDesc.iCurrentCellNum = pDesc.iCurrentCellNum;
					if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Map"), TEXT("Prototype_GameObject_StaticObj"), &staticObjDesc)))
						return E_FAIL;

					vMinPos.x = min(vMinPos.x, pDesc.vPosition.x);
					vMinPos.y = min(vMinPos.y, pDesc.vPosition.y);
					vMinPos.z = min(vMinPos.z, pDesc.vPosition.z);
					vMaxPos.x = max(vMaxPos.x, pDesc.vPosition.x);
					vMaxPos.y = max(vMaxPos.y, pDesc.vPosition.y);
					vMaxPos.z = max(vMaxPos.z, pDesc.vPosition.z);
				} 

				else if (strLayerTag == "Layer_InteractObject")
				{
					 if (wcscmp(pDesc.szModelTag, TEXT("SK_DLV_Stargazer_01")) == 0)
					 {
						 if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Stargazer"), TEXT("Prototype_GameObject_Stargazer"), &pDesc)))
							 return E_FAIL;
					 }
					 else if (wcscmp(pDesc.szModelTag, TEXT("SK_FO_Monastery_Lift_01_Controller")) == 0)
					 {
						 if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_InteractObj"), TEXT("Prototype_GameObject_Lift_Controller"), &pDesc)))
							 return E_FAIL;
					 }
					 else if (wcscmp(pDesc.szModelTag, TEXT("SK_NewTown_Lift_01_Door")) == 0)
					 {
						 if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_LiftDoor"), TEXT("Prototype_GameObject_Lift_Door"), &pDesc)))
							 return E_FAIL;
					 }
					 else if (wcscmp(pDesc.szModelTag, TEXT("SK_FO_Monastery_TowerDoor_01")) == 0)
					 {
						 if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_InteractObj"), TEXT("Prototype_GameObject_TowerDoor"), &pDesc)))
							 return E_FAIL;
					 }	 
					 else if (wcscmp(pDesc.szModelTag, TEXT("SK_FO_Monastery_WallDeco_01_Scupture04")) == 0)
					 {
						 if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_InteractObj"), TEXT("Prototype_GameObject_WallDeco"), &pDesc)))
							 return E_FAIL;
					 }
					 else if (wcscmp(pDesc.szModelTag, TEXT("SK_FO_TreasureChest_02_Red")) == 0)
					 {
						 if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_InteractObj"), TEXT("Prototype_GameObject_TreasureBox"), &pDesc)))
							 return E_FAIL;
					 }
					 else if (wcscmp(pDesc.szModelTag, TEXT("SK_FO_Monastery_TheLastDoor_01")) == 0)
					 {
						 if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_LastDoor"), TEXT("Prototype_GameObject_LastDoor"), &pDesc)))
							 return E_FAIL;
					 }
					 else if (wcscmp(pDesc.szModelTag, TEXT("SK_LV_Ladder_MetalWood_01_KSJ")) == 0 || wcscmp(pDesc.szModelTag, TEXT("SK_LV_Ladder_MetalWood_Slide6m_SM_KSJ")) == 0 )
					 {
						 CLadder::LADDER_DESC laderDesc = {};
						 int bufferSize = WideCharToMultiByte(CP_ACP, 0, pDesc.szModelTag, -1, NULL, 0, NULL, NULL);
						 WideCharToMultiByte(CP_ACP, 0, pDesc.szModelTag, -1, laderDesc.szModelTag, bufferSize, NULL, NULL);
						 laderDesc.vPosition = pDesc.vPosition;
						 laderDesc.vScale = pDesc.vScale;
						 laderDesc.vRotation = pDesc.vRotation;
						 laderDesc.bShadow = pDesc.bShadow;
						 laderDesc.iRenderGroupID = pDesc.iID;
						 if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_InteractObj"), TEXT("Prototype_GameObject_Ladder"), &laderDesc)))
							 return E_FAIL;
					 }
				}
				else if (strLayerTag == "Layer_Monster")
				{
					if (wcscmp(pDesc.szModelTag, TEXT("Prototype_AnimModel_CarcassTail")) == 0)
					{
						if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_CarcassTail"), &pDesc)))
							return E_FAIL;
					}
					if (wcscmp(pDesc.szModelTag, TEXT("Prototype_AnimModel_CarcassBigA")) == 0)
					{
						if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_CarcassBigA"), &pDesc)))
							return E_FAIL;
					}
				}
			}
			else
			{
				LIGHT_DESC pDesc = {};

				fin.read(reinterpret_cast<char*>(&pDesc), sizeof(pDesc));
				if (FAILED(m_pGameInstance->Add_Light(pDesc)))
					return E_FAIL;
			}
		}
		++i;
	}
	fin.close();

	m_pGameInstance->Create_Octree(vMinPos, vMaxPos);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_CutScene_Data()
{
	const char cFile[128] = "../Bin/DataFiles/CutScene_Data.dat";
	ifstream fin(cFile, ios::in | ios::binary);

	//	fin.open("../Bin/Map_Data.txt");
	if (!fin.is_open())    // ���� �����ٸ�
	{
		MSG_BOX(TEXT("���� �б⸦ �����߾��.."));
		return E_FAIL;
	}

	_uint CutSceneCount = { 0 };
	fin.read(reinterpret_cast<char*>(&CutSceneCount), sizeof(_uint));

	for (_uint i = 0; i < CutSceneCount; ++i)
	{
		CCutScene* pCutScene = dynamic_cast<CCutScene*>(m_pGameInstance->Get_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_CutScene"), TEXT("Prototype_GameObject_CutScene")));

		//�ƽ� �ε���
		_uint iCutScene_Index;
		fin.read(reinterpret_cast<char*>(&iCutScene_Index), sizeof(_uint));
		pCutScene->Set_CutSceneIndex(iCutScene_Index);

		//�ƽ��� ��ü ������ (�ð�)
		_float fMaxFrame;
		fin.read(reinterpret_cast<char*>(&fMaxFrame), sizeof(_float));
		pCutScene->Set_MaxFrame(fMaxFrame);

		//�ƽ��� KeyFrame����(�̺�Ʈ ����)
		_int iKeyFrameCount;
		fin.read(reinterpret_cast<char*>(&iKeyFrameCount), sizeof(_int));

		for (int j = 0; j < iKeyFrameCount; ++j)
		{
			CUTSCENE_KEYFRAME_DESC pDesc = {};
			fin.read(reinterpret_cast<char*>(&pDesc), sizeof(CUTSCENE_KEYFRAME_DESC));
			pCutScene->Load_KeyFrame(pDesc);
		}
	}

	fin.close();
	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();


	m_pGameInstance->Set_Listener(nullptr);
	// �ν��Ͻ��� �� �𵨵��� ��Ƶ� �Ŵ��� Ŭ�����ϱ�
	Safe_Release(m_pPlayer);
	m_pGameInstance->Clear_Instance();
	CCamera_Manager::Get_Instance()->Destroy_Instance();
}
