#include "stdafx.h"
#include "..\Public\UIPage_Test.h"

#include "GameInstance.h"


CUIPage_Test::CUIPage_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{

}

CUIPage_Test::CUIPage_Test(const CUIPage_Test& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Test::Initialize_Prototype()
{
	// ��� UIPage�� Ŭ�� �� ��, ���⼭ ��� ���� ������ 

	UI_DESC			Desc{};

	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;
	Desc.fSizeX = g_iWinSizeX;
	Desc.fSizeY = g_iWinSizeY;

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);

	m_vecPageAction.resize(_int(PAGEACTION::ACTION_END));

	/* ���������� ���� �����͵��� ��� ���ϳ�. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CUIPage_Test::Initialize(void* pArg)
{


	return S_OK;
}

void CUIPage_Test::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUIPage_Test::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUIPage_Test::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUIPage_Test::Render()
{
	return S_OK;
}

void CUIPage_Test::OpenAction()
{
	__super::OpenAction();
}

void CUIPage_Test::CloseAction()
{
	__super::CloseAction();
}

CHECK_MOUSE CUIPage_Test::Check_Mouse_By_Part_In_Page()
{
	__super::Check_Mouse_By_Part_In_Page();

	return CHECK_MOUSE::MOUSE_NONE;
}

HRESULT CUIPage_Test::Ready_UIPart_Group_Control()
{
	__super::Ready_UIPart_Group_Control();

	return S_OK;
}

CUIPage_Test* CUIPage_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Test* pInstance = new CUIPage_Test(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Test"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIPage_Test::Clone(void* pArg)
{
	CUIPage_Test* pInstance = new CUIPage_Test(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Test"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Test::Free()
{
	__super::Free();

	for (auto& iter : m_vecPart)
	{
		Safe_Delete(iter);
	}

	m_vecPart.clear();
}
