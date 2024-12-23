#include "stdafx.h"
#include "..\Public\UIPage.h"

#include "GameInstance.h"
#include "GameInterface_Controller.h"

CUIPage::CUIPage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIPage::CUIPage(const CUIPage& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUIPage::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIPage::Initialize(void* pArg)
{
	m_pItemaction = new ITEMACTION;

	/* FOR.Com_VoiceSound */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Sound"),
		TEXT("Com_VoiceSound"), reinterpret_cast<CComponent**>(&m_pSoundCom))))
		return E_FAIL;

	m_pSoundCom->Set_Owner(this);

	/* ���������� ���� �����͵��� ��� ���ϳ�. */
	if (FAILED(__super::Initialize(&pArg)))
		return E_FAIL;

	return S_OK;
}

void CUIPage::Priority_Update(_float fTimeDelta)
{
	// �� ������ �ʱ�ȭ�ؾ� �ϴ� ������ ó�� 
	__super::Priority_Update(fTimeDelta);
}

void CUIPage::Update(_float fTimeDelta)
{
	// ������� �� �������� ���� ���� ������ ������ �� 
	__super::Update(fTimeDelta);
	m_pSoundCom->Update(fTimeDelta);
}

void CUIPage::Late_Update(_float fTimeDelta)
{
	// �� ������ �� ���� ������ ���� �ݿ��ϴ� �ܰ�
	__super::Late_Update(fTimeDelta);

	if ((m_vecPageAction[_int(PAGEACTION::ACTION_CLOSING)]) && (!m_vecPageAction[_int(PAGEACTION::ACTION_OPENING)]))
	{
		m_fTopPartMove -= m_fTopPartMove_Excel * fTimeDelta;
		if (m_fTopPartMove < 0.f)
		{
			m_fTopPartMove = 0.f;
			m_vecPageAction[_int(PAGEACTION::ACTION_CLOSING)] = false;
			m_vecPageAction[_int(PAGEACTION::ACTION_INACTIVE)] = true;
		}
	}
	else if ((!m_vecPageAction[_int(PAGEACTION::ACTION_CLOSING)]) && (m_vecPageAction[_int(PAGEACTION::ACTION_OPENING)]))
	{
		m_fTopPartMove += m_fTopPartMove_Excel * fTimeDelta;
		if (m_fTopPartMove > 1.f)
		{
			m_fTopPartMove = 1.f;
			m_vecPageAction[_int(PAGEACTION::ACTION_OPENING)] = false;
			m_vecPageAction[_int(PAGEACTION::ACTION_ACTIVE)] = true;
		}
	}

	for (auto& iter : m_vecPart)
	{
		iter->MakeDirec();

		if (iter->iParentPart_Index == -1)
		{
			if (m_vecPageAction[_int(PAGEACTION::ACTION_CLOSING)] + m_vecPageAction[_int(PAGEACTION::ACTION_OPENING)] == 1)
				iter->fRatio = m_fTopPartMove;

			iter->MovePart({ m_fX,m_fY }, fTimeDelta);
		}
		else
		{
			iter->MovePart(m_vecPart[iter->iParentPart_Index]->fPosition, fTimeDelta);
		}

			if (m_bRender)
				if (iter->bRender)
					Input_Render_Info(*iter);
	}

	if ((m_fTopPartMove == 0.f) || (m_fTopPartMove == -1.f))
	{
		if (m_vecPageAction[_int(PAGEACTION::ACTION_CLOSING)])
			m_bRender = false;

		m_vecPageAction[_int(PAGEACTION::ACTION_CLOSING)] = false;
		m_vecPageAction[_int(PAGEACTION::ACTION_OPENING)] = false;
	}

}

HRESULT CUIPage::Render()
{
	return S_OK;
}

void CUIPage::OpenAction()
{
	m_vecPageAction[_int(PAGEACTION::ACTION_CLOSING)] = false;
	m_vecPageAction[_int(PAGEACTION::ACTION_OPENING)] = true;
	m_vecPageAction[_int(PAGEACTION::ACTION_INACTIVE)] = false;
	m_vecPageAction[_int(PAGEACTION::ACTION_ACTIVE)] = false;
	m_bRender = true;
}

void CUIPage::CloseAction()
{
	m_vecPageAction[_int(PAGEACTION::ACTION_CLOSING)] = true;
	m_vecPageAction[_int(PAGEACTION::ACTION_OPENING)] = false;
	m_vecPageAction[_int(PAGEACTION::ACTION_INACTIVE)] = false;
	m_vecPageAction[_int(PAGEACTION::ACTION_ACTIVE)] = false;
	//m_bRender = false;
}

HRESULT CUIPage::Ready_UIPart_Group_Control()
{
	return S_OK;
}

CHECK_MOUSE CUIPage::Check_Page_Action(_float fTimeDelta)
{
	return CHECK_MOUSE::MOUSE_NONE;
}

void CUIPage::Release_Control(UG_CTRL* pCtrl)
{
	pCtrl->PartIndexlist.clear();
	Safe_Delete(pCtrl);
}

_Vec2 CUIPage::Check_Mouse_By_Part(UPART& Part)
{
	_Vec2 fPos = Part.fPosition;
	_Vec2 fSize = Part.fSize;
	return GET_GAMEINTERFACE->CheckMouse(fPos, fSize);
}

void CUIPage::UpdatePart_ByIndex(_int Index, _float fTimeDelta)
{
	m_vecPart[Index]->MakeDirec();

	if (m_vecPart[Index]->iParentPart_Index == -1)
	{
		if (m_vecPageAction[_int(PAGEACTION::ACTION_CLOSING)] + m_vecPageAction[_int(PAGEACTION::ACTION_OPENING)] == 1)
			m_vecPart[Index]->fRatio = m_fTopPartMove;

		m_vecPart[Index]->MovePart({ m_fX,m_fY }, fTimeDelta);
	}
	else
		m_vecPart[Index]->MovePart(m_vecPart[m_vecPart[Index]->iParentPart_Index]->fPosition, fTimeDelta);
}

void CUIPage::Input_Render_Info(UPART& Part, SCROLL_AREA eArea)
{
	if (Part.iTexture_Index < 0)
		if ((Part.iFontIndex < 0) || (Part.iFontIndex >= _int(UI_FONT::FONT_END)))
			return;


	CUIRender_Batching::UIRENDER_INFO* pNew = new CUIRender_Batching::UIRENDER_INFO;

	pNew->bIsItem = Part.bIsItem;
	pNew->iTexture = Part.iTexture_Index;
	pNew->bIsMultiple = Part.bTexture_Color_Multiple;
	if (Part.iTexture_Index > 0)
		pNew->eText_Type = TEXT_TYPE::TEXT_END;
	else if (Part.bText_Right)
		pNew->eText_Type = TEXT_TYPE::TEXT_RIGHT;
	else if (Part.bCenter)
		pNew->eText_Type = TEXT_TYPE::TEXT_CENTER;
	else
		pNew->eText_Type = TEXT_TYPE::TEXT_LEFT;
	pNew->fTurn = Part.fTurn_Degree;
	pNew->iFont = Part.iFontIndex;
	pNew->strText = Part.strText;
	pNew->vColor_Text = Part.fTextColor;

	if (m_fTopPartMove >= 0.f)
		pNew->vColor_Text.w *= m_fTopPartMove;

	pNew->vColor_Texture = Part.fTextureColor;

	if (m_fTopPartMove >= 0.f)
		pNew->vColor_Texture.w *= m_fTopPartMove;

	if (Part.m_bEmpty_Stack_Item)
	{
		pNew->vColor_Texture.x *= 0.1f;
		pNew->vColor_Texture.y *= 0.1f;
		pNew->vColor_Texture.z *= 0.1f;
	}
		

	pNew->vPosition = Part.fPosition;
	pNew->vSize = Part.fSize;
	pNew->fAlpha_Strash = Part.fStrash_Alpha;

	pNew->eArea = eArea;
	pNew->vRange = Part.vTexture_Range;
	pNew->vAngle = Part.vTexture_Angle;

	if (Part.bIs_TwoDPolygon)
	{
		pNew->bIs_TwoDPolygon = true;
		pNew->iTwoPolygon_Buffer_Num = Part.iTwoPolygon_Buffer_Num;
		memcpy(pNew->fRatio_TwoDPolygon, Part.fRatio_TwoDPolygon, sizeof(_float) * 8);
	}
	else 
		pNew->bIs_TwoDPolygon = false;

	if (Part.iMoveType == _int(MOVETYPE::TYPE_BAR))
		pNew->vSize = Part.GetBarSize();

	pNew->bIsAlpha_Adjust = Part.bIsAlpha_Adjust;

	GET_GAMEINTERFACE->Input_Render_Info(pNew);
}

void CUIPage::UpdatePart_ByControl(UG_CTRL* pCtrl)
{
	for (auto& iter : pCtrl->PartIndexlist)
	{
		m_vecPart[iter]->fRatio = pCtrl->fRatio;
		m_vecPart[iter]->bUpdate = pCtrl->bUpdate;
		m_vecPart[iter]->bRender = pCtrl->bRender;
	}
}

CUIPage* CUIPage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIPage* pInstance = new CUIPage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIPage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIPage::Clone(void* pArg)
{
	CUIPage* pInstance = new CUIPage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIPage"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIPage::Free()
{
	__super::Free();

	m_vecPageAction.clear();

	for (auto& iter : m_vec_Group_Ctrl)
		Safe_Delete(iter);

	m_vec_Group_Ctrl.clear();

	Safe_Delete(m_pItemaction);

	Safe_Release(m_pSoundCom);
}

void CUIPage::SCROLL_INFO::Initialize_Scroll(UPART* pData, UPART* pBar, SCROLL_AREA eArea, _bool bIsX, _bool bIsY)
{
	vPos_Render = pData->fPosition;
	vSize_Render = pData->fSize;
	eScroll_Area = eArea;
	bIsSetting_Y = bIsY;
	bIsSetting_X = bIsX;

	// Y
	if (bIsY == true)
	{
		fData_Height_Origin = vSize_Render.y;

		fBar_Move_Max_Length_Y = pBar->fAdjust_End.y - pBar->fAdjust_Start.y;

		fRender_Top_Y = vPos_Render.y - vSize_Render.y * 0.5f; // <- �׷����� ���� ���� ��ġ 
		fRender_Bottom_Y = vPos_Render.y + vSize_Render.y * 0.5f; // <- �׷����� ���� ���� ��ġ 
	}
	
	// X
	if (bIsX == true)
	{
		fData_Width_Origin = vSize_Render.x;

		fBar_Move_Max_Length_X = pBar->fAdjust_End.x - pBar->fAdjust_Start.x;

		fRender_Left_X = vPos_Render.x - vSize_Render.x * 0.5f; // <- �׷����� ���� ���� ��ġ 
		fRender_Right_X = vPos_Render.x + vSize_Render.x * 0.5f; // <- �׷����� ���� ���� ��ġ 
	}
	
	GET_GAMEINTERFACE->Set_Scroll_Area(eScroll_Area, pData->fPosition, pData->fSize);
}

void CUIPage::SCROLL_INFO::Activate_Scroll(_float fData_Height_New, _float fData_Width_New, _Vec2 vPos, _Vec2 vSize)
{
	bIsActive_X = true;
	bIsActive_Y = true;

	if (bIsSetting_Y)
	{
		if (fData_Height_Origin >= fData_Height_New)
		{
			DeActivate_Scroll_Y();
			bIsActive_Y = false;
		}
		else
			bIsActive_Y = true;
	}
	
	if (bIsSetting_X)
	{
		if (fData_Width_Origin >= fData_Width_New)
		{
			DeActivate_Scroll_X();
			bIsActive_X = false;
		}
		else
			bIsActive_X = true;
	}

	if ((!bIsActive_X) && (!bIsActive_Y))
		return;

	if (bIsSetting_Y)
	{
		if ((vPos.y >= 0.f) && (vSize.y >= 0.f))
		{
			vPos_Render = vPos;
			vSize_Render = vSize;

			fData_Height_Origin = vSize_Render.y;

			fRender_Top_Y = vPos_Render.y - vSize_Render.y * 0.5f; // <- �׷����� ���� ���� ��ġ 
			fRender_Bottom_Y = vPos_Render.y + vSize_Render.y * 0.5f; // <- �׷����� ���� ���� ��ġ 
		}

		fData_Height_Max = fData_Height_New - fData_Height_Origin;
	}

	if (bIsSetting_X)
	{
		if ((vPos.x >= 0.f) && (vSize.x >= 0.f))
		{
			vPos_Render = vPos;
			vSize_Render = vSize;

			fData_Width_Origin = vSize_Render.x;

			fRender_Left_X = vPos_Render.x - vSize_Render.x * 0.5f; // <- �׷����� ���� ���� ��ġ 
			fRender_Right_X = vPos_Render.x + vSize_Render.x * 0.5f; // <- �׷����� ���� ���� ��ġ 

			
		}

		fData_Width_Max = fData_Width_New - fData_Width_Origin;
	}

	if ((vPos.y >= 0.f) && (vSize.y >= 0.f))
	{
		GET_GAMEINTERFACE->Set_Scroll_Area(eScroll_Area, vPos, vSize);
	}
}
