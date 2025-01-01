#include "stdafx.h"
#include "..\Public\UITutorial_Info.h"

#include "GameInstance.h"
#include "GameInterface_Controller.h"

CUITutorial_Info::CUITutorial_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage_Tutorial{ pDevice, pContext }
{

}

CUITutorial_Info::CUITutorial_Info(const CUITutorial_Info& Prototype)
	: CUIPage_Tutorial{ Prototype }
{
}

HRESULT CUITutorial_Info::Initialize_Prototype()
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

HRESULT CUITutorial_Info::Initialize(void* pArg)
{


	return S_OK;
}

void CUITutorial_Info::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUITutorial_Info::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUITutorial_Info::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	for (auto& iter : m_vec_Group_Ctrl)
		__super::UpdatePart_ByControl(iter);
}

HRESULT CUITutorial_Info::Render()
{
	return S_OK;
}

void CUITutorial_Info::OpenAction()
{
	__super::OpenAction();
}

void CUITutorial_Info::CloseAction()
{
	__super::CloseAction();
}

CHECK_MOUSE CUITutorial_Info::Check_Page_Action(_float fTimeDelta)
{
	__super::Check_Page_Action(fTimeDelta);

	return CHECK_MOUSE::MOUSE_NONE;
}

HRESULT CUITutorial_Info::Ready_UIPart_Group_Control()
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

void CUITutorial_Info::Set_Info(vector<struct CUIPage::UIPART_INFO*>& vecOrigin)
{
	_int iMission = -1;

	for (auto& iter : vecOrigin)
	{
		if (iter->iGroupIndex <= _int(PART_GROUP::GROUP_INFO_MISSION))
		{
			m_vecSharedPointer_All.push_back(iter);
		}
		
		if (iter->iGroupIndex == _int(PART_GROUP::GROUP_INFO_MISSION))
		{
			++iMission;

			if (iMission == 1)
				m_pSharedPointer_Mission_Title = iter;
			if (iMission == 2)
				m_pSharedPointer_Mission_Count = iter;
			if (iMission == 3)
				m_pSharedPointer_Mission_Result_Back = iter;
			if (iMission == 4)
				m_pSharedPointer_Mission_Result_Text = iter;
		}
	}
}

void CUITutorial_Info::Update_Info(TUTO_MISSION& NowData, _float fTimeDelta)
{
	_float fRatio = _float(NowData.iMissionIndex) / 3.f;

	m_vecSharedPointer_All[1]->fRatio = fRatio;
	m_pSharedPointer_Mission_Title->strText = NowData.strTitle;
	
	_wstring strCount = to_wstring(_int(NowData.fNow));
	strCount += TEXT(" / ");
	strCount += to_wstring(_int(NowData.fGoal));

	m_pSharedPointer_Mission_Count->strText = strCount;

	if (NowData.bComplete == true)
	{
		m_pSharedPointer_Mission_Result_Back->fTextureColor.y = 1.f;
		m_pSharedPointer_Mission_Result_Text->strText = TEXT("O");
	}
	else
	{
		m_pSharedPointer_Mission_Result_Back->fTextureColor.y = 0.5f;
		m_pSharedPointer_Mission_Result_Text->strText = TEXT("X");
	}

	for (_int i = 1; i < m_vecSharedPointer_All.size(); ++i)
	{
		m_vecSharedPointer_All[i]->MovePart(m_vecSharedPointer_All[m_vecSharedPointer_All[i]->iParentPart_Index]->fPosition, fTimeDelta);
		__super::Input_Render_Info(*m_vecSharedPointer_All[i]);
	}
}

CUITutorial_Info* CUITutorial_Info::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUITutorial_Info* pInstance = new CUITutorial_Info(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUITutorial_Info"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUITutorial_Info::Clone(void* pArg)
{
	CUITutorial_Info* pInstance = new CUITutorial_Info(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUITutorial_Info"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUITutorial_Info::Free()
{
	__super::Free();

	for (auto& iter : m_vecPart)
	{
		Safe_Delete(iter);
	}

	m_vecPart.clear();
	m_vecSharedPointer_All.clear();
}
