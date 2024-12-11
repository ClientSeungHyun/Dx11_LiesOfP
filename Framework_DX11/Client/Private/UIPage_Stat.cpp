#include "stdafx.h"
#include "..\Public\UIPage_Stat.h"

#include "GameInstance.h"
#include "GameInterface_Controller.h"

CUIPage_Stat::CUIPage_Stat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{

}

CUIPage_Stat::CUIPage_Stat(const CUIPage_Stat& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_Stat::Initialize_Prototype()
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

HRESULT CUIPage_Stat::Initialize(void* pArg)
{


	return S_OK;
}

void CUIPage_Stat::Priority_Update(_float fTimeDelta)
{


	__super::Priority_Update(fTimeDelta);
}

void CUIPage_Stat::Update(_float fTimeDelta)
{

	__super::Update(fTimeDelta);
}

void CUIPage_Stat::Late_Update(_float fTimeDelta)
{
	Update_Point(fTimeDelta);
	Update_LevelUp(fTimeDelta);
	Update_Focus(fTimeDelta);
	Update_SpecData(fTimeDelta);
	Update_StarChart(fTimeDelta);


	for (auto& iter : m_vec_Group_Ctrl)
		__super::UpdatePart_ByControl(iter);

	__super::Late_Update(fTimeDelta);

	
	
}

HRESULT CUIPage_Stat::Render()
{
	return S_OK;
}

void CUIPage_Stat::OpenAction()
{
	__super::OpenAction();
	m_iFocus_Point = 0;
}

void CUIPage_Stat::CloseAction()
{
	__super::CloseAction();
}

CHECK_MOUSE CUIPage_Stat::Check_Page_Action(_float fTimeDelta)
{
	__super::Check_Page_Action(fTimeDelta);

	Action_Point(fTimeDelta);
	Action_LevelUp(fTimeDelta);
	Action_Focus(fTimeDelta);

	return CHECK_MOUSE::MOUSE_NONE;
}

HRESULT CUIPage_Stat::Ready_UIPart_Group_Control()
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
	Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->iMoveType = 1;
	m_bRender = false;

	return S_OK;
}

void CUIPage_Stat::Action_Point(_float fTimeDelta)
{
	_bool bClick = KEY_TAP(KEY::LBUTTON);

	const CPlayer::PLAYER_STAT_INFO pOrigin = GET_GAMEINTERFACE->Get_Player()->Get_Player_Stat();
	CPlayer::PLAYER_STAT_INFO* pAdjust = GET_GAMEINTERFACE->Get_Player()->Get_Player_Stat_Adjust();

	m_iUsing_Point_Now = 0;

	for (_int i = 0; i < 5; ++i)
		m_iUsing_Point_Now += m_iLevelUp_Buffer_Point[i];

	for (_int i = 0; i < 5; ++i)
	{
		_Vec2 vMouseFront = GET_GAMEINTERFACE->CheckMouse(m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_BUTTON_0) + i]->PartIndexlist.front()]->fPosition, m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_BUTTON_0) + i]->PartIndexlist.front()]->fSize);
		_Vec2 vMouseBack = GET_GAMEINTERFACE->CheckMouse(m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_BUTTON_0) + i]->PartIndexlist.back()]->fPosition, m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_BUTTON_0) + i]->PartIndexlist.back()]->fSize);

		if (vMouseFront.x != -1.f)
		{
			m_iFocus_Point = i;
			if (bClick)
				if (m_iLevelUp_Buffer_Point[i] > 0)
				{
					--m_iLevelUp_Buffer_Point[i];
					--m_iUsing_Point_Now;
				}	
		}

		if (vMouseBack.x != -1.f)
		{
			m_iFocus_Point = i;
			if (bClick)
			{
				if (m_iUsing_Point_Now < pOrigin.iPoint_Per_Level)
				{
					++m_iLevelUp_Buffer_Point[i];
					++m_iUsing_Point_Now;
				}
				else 
					GET_GAMEINTERFACE->Show_Popup(TEXT("����Ʈ �й� �Ұ�"), TEXT("����� �� �ִ� ����Ʈ�� �����ϴ�."));
			}
		}
	}
}

void CUIPage_Stat::Action_LevelUp(_float fTimeDelta)
{
	const CPlayer::PLAYER_STAT_INFO pOrigin = GET_GAMEINTERFACE->Get_Player()->Get_Player_Stat();

	m_bActive_LevelUp_Button = false;

	if (m_iUsing_Point_Now == pOrigin.iPoint_Per_Level)
		if (pOrigin.iErgo >= pOrigin.iErgo_LevelUp)
			m_bActive_LevelUp_Button = true;

	if (KEY_TAP(KEY::LBUTTON))
		if (GET_GAMEINTERFACE->CheckMouse(__super::Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_LEVELUP_BUTTON))->fPosition, __super::Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_LEVELUP_BUTTON))->fSize).x != -1.f)
		{
			if (m_iUsing_Point_Now < pOrigin.iPoint_Per_Level)
				GET_GAMEINTERFACE->Show_Popup(TEXT("���� �� �Ұ�"), TEXT("�й����� ���� ����Ʈ�� �ֽ��ϴ�."));
			else if (pOrigin.iErgo < pOrigin.iErgo_LevelUp)
				GET_GAMEINTERFACE->Show_Popup(TEXT("���� �� �Ұ�"), TEXT("������ �����մϴ�."));
		}
		
}

void CUIPage_Stat::Action_Focus(_float fTimeDelta)
{
	_bool bFind = false;

	for (_int i = 0; i < 5; ++i)
	{
		for (auto& iter : m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_RECT_0) + i]->PartIndexlist)
		{
			if (GET_GAMEINTERFACE->CheckMouse(m_vecPart[iter]->fPosition, m_vecPart[iter]->fSize).x != -1.f)
			{
				m_iFocus_Point = i;
				bFind = true;
				break;
			}
		}

		if (bFind) break;
	}

	// Rect 
	Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_RECT))->iParentPart_Index = _int(PART_GROUP::GROUP_POINT_RECT_0) + m_iFocus_Point;

	// Focus 
	if (Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->iParentPart_Index != _int(PART_GROUP::GROUP_POINT_RECT_0) + m_iFocus_Point)
	{
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->iParentPart_Index = _int(PART_GROUP::GROUP_POINT_RECT_0) + m_iFocus_Point;
		_Vec2 vPos = Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_POINT_RECT_0) + m_iFocus_Point)->fPosition;
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->fAdjust_Start = Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->fPosition - vPos;
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->fAdjust_End = { 0.f,0.f };
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->fRatio = 0.f;
	}

	if (fTimeDelta > 0.1f)
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->fRatio += 0.01f;
	else
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->fRatio += fTimeDelta;

	if (Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->fRatio > 1.f)
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->fRatio = 1.f;
}

void CUIPage_Stat::Update_Point(_float fTimeDelta)
{
	const CPlayer::PLAYER_STAT_INFO pOrigin = GET_GAMEINTERFACE->Get_Player()->Get_Player_Stat();
	CPlayer::PLAYER_STAT_INFO* pAdjust = GET_GAMEINTERFACE->Get_Player()->Get_Player_Stat_Adjust();
	CPlayer::PLAYER_ABILITY_INFO* pAblity = GET_GAMEINTERFACE->Get_Player()->Get_Player_Ability();

	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_0)]->PartIndexlist.front()]->strText = to_wstring((m_iLevelUp_Buffer_Point[0] + pOrigin.iPoint_HP) / 10);
	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_0)]->PartIndexlist.back()]->strText = to_wstring((m_iLevelUp_Buffer_Point[0] + pOrigin.iPoint_HP) % 10);
	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_1)]->PartIndexlist.front()]->strText = to_wstring((m_iLevelUp_Buffer_Point[1] + pOrigin.iPoint_Stamina) / 10);
	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_1)]->PartIndexlist.back()]->strText = to_wstring((m_iLevelUp_Buffer_Point[1] + pOrigin.iPoint_Stamina) % 10);
	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_2)]->PartIndexlist.front()]->strText = to_wstring((m_iLevelUp_Buffer_Point[2] + pOrigin.iPoint_Attack) / 10);
	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_2)]->PartIndexlist.back()]->strText = to_wstring((m_iLevelUp_Buffer_Point[2] + pOrigin.iPoint_Attack) % 10);
	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_3)]->PartIndexlist.front()]->strText = to_wstring((m_iLevelUp_Buffer_Point[3] + pOrigin.iPoint_Defence) / 10);
	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_3)]->PartIndexlist.back()]->strText = to_wstring((m_iLevelUp_Buffer_Point[3] + pOrigin.iPoint_Defence) % 10);
	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_4)]->PartIndexlist.front()]->strText = to_wstring((m_iLevelUp_Buffer_Point[4] + pOrigin.iPoint_Heal) / 10);
	m_vecPart[m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_POINT_NUM_4)]->PartIndexlist.back()]->strText = to_wstring((m_iLevelUp_Buffer_Point[4] + pOrigin.iPoint_Heal) % 10);
}

void CUIPage_Stat::Update_LevelUp(_float fTimeDelta)
{
	if (m_bActive_LevelUp_Button == false)
	{
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_LEVELUP_FX))->bRender = false;
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_LEVELUP_TEXT))->fTextColor = { 0.5f, 0.5f, 0.5f, 1.f };
	}
	else
	{
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_LEVELUP_FX))->bRender = true;
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_LEVELUP_TEXT))->fTextColor = { 1.f, 1.f, 1.f, 1.f };
	}
}

void CUIPage_Stat::Update_Focus(_float fTimeDelta)
{
	if (m_iFocus_Point > -1)
	{

		_int iIndex = Get_Front_PartIndex_In_Control(_int(PART_GROUP::GROUP_POINT_RECT_0) + m_iFocus_Point);
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_RECT))->iParentPart_Index = Get_Front_PartIndex_In_Control(_int(PART_GROUP::GROUP_POINT_RECT_0) + m_iFocus_Point);
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->iParentPart_Index = Get_Front_PartIndex_In_Control(_int(PART_GROUP::GROUP_POINT_RECT_0) + m_iFocus_Point);
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_RECT))->bRender = true;
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->bRender = true;
	}
	else
	{
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_RECT))->bRender = false;
		Get_Front_Part_In_Control(_int(PART_GROUP::GROUP_SELECT_FIRE))->bRender = false;
	}
}

void CUIPage_Stat::Update_SpecData(_float fTimeDelta)
{
	const CPlayer::PLAYER_STAT_INFO pOrigin = GET_GAMEINTERFACE->Get_Player()->Get_Player_Stat();
	CPlayer::PLAYER_STAT_INFO* pAdjust = GET_GAMEINTERFACE->Get_Player()->Get_Player_Stat_Adjust();
	CPlayer::PLAYER_ABILITY_INFO* pAblity = GET_GAMEINTERFACE->Get_Player()->Get_Player_Ability();

	_float* pOrigin_Stat = new _float[9];
	_float* pAdjust_Stat = new _float[9];

	pOrigin_Stat[0] = pOrigin.vGauge_Hp.z;
	pOrigin_Stat[1] = pOrigin.vGauge_Stamina.z;
	pOrigin_Stat[2] = pOrigin.iStat_Attack;
	pOrigin_Stat[3] = pOrigin.iStat_Defence;
	pOrigin_Stat[4] = 0.f;
	pOrigin_Stat[5] = pOrigin.vGauge_Region.z;
	pOrigin_Stat[6] = 0.f;
	pOrigin_Stat[7] = 0.f;
	pOrigin_Stat[8] = 0.f;

	pAdjust_Stat[0] = pAdjust->vGauge_Hp.z;
	pAdjust_Stat[1] = pAdjust->vGauge_Stamina.z;
	pAdjust_Stat[2] = pAdjust->iStat_Attack;
	pAdjust_Stat[3] = pAdjust->iStat_Defence;
	pAdjust_Stat[4] = pAblity->fIncrease_Hp;
	pAdjust_Stat[5] = pAdjust->vGauge_Region.z;
	pAdjust_Stat[6] = pAblity->fResist_Fire;
	pAdjust_Stat[7] = pAblity->fResist_Electric;
	pAdjust_Stat[8] = pAblity->fResist_Acid;

	m_iLevelUp_Buffer_Stat[0] = m_iLevelUp_Buffer_Point[0] * 50.f;
	m_iLevelUp_Buffer_Stat[1] = m_iLevelUp_Buffer_Point[1] * 50.f;
	m_iLevelUp_Buffer_Stat[2] = m_iLevelUp_Buffer_Point[2] * 10.f;
	m_iLevelUp_Buffer_Stat[3] = m_iLevelUp_Buffer_Point[3] * 10.f;
	m_iLevelUp_Buffer_Stat[4] = m_iLevelUp_Buffer_Point[4] * 10.f;
	m_iLevelUp_Buffer_Stat[5] = m_iLevelUp_Buffer_Point[1] * 10.f;
	m_iLevelUp_Buffer_Stat[6] = m_iLevelUp_Buffer_Point[3] * 10.f;
	m_iLevelUp_Buffer_Stat[7] = m_iLevelUp_Buffer_Point[2] * 10.f;
	m_iLevelUp_Buffer_Stat[8] = m_iLevelUp_Buffer_Point[3] * 10.f;

	


	for (_int i = 0; i < 9; ++i)
	{
		pOrigin_Stat[i] *= 100.f;
		round(pOrigin_Stat[i]);
		pOrigin_Stat[i] /= 100.f;

		pAdjust_Stat[i] *= 100.f;
		round(pAdjust_Stat[i]);
		pAdjust_Stat[i] /= 100.f;

		m_iLevelUp_Buffer_Stat[i] *= 100.f;
		round(m_iLevelUp_Buffer_Stat[i]);
		m_iLevelUp_Buffer_Stat[i] /= 100.f;
	}

	if (pAblity->bDebuff_Fire_Ignore)
		m_iLevelUp_Buffer_Stat[6] = -1.f;
	if (pAblity->bDebuff_Electric_Ignore)
		m_iLevelUp_Buffer_Stat[7] = -1.f;
	if (pAblity->bDebuff_Acid_Ignore)
		m_iLevelUp_Buffer_Stat[8] = -1.f;

	m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_ABILITY_6)]->bRender = false;
	m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_ABILITY_7)]->bRender = false;
	m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_ABILITY_8)]->bRender = false;


	for (_int i = 0; i < 9; ++i)
	{
		if (m_iLevelUp_Buffer_Stat[i] < 0.f)
		{
			m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_SPEC_0) + i]->bRender = false;

			if (i >= _int(PART_GROUP::GROUP_ABILITY_6))
				m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_ABILITY_6) + i]->bRender = true;

			continue;
		}
		

		list<_int>::iterator iter = m_vec_Group_Ctrl[_int(PART_GROUP::GROUP_SPEC_0) + i]->PartIndexlist.begin();

		m_vecPart[*iter]->fTextureColor = { 0.2f,0.2f,0.2f,0.2f };


		if (m_iFocus_Point == 0)
			if (i == 0)
				m_vecPart[*iter]->fTextureColor = { 0.5f,1.f,0.9f,0.7f };

		if (m_iFocus_Point == 1)
			if ((i == 1) || (i == 5))
				m_vecPart[*iter]->fTextureColor = { 0.5f,1.f,0.9f,0.7f };

		if (m_iFocus_Point == 2)
			if ((i == 2) || (i == 7))
				m_vecPart[*iter]->fTextureColor = { 0.5f,1.f,0.9f,0.7f };
			
		if (m_iFocus_Point == 3)
		{
			if ((i == 3) || (i == 6))
				m_vecPart[*iter]->fTextureColor = { 0.5f,1.f,0.9f,0.7f };
			if (i == 8)
				m_vecPart[*iter]->fTextureColor = { 0.5f,1.f,0.9f,0.7f };
		}
			
		if (m_iFocus_Point == 4)
			if (i == 4)
				m_vecPart[*iter]->fTextureColor = { 0.5f,1.f,0.9f,0.7f };



		++iter;
		m_vecPart[*iter]->strText = to_wstring(_int(pOrigin_Stat[i]));

		if (m_iLevelUp_Buffer_Stat[i] > 0.f)
		{
			m_vecPart[*iter]->fRatio = 0.f;
			++iter;
			m_vecPart[*iter]->strText = TEXT("(+");
			m_vecPart[*iter]->strText += to_wstring(_int(m_iLevelUp_Buffer_Stat[i]));
			m_vecPart[*iter]->strText += TEXT(")");
		}
		else
		{
			m_vecPart[*iter]->fRatio = 1.f;
			++iter;
			m_vecPart[*iter]->strText = {};
		}

		++iter;
		m_vecPart[*iter]->strText = to_wstring(_int(pAdjust_Stat[i]));

		++iter;
		m_vecPart[*iter]->strText = to_wstring(_int(pOrigin_Stat[i] + m_iLevelUp_Buffer_Stat[i] + pAdjust_Stat[i]));
	}

	Safe_Delete_Array(pOrigin_Stat); 
	Safe_Delete_Array(pAdjust_Stat);
}

void CUIPage_Stat::Update_StarChart(_float fTimeDelta)
{
}

CUIPage_Stat* CUIPage_Stat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_Stat* pInstance = new CUIPage_Stat(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_Stat"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIPage_Stat::Clone(void* pArg)
{
	CUIPage_Stat* pInstance = new CUIPage_Stat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_Stat"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_Stat::Free()
{
	__super::Free();

	for (auto& iter : m_vecPart)
	{
		Safe_Delete(iter);
	}

	m_vecPart.clear();
}
