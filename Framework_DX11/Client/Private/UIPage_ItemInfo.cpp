#include "stdafx.h"
#include "..\Public\UIPage_ItemInfo.h"

#include "GameInstance.h"
#include "GameInterface_Controller.h"

CUIPage_ItemInfo::CUIPage_ItemInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIPage{ pDevice, pContext }
{

}

CUIPage_ItemInfo::CUIPage_ItemInfo(const CUIPage_ItemInfo& Prototype)
	: CUIPage{ Prototype }
{
}

HRESULT CUIPage_ItemInfo::Initialize_Prototype()
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

HRESULT CUIPage_ItemInfo::Initialize(void* pArg)
{


	return S_OK;
}

void CUIPage_ItemInfo::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUIPage_ItemInfo::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUIPage_ItemInfo::Late_Update(_float fTimeDelta)
{
	if (m_vecPageAction[_int(PAGEACTION::ACTION_ACTIVE)] == false)
		return;

	SCROLL_AREA eArea = SCROLL_AREA::SCROLL_NONE;
	if (m_eNowPage == UIPAGE::PAGE_INVEN)
		eArea = SCROLL_AREA::SCROLL_INVEN;

	// focus 
	if (m_bFocus)
	{
		if (m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->bRender)
		{
			m_fFocus_Fire_Move_Ratio += fTimeDelta * 5.f;
			m_fFocus_Fire_Move_Ratio = min(m_fFocus_Fire_Move_Ratio, 1.f);

			m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->MakeDirec();
			m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fPosition =
			{
				m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fAdjust_Start.x + (m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fDirec.x * m_fFocus_Fire_Move_Ratio),
				m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fAdjust_Start.y + (m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fDirec.y * m_fFocus_Fire_Move_Ratio)
			};
		}

		Input_Render_Info(*m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Rect)], eArea);
		Input_Render_Info(*m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)], eArea);
	}

	// New
	while (!m_queueNewMarkPos.empty())
	{
		_Vec2 vPos = m_queueNewMarkPos.front();
		m_queueNewMarkPos.pop();

		m_vecPart[_int(PART_GROUP::ITEMINFO_NEW)]->fPosition = vPos;

		Input_Render_Info(*m_vecPart[_int(PART_GROUP::ITEMINFO_NEW)], eArea);
	}

	// Action
	if (m_bIsActive_Func)
	for (_int i = _int(PART_GROUP::ITEMINFO_ACTION_Back); i <= _int(PART_GROUP::ITEMINFO_ACTION_Text_3); ++i)
	{
		m_vecPart[i]->MovePart(m_vecPart[m_vecPart[i]->iParentPart_Index]->fPosition, fTimeDelta);

		if (m_vecPart[i]->bRender)
			Input_Render_Info(*m_vecPart[i], SCROLL_AREA::SCROLL_NONE);
	}

	m_vecPageAction[_int(PAGEACTION::ACTION_ACTIVE)] = false;
}

HRESULT CUIPage_ItemInfo::Render()
{
	return S_OK;
}

void CUIPage_ItemInfo::OpenAction()
{
	__super::OpenAction();
}

void CUIPage_ItemInfo::CloseAction()
{
	__super::CloseAction();
}

CHECK_MOUSE CUIPage_ItemInfo::Check_Page_Action(_float fTimeDelta)
{
	__super::Check_Page_Action(fTimeDelta);

	if (m_bIsActive_Func)
		Action_ItemAction(fTimeDelta);

	return CHECK_MOUSE::MOUSE_NONE;
}

HRESULT CUIPage_ItemInfo::Ready_UIPart_Group_Control()
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

	__super::Array_Control(_int(PART_GROUP::ITEMINFO_TOOLTIP_Frame), _int(PART_GROUP::ITEMINFO_ACTION_Text_3), CTRL_COMMAND::COM_RENDER, false);

	m_strFuncName[_int(ITEM_FUNC::FUNC_USING)] = TEXT("����ϱ�");
	m_strFuncName[_int(ITEM_FUNC::FUNC_TO_INVEN)] = TEXT("�κ��丮�� �̵�");
	m_strFuncName[_int(ITEM_FUNC::FUNC_TO_EQUIP)] = TEXT("���â���� �̵�");
	m_strFuncName[_int(ITEM_FUNC::FUNC_EQUIP)] = TEXT("�����ϱ�");
	m_strFuncName[_int(ITEM_FUNC::FUNC_UNEQUIP)] = TEXT("�����ϱ�");
	m_strFuncName[_int(ITEM_FUNC::FUNC_DELETE)] = TEXT("������");
	m_strFuncName[_int(ITEM_FUNC::FUNC_EQUIP_TOP)] = TEXT("�����ϱ� : �� ��Ʈ");
	m_strFuncName[_int(ITEM_FUNC::FUNC_EQUIP_BOTTOM)] = TEXT("�����ϱ� : �Ʒ� ��Ʈ");
	m_strFuncName[_int(ITEM_FUNC::FUNC_EQUIP_BAG)] = TEXT("�����ϱ� : ��������");
	m_strFuncName[_int(ITEM_FUNC::FUNC_EQUIP_WEAPON_FIRST)] = TEXT("�����ϱ� : ���� 1��");
	m_strFuncName[_int(ITEM_FUNC::FUNC_EQUIP_WEAPON_SECOND)] = TEXT("�����ϱ� : ���� 2��");
	Off_ItemAction();

	m_bRender = false;

	return S_OK;
}

void CUIPage_ItemInfo::Show_Focus(_Vec2 vItemCellPos, _Vec2 vItemCellSize)
{
	m_bFocus = true;

	m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Rect)]->fPosition = vItemCellPos;
	m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Rect)]->fSize = vItemCellSize * 1.1f;

	if (!m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Rect)]->bRender)
	{
		//m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fAdjust_Start = vItemCellPos + _Vec2{ -(vItemCellSize.x * 0.4f), vItemCellSize.y * 0.4f };
		m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fAdjust_End = vItemCellPos + _Vec2{ -(vItemCellSize.x * 0.4f), vItemCellSize.y * 0.4f };
	}
	else
	{
		m_fFocus_Fire_Move_Ratio = 0.f;
		m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fAdjust_Start = m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fPosition;
		m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->fAdjust_End = vItemCellPos + _Vec2{ -(vItemCellSize.x * 0.4f), vItemCellSize.y * 0.4f };
	}

	m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Rect)]->bRender = true;
	m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->bRender = true;
}

void CUIPage_ItemInfo::Off_Focus()
{
	m_bFocus = false;

	m_fFocus_Fire_Move_Ratio = 0.f;
	m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Rect)]->bRender = false;
	m_vecPart[_int(PART_GROUP::ITEMINFO_SELECT_Fire)]->bRender = false;
}

void CUIPage_ItemInfo::Show_NewMark(_Vec2 vItemCellPos, _Vec2 vItemCellSize)
{
	_Vec2 vPos = vItemCellPos + _Vec2(vItemCellSize.x * 0.5f, -vItemCellSize.y * 0.5f);
	m_queueNewMarkPos.push(vPos);
}

void CUIPage_ItemInfo::Show_ItemAction(_Vec2 vItemCellPos, _Vec2 vItemCellSize, ITEM_FUNC eFunc0, ITEM_FUNC eFunc1, ITEM_FUNC eFunc2, ITEM_FUNC eFunc3)
{
	m_bIsActive_Func = true;
	m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Header)]->fPosition = vItemCellPos + (m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Header)]->fSize * 0.5f) + (vItemCellSize * 0.5f);
	m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Back)]->bRender = true;
	m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Deco)]->bRender = true;

	m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Back)]->fRatio = 0.f;

	m_eActive_Func[0] = eFunc0;
	m_eActive_Func[1] = eFunc1;
	m_eActive_Func[2] = eFunc2;
	m_eActive_Func[3] = eFunc3;

	for (_int i = 0; i < 4; ++i)
	{
		if (m_eActive_Func[i] != ITEM_FUNC::FUNC_END)
		{
			m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Back)]->fRatio += 0.25f;
			m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Text_0) + (i * 3)]->strText = m_strFuncName[_int(m_eActive_Func[i])];
			m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Text_0) + (i * 3)]->bRender = true;
		}
		else
		{
			m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Text_0) + (i * 3)]->strText = {};
			m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Text_0) + (i * 3)]->bRender = false;
		}
	}
}

void CUIPage_ItemInfo::Off_ItemAction()
{
	m_bIsActive_Func = false;

	__super::Array_Control(_int(PART_GROUP::ITEMINFO_ACTION_Header), _int(PART_GROUP::ITEMINFO_ACTION_Text_3), CTRL_COMMAND::COM_RENDER, false);
	for (_int i = 0; i < 4; ++i)
		m_eActive_Func[i] = ITEM_FUNC::FUNC_END;
}

void CUIPage_ItemInfo::Action_ItemAction(_float fTimeDelta)
{
	m_iNow_Func = -1;

	for (_int i = 0; i < 4; ++i)
	{
		if (m_eActive_Func[i] != ITEM_FUNC::FUNC_END)
		{
			if (GET_GAMEINTERFACE->CheckMouse(m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Mouse_0) + (i * 3)]->fPosition, m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Mouse_0) + (i * 3)]->fSize).x != -1.f)
			{
				m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Fx_0) + (i * 3)]->bRender = true;

				if (KEY_TAP(KEY::LBUTTON))
				{
					m_iNow_Func = i;
				}
			}
			else 
				m_vecPart[_int(PART_GROUP::ITEMINFO_ACTION_Fx_0) + (i * 3)]->bRender = false;
		}
		else
			break;
	}
}


CUIPage_ItemInfo* CUIPage_ItemInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage_ItemInfo* pInstance = new CUIPage_ItemInfo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage_ToolTip"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIPage_ItemInfo::Clone(void* pArg)
{
	CUIPage_ItemInfo* pInstance = new CUIPage_ItemInfo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage_ToolTip"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage_ItemInfo::Free()
{
	__super::Free();

	for (auto& iter : m_vecPart)
	{
		Safe_Delete(iter);
	}

	m_vecPart.clear();
}