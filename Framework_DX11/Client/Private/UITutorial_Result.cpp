#include "stdafx.h"
#include "..\Public\UITutorial_Result.h"

#include "GameInstance.h"
#include "GameInterface_Controller.h"

CUITutorial_Result::CUITutorial_Result(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage_Tutorial{ pDevice, pContext }
{

}

CUITutorial_Result::CUITutorial_Result(const CUITutorial_Result& Prototype)
	: CUIPage_Tutorial{ Prototype }
{
}

HRESULT CUITutorial_Result::Initialize_Prototype()
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

HRESULT CUITutorial_Result::Initialize(void* pArg)
{


	return S_OK;
}

void CUITutorial_Result::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUITutorial_Result::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUITutorial_Result::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	for (auto& iter : m_vec_Group_Ctrl)
		__super::UpdatePart_ByControl(iter);
}

HRESULT CUITutorial_Result::Render()
{
	return S_OK;
}

void CUITutorial_Result::OpenAction()
{
	__super::OpenAction();
}

void CUITutorial_Result::CloseAction()
{
	__super::CloseAction();
}

CHECK_MOUSE CUITutorial_Result::Check_Page_Action(_float fTimeDelta)
{
	__super::Check_Page_Action(fTimeDelta);

	return CHECK_MOUSE::MOUSE_NONE;
}

HRESULT CUITutorial_Result::Ready_UIPart_Group_Control()
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

void CUITutorial_Result::Set_Result(vector<struct CUIPage::UIPART_INFO*>& vecOrigin)
{






}

CUITutorial_Result* CUITutorial_Result::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUITutorial_Result* pInstance = new CUITutorial_Result(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUITutorial_Result"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUITutorial_Result::Clone(void* pArg)
{
	CUITutorial_Result* pInstance = new CUITutorial_Result(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUITutorial_Result"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUITutorial_Result::Free()
{
	__super::Free();

	for (auto& iter : m_vecPart)
	{
		Safe_Delete(iter);
	}

	m_vecPart.clear();

	m_vecSharedPointer_Frame.clear();
}
