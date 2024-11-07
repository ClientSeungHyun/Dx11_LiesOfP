#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "FreeCamera.h"
#include "GameInstance.h"

CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_Tool::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera()))
		return E_FAIL;
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Effect()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Monster()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;
	if (FAILED(Ready_Layer_Paticle()))
		return E_FAIL;

	
	


	return S_OK;
}

void CLevel_Tool::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_Tool::Render()
{
	SetWindowText(g_hWnd, TEXT("�����÷��̷����Դϴ�."));
	return S_OK;
}

HRESULT CLevel_Tool::Ready_Lights()
{
	/* �����÷��� ������ �ʿ��� ������ �غ��Ѵ�. */
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	_Matrix CascadeViewMatrix[3];
	_Matrix CascadeProjMatrix[3];
	_Vec4 vLightPos = _Vec4(0.f, 500.f, 0.f, 1.f);

	// �� ĳ�����̵尡 ��Ÿ�� ���� ���� ����.
	_float fCascadeSplits[3] = { 0.0f };
	_float fNearPlane = 0.1f;
	_float fFarPlane = 1000.0f;
	_float delta = fFarPlane - fNearPlane;

	for (_uint i = 0; i < 3; ++i) {
		float lambda = (i + 1) / 3.f;
		fCascadeSplits[i] = fNearPlane + lambda * delta;
	}


	for (_uint i = 0; i < 3; ++i)
	{
		// �� ĳ�����̵��� near plane�� far plane ���
		float fCascadeNear = fCascadeSplits[i];
		float fCascadeFar = (i + 1 < 3) ? fCascadeSplits[i + 1] : fFarPlane;

		// 1. �� ��� ��� (���� ��ġ�� ��������)
		XMMATRIX viewMatrix = XMMatrixLookAtLH(
			vLightPos,  // ���� ��ġ
			_Vec4(0.f, 0.f, 0.f, 1.f),  // ���� ��ǥ ��ġ (��: ����)
			_Vec4(0.f, 1.f, 0.f, 0.f)   // �� ����
		);

		CascadeViewMatrix[i] = viewMatrix;

		// 2. ���� ��� ��� (�� ĳ�����̵��� near, far ������ �°�)
		XMMATRIX projMatrix = XMMatrixOrthographicLH(
			1280.f,  // ������ ũ��, ���ϴ� ������ ���� (��: ���� ũ��)
			720.f,  // ������ ũ��
			fCascadeNear,  // near plane
			fCascadeFar    // far plane
		);

		CascadeProjMatrix[i] = projMatrix;
	}

	m_pGameInstance->Set_CascadeViewMatirx(CascadeViewMatrix);
	m_pGameInstance->Set_CascadeProjMatirx(CascadeProjMatrix);


	//ZeroMemory(&LightDesc, sizeof LightDesc);
	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(10.f, 3.f, 10.f, 1.f);
	//LightDesc.fRange = 7.f;	
	//LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	//LightDesc.vAmbient = /*_float4(0.4f, 0.2f, 0.2f, 1.f);*/_float4(0.f, 0.f, 0.f, 0.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
	//	return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof LightDesc);
	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(20.f, 3.f, 10.f, 1.f);
	//LightDesc.fRange = 7.f;
	//LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	//LightDesc.vAmbient = /*_float4(0.2f, 0.4f, 0.2f, 1.f);*/_float4(0.f, 0.f, 0.f, 0.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
	//	return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Camera()
{
	CFreeCamera::CAMERA_FREE_DESC		Desc{};

	Desc.fSensor = 0.2f;
	Desc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
	Desc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	Desc.fFovy = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.fSpeedPerSec = 30.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	Desc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_FreeCamera"), &Desc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Tool::Ready_Layer_BackGround()
{
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Sky"))))
		return E_FAIL;


	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_ForkLift"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Effect()
{

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Monster()
{
	/*for (size_t i = 0; i < 10; i++)
	{
		if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Monster"))))
			return E_FAIL;
	}*/

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Paticle()
{
	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Particle_Expolosion"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Particle"), TEXT("Prototype_GameObject_Particle_Snow"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_Player()
{
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Prototype_GameObject_Player"))))
		return E_FAIL;

	return S_OK;
}

CLevel_Tool * CLevel_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Tool*		pInstance = new CLevel_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Tool"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tool::Free()
{
	__super::Free();

}
