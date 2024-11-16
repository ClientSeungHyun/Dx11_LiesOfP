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

	Initialize_Ortho_Info();

	m_bRender = false;

	return S_OK;
}

HRESULT CUIPage_Ortho::Initialize(void* pArg)
{
	

	return S_OK;
}

void CUIPage_Ortho::Priority_Update(_float fTimeDelta)
{
	// �ӽ� �ڵ� 
	// ���� UI�� ���� �������� ���� 
	// ��Ȱ��ȭ ���·� git�� �÷� �α� 
	return;

	__super::Priority_Update(fTimeDelta);
}

void CUIPage_Ortho::Update(_float fTimeDelta)
{
	// �ӽ� �ڵ� 
	// ���� UI�� ���� �������� ���� 
	// ��Ȱ��ȭ ���·� git�� �÷� �α� 
	return;

	__super::Update(fTimeDelta);
}

void CUIPage_Ortho::Late_Update(_float fTimeDelta)
{
	// �ӽ� �ڵ� 
	// ���� UI�� ���� �������� ���� 
	// ��Ȱ��ȭ ���·� git�� �÷� �α� 
	return;


	CheckHost(fTimeDelta);
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

void CUIPage_Ortho::Register_Pointer_Into_OrthoUIPage(UI_ORTHO_OBJ_TYPE eType, void* pObj)
{
	ORTHO_HOST* pNew = new ORTHO_HOST;
	pNew->eType = eType;
	pNew->pHost = static_cast<CGameObject*>(pObj);
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

void CUIPage_Ortho::CheckHost(_float fTimeDelta)
{
	for (list<CUIPage_Ortho::OR_HOST*>::iterator iter = m_Ortho_Host_list.begin(); iter!= m_Ortho_Host_list.end();)
	{
		// Host ���� ���� �� 
		/*
		1. host�� nullptr�̰ų� dead�̴� 
		
		
		*/

		if (((*iter)->pHost == nullptr) || ((*iter)->pHost->Get_Dead()))
		{
			// iter->pHost�� ���� ����� ���������� addref ���� �ʾ���
			Safe_Delete(*iter);
			iter = m_Ortho_Host_list.erase(iter);
		}
		else
		{
			CheckHost_By_ObjType((*iter)->pHost, (*iter)->eType, fTimeDelta);
			++iter;
		}
	}
}

void CUIPage_Ortho::CheckHost_By_ObjType(CGameObject* pHost, UI_ORTHO_OBJ_TYPE eType, _float fTimeDelta)
{
	// �Ϲ� ����
	if (eType == UI_ORTHO_OBJ_TYPE::ORTHO_NORMAL_MONSTER)
	{
		// ���� UI�� ON/OFF �ϴ� �� 
		// �� ���� UI �� � �� 


		// HP ��
		_float fHP_Ratio = 0.5f; // <- �׽�Ʈ ��ġ�� ���� ���� �ʿ�

		Make_Ortho_UI(pHost, PART_GROUP::GROUP_HP_FRAME, fTimeDelta, fHP_Ratio);

		// ��Ŀ��
		Make_Ortho_UI(pHost, PART_GROUP::GROUP_FOCUS, fTimeDelta);

		// Ư������
		Make_Ortho_UI(pHost, PART_GROUP::GROUP_SPECIAL_HIT, fTimeDelta);
	}
}

void CUIPage_Ortho::Make_Ortho_UI(CGameObject* pHost, PART_GROUP eGroup, _float fTimeDelta, _float fRatio)
{
	_float2 fPos = { 0.f,0.f };
	_float fDistance = 0.f;
	if (!Make_OrthoGraphy_Position(pHost, eGroup, &fPos, &fDistance))
		return;

	m_vecPart[m_vecOrtho_Group_Ctrl[_int(eGroup)]->PartIndexlist.front()]->fAdjust = fPos;

	if (eGroup == PART_GROUP::GROUP_HP_FRAME)
	{
		m_vecPart[m_vecOrtho_Group_Ctrl[_int(PART_GROUP::GROUP_HP_FILL)]->PartIndexlist.front()]->fRatio = fRatio;
		__super::UpdatePart_ByIndex(m_vecOrtho_Group_Ctrl[_int(PART_GROUP::GROUP_HP_FILL)]->PartIndexlist.front(), fTimeDelta);
		OR_RENDER* pNew = new OR_RENDER(fDistance, m_vecPart[m_vecOrtho_Group_Ctrl[_int(PART_GROUP::GROUP_HP_FILL)]->PartIndexlist.front()]);
	}

	for (auto& iter : m_vecOrtho_Group_Ctrl[_int(eGroup)]->PartIndexlist)
	{
		__super::UpdatePart_ByIndex(iter, fTimeDelta);
		OR_RENDER* pNew = new OR_RENDER(fDistance, m_vecPart[iter]);
	}
}

_bool CUIPage_Ortho::Make_OrthoGraphy_Position(CGameObject* pHost, PART_GROUP eGroup, _float2* fPosition, _float* fDistance_Cam)
{
	const _matrix mMat = pHost->Get_Transform()->Get_WorldMatrix();
	const _matrix mProj = m_pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ);
	const _matrix mView = m_pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW);
	_float3 fAdj = m_vecOrtho_Adjust[_int(eGroup)];
	_vector vResult = { fAdj.x, fAdj.y, fAdj.z, 0.f };

	// ���� ��ǥ ���
	vResult = XMVector3Transform(vResult, mMat);
	vResult = XMVector3Transform(vResult, mView);
	vResult = XMVector3Transform(vResult, mProj);

	// W������
	_float4 fResult{};
	XMStoreFloat4(&fResult, vResult);

	if (fResult.w < 0.f)
		return false;

	
	_vector vDistance = pHost->Get_Transform()->Get_State(CTransform::STATE_POSITION) - m_pGameInstance->Get_CamPosition_Vec4();

	*fDistance_Cam = sqrt(pow(vDistance.m128_f32[0], 2) + pow(vDistance.m128_f32[1], 2) + pow(vDistance.m128_f32[2], 2));
	fPosition->x = fResult.x / fResult.w;
	fPosition->y = fResult.y / fResult.w;

	return true;
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
