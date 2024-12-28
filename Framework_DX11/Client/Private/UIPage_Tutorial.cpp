#include "stdafx.h"
#include "..\Public\UIPage_Tutorial.h"

#include "GameInstance.h"
#include "GameInterface_Controller.h"

CUIPage_Tutorial::CUIPage_Tutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{

}

CUIPage_Tutorial::CUIPage_Tutorial(const CUIPage_Tutorial& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Tutorial::Initialize_Prototype()
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

	m_fTopPartMove = -1.f;

	return S_OK;
}

HRESULT CUIPage_Tutorial::Initialize(void* pArg)
{


	return S_OK;
}

void CUIPage_Tutorial::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUIPage_Tutorial::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUIPage_Tutorial::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	for (auto& iter : m_vec_Group_Ctrl)
		__super::UpdatePart_ByControl(iter);
}

HRESULT CUIPage_Tutorial::Render()
{
	return S_OK;
}

void CUIPage_Tutorial::OpenAction()
{
	__super::OpenAction();
}

void CUIPage_Tutorial::CloseAction()
{
	__super::CloseAction();
}

CHECK_MOUSE CUIPage_Tutorial::Check_Page_Action(_float fTimeDelta)
{
	__super::Check_Page_Action(fTimeDelta);

	return CHECK_MOUSE::MOUSE_NONE;
}

HRESULT CUIPage_Tutorial::Ready_UIPart_Group_Control()
{
	__super::Ready_UIPart_Group_Control();

	m_vec_Group_Ctrl.resize(_int(PART_GROUP::GROUP_END));

	for (auto& iter : m_vec_Group_Ctrl)
		iter = new UG_CTRL;

	for (_int i = 0; i < m_vecPart.size(); ++i)
	{
		if (m_vecPart[i]->iGroupIndex != -1)
			m_vec_Group_Ctrl[m_vecPart[i]->iGroupIndex]->PartIndexlist.push_back(i);
	}

	m_bRender = false;

	return S_OK;
}

CUIPage_Tutorial* CUIPage_Tutorial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Tutorial* pInstance = new CUIPage_Tutorial(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Tutorial"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIPage_Tutorial::Clone(void* pArg)
{
	CUIPage_Tutorial* pInstance = new CUIPage_Tutorial(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Tutorial"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Tutorial::Free()
{
	__super::Free();

	for (auto& iter : m_vecPart)
	{
		Safe_Delete(iter);
	}

	m_vecPart.clear();
}
