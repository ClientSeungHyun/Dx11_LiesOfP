#include "stdafx.h"
#include "..\Public\UIPage_Ortho.h"

#include "GameInstance.h"
#include "UIPage_Ortho_Include.h"

CUIPage_Ortho::CUIPage_Ortho(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{

}

CUIPage_Ortho::CUIPage_Ortho(const CUIPage_Ortho& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Ortho::Initialize_Prototype()
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

	m_bRender = false;

	return S_OK;
}

HRESULT CUIPage_Ortho::Initialize(void* pArg)
{
	

	return S_OK;
}

void CUIPage_Ortho::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUIPage_Ortho::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUIPage_Ortho::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUIPage_Ortho::Render()
{
	return S_OK;
}

void CUIPage_Ortho::OpenAction()
{
	__super::OpenAction();
}

void CUIPage_Ortho::CloseAction()
{
	__super::CloseAction();
}

HRESULT CUIPage_Ortho::Ready_UIPart_Group_Control()
{
	__super::Ready_UIPart_Group_Control();

	for (_int i = 0; i < _int(PART_GROUP::GROUP_END); ++i)
	{
		UG_CTRL* pNew = new UG_CTRL;
		m_Ctrllist.push_back(pNew);
		m_vecOrtho_Group_Ctrl.push_back(pNew);
	}

	return S_OK;
}

void CUIPage_Ortho::Register_Pointer_Into_OrthoUIPage(UI_ORTHO_TYPE eType, const void* pObj)
{
	ORTHO_HOST* pNew = new ORTHO_HOST;
	pNew->eType = eType;
	pNew->pHost = static_cast<const CGameObject*>(pObj);
	m_Ortho_Host_list.push_back(pNew);
}

void CUIPage_Ortho::Initialize_Ortho_Info()
{
	// ���� UI �� ����ġ ���� 
	m_vecOrtho_Adjust.resize(_int(PART_GROUP::GROUP_END)); // <- ��Ʈ �׷��� �������� ���Ѵ� (�θ� �ε����� -1�� �� ����)
	m_vecOrtho_Adjust[_int(PART_GROUP::GROUP_HP_FRAME)] = { 0.f,-1.f,0.f }; // ���� ü�¹� (������, Fill ��� �����)
	m_vecOrtho_Adjust[_int(PART_GROUP::GROUP_HP_FILL)] = { 0.f,0.f,0.f }; // ���� ü�¹� (������, Fill ��� �����)
	m_vecOrtho_Adjust[_int(PART_GROUP::GROUP_FOCUS)] = { 0.f,0.f,0.f }; // ���� ��Ŀ��
	m_vecOrtho_Adjust[_int(PART_GROUP::GROUP_SPECIAL_HIT)] = { 0.f,0.f,0.f }; // ���� Ư������
}

void CUIPage_Ortho::Make_OrthoGraphy_Position(const CGameObject* pHost, PART_GROUP eGroup, _float2* fPosition)
{






}

CUIPage_Ortho* CUIPage_Ortho::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Ortho* pInstance = new CUIPage_Ortho(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Ortho"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIPage_Ortho::Clone(void* pArg)
{
	CUIPage_Ortho* pInstance = new CUIPage_Ortho(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Ortho"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Ortho::Free()
{
	__super::Free();

	for (auto& iter : m_Ortho_Host_list)
		Safe_Delete(iter);

	m_Ortho_Host_list.clear();

	for (auto& iter : m_vecPart)
	{
		Safe_Delete(iter);
	}

	m_vecPart.clear();
	m_vecOrtho_Adjust.clear();
}
