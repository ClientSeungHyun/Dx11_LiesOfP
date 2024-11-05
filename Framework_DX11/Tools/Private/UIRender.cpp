#include "stdafx.h"
#include "..\Public\UIRender.h"

#include "GameInstance.h"

CUIRender::CUIRender(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIRender::CUIRender(const CUIRender& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CUIRender::Initialize_Prototype()
{
	UI_DESC			Desc{};

	Desc.fX = g_iWinSizeX >> 1;
	Desc.fY = g_iWinSizeY >> 1;
	Desc.fSizeX = g_iWinSizeX;
	Desc.fSizeY = g_iWinSizeY;

	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* ���������� ���� �����͵��� ��� ���ϳ�. */
	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	Ready_Font();

	return S_OK;
}

HRESULT CUIRender::Initialize(void* pArg)
{
	return S_OK;
}

void CUIRender::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIRender::Update(_float fTimeDelta)
{
	int a = 10;
}

void CUIRender::Late_Update(_float fTimeDelta)
{
	/* ���������� ���� ������ı��� �����ϰ� �ȴ�. */
	__super::Late_Update(fTimeDelta);


	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUIRender::Render()
{
	while(!m_UIRenderlist.empty())
	{
		URCOM* pNow = m_UIRenderlist.front();

		if (pNow->iTextureIndex != -1)
		{
			m_pTransformCom->Set_Scaled(pNow->fSize.x, pNow->fSize.y, 1.f);

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(pNow->fPosition.x - m_fViewWidth * 0.5f, -pNow->fPosition.y + m_fViewHeight * 0.5f, 0.f, 1.f));

			if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
				return E_FAIL;

			if (FAILED(m_vecTextureInfo[pNow->iTextureIndex]->Texture->Bind_ShadeResource(m_pShaderCom, "g_Texture", 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;

			if (FAILED(m_pVIBufferCom->Bind_Buffers()))
				return E_FAIL;

			if (FAILED(m_pVIBufferCom->Render()))
				return E_FAIL;
		}
		else
		{
			_vector vPosition = { pNow->fPosition.x, pNow->fPosition.y, 0.f,0.f };
			_vector vColor = { 1.f,1.f,1.f,1.f };

			if (pNow->fRGB.x > 0.f)
				vColor.m128_f32[0] = pNow->fRGB.x;

			if (pNow->fRGB.y > 0.f)
				vColor.m128_f32[1] = pNow->fRGB.y;

			if (pNow->fRGB.z > 0.f)
				vColor.m128_f32[2] = pNow->fRGB.z;

			if (pNow->fAlpah > 0.f)
				vColor.m128_f32[3] = pNow->fAlpah;


			if (pNow->bIsCenter)
				m_pGameInstance->Render_TextCenter(m_vecFont_tchar[_int(pNow->eType)], pNow->szText, vPosition, vColor);
			else
				m_pGameInstance->Render_Text(m_vecFont_tchar[_int(pNow->eType)], pNow->szText, vPosition, vColor);
		}
		

		Safe_Delete_Array(pNow->szText);
		m_UIRenderlist.pop_front();
	}
	
	return S_OK;
}

void CUIRender::AddRenderUIObject(_int iTextureIndex, _float2 fPosition, _float2 fSize, _float3 fRGB, _float fAlpha)
{
	URCOM* pNew = new URCOM;

	pNew->iTextureIndex = iTextureIndex;
	pNew->fPosition = fPosition;
	pNew->fSize = fSize;
	pNew->fRGB = fRGB;
	pNew->fAlpah = fAlpha;

	m_UIRenderlist.push_back(pNew);
}

void CUIRender::AddRenderText(UI_FONT eFont, _bool bIsCenter, _tchar* szText, _float2 fPosition, _float3 fColor, _float fAlpha)
{
	URCOM* pNew = new URCOM;

	pNew->fPosition = fPosition;
	pNew->fRGB = fColor;
	pNew->fAlpah = fAlpha;
	pNew->eType = eFont;
	pNew->szText = szText;
	pNew->bIsCenter = bIsCenter;

	m_UIRenderlist.push_back(pNew);
}

HRESULT CUIRender::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	///* FOR.Com_Texture */
	//if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Texture_UIRender"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

void CUIRender::Ready_Font()
{
	m_vecFont_char.resize(_int(UI_FONT::FONT_END));
	m_vecFont_tchar.resize(_int(UI_FONT::FONT_END));
	
		

	m_pGameInstance->Add_Font(TEXT("FONT_INFO_12"), TEXT("../Bin/Resources/Fonts/Font_Info_12.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_INFO_18"), TEXT("../Bin/Resources/Fonts/Font_Info_18.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_INFO_24"), TEXT("../Bin/Resources/Fonts/Font_Info_24.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_INFO_36"), TEXT("../Bin/Resources/Fonts/Font_Info_36.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_INFO_48"), TEXT("../Bin/Resources/Fonts/Font_Info_48.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_INFO_60"), TEXT("../Bin/Resources/Fonts/Font_Info_60.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_INFO_72"), TEXT("../Bin/Resources/Fonts/Font_Info_72.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_TITLE_12"), TEXT("../Bin/Resources/Fonts/Font_Title_12.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_TITLE_18"), TEXT("../Bin/Resources/Fonts/Font_Title_18.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_TITLE_24"), TEXT("../Bin/Resources/Fonts/Font_Title_24.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_TITLE_36"), TEXT("../Bin/Resources/Fonts/Font_Title_36.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_TITLE_48"), TEXT("../Bin/Resources/Fonts/Font_Title_48.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_TITLE_60"), TEXT("../Bin/Resources/Fonts/Font_Title_60.spritefont"));
	m_pGameInstance->Add_Font(TEXT("FONT_TITLE_72"), TEXT("../Bin/Resources/Fonts/Font_Title_72.spritefont"));

	m_vecFont_char[_int(UI_FONT::FONT_INFO_12)] = "FONT_INFO_12";
	m_vecFont_char[_int(UI_FONT::FONT_INFO_18)] = "FONT_INFO_18";
	m_vecFont_char[_int(UI_FONT::FONT_INFO_24)] = "FONT_INFO_24";
	m_vecFont_char[_int(UI_FONT::FONT_INFO_36)] = "FONT_INFO_36";
	m_vecFont_char[_int(UI_FONT::FONT_INFO_48)] = "FONT_INFO_48";
	m_vecFont_char[_int(UI_FONT::FONT_INFO_60)] = "FONT_INFO_60";
	m_vecFont_char[_int(UI_FONT::FONT_INFO_72)] = "FONT_INFO_72";
	m_vecFont_char[_int(UI_FONT::FONT_TITLE_12)] = "FONT_TITLE_12";
	m_vecFont_char[_int(UI_FONT::FONT_TITLE_18)] = "FONT_TITLE_18";
	m_vecFont_char[_int(UI_FONT::FONT_TITLE_24)] = "FONT_TITLE_24";
	m_vecFont_char[_int(UI_FONT::FONT_TITLE_36)] = "FONT_TITLE_36";
	m_vecFont_char[_int(UI_FONT::FONT_TITLE_48)] = "FONT_TITLE_48";
	m_vecFont_char[_int(UI_FONT::FONT_TITLE_60)] = "FONT_TITLE_60";
	m_vecFont_char[_int(UI_FONT::FONT_TITLE_72)] = "FONT_TITLE_72";

	m_vecFont_tchar[_int(UI_FONT::FONT_INFO_12)] = TEXT("FONT_INFO_12");
	m_vecFont_tchar[_int(UI_FONT::FONT_INFO_18)] = TEXT("FONT_INFO_18");
	m_vecFont_tchar[_int(UI_FONT::FONT_INFO_24)] = TEXT("FONT_INFO_24");
	m_vecFont_tchar[_int(UI_FONT::FONT_INFO_36)] = TEXT("FONT_INFO_36");
	m_vecFont_tchar[_int(UI_FONT::FONT_INFO_48)] = TEXT("FONT_INFO_48");
	m_vecFont_tchar[_int(UI_FONT::FONT_INFO_60)] = TEXT("FONT_INFO_60");
	m_vecFont_tchar[_int(UI_FONT::FONT_INFO_72)] = TEXT("FONT_INFO_72");
	m_vecFont_tchar[_int(UI_FONT::FONT_TITLE_12)] = TEXT("FONT_TITLE_12");
	m_vecFont_tchar[_int(UI_FONT::FONT_TITLE_18)] = TEXT("FONT_TITLE_18");
	m_vecFont_tchar[_int(UI_FONT::FONT_TITLE_24)] = TEXT("FONT_TITLE_24");
	m_vecFont_tchar[_int(UI_FONT::FONT_TITLE_36)] = TEXT("FONT_TITLE_36");
	m_vecFont_tchar[_int(UI_FONT::FONT_TITLE_48)] = TEXT("FONT_TITLE_48");
	m_vecFont_tchar[_int(UI_FONT::FONT_TITLE_60)] = TEXT("FONT_TITLE_60");
	m_vecFont_tchar[_int(UI_FONT::FONT_TITLE_72)] = TEXT("FONT_TITLE_72");
}

CUIRender* CUIRender::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIRender* pInstance = new CUIRender(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUIRender"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CUIRender::Clone(void* pArg)
{
	CUIRender* pInstance = new CUIRender(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CUIRender"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIRender::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);

	for (auto& iter : m_vecTextureInfo)
	{
		Safe_Release(iter->Texture);
		Safe_Delete(iter);
	}

	m_vecTextureInfo.clear();

	for (auto& iter : m_UIRenderlist)
		Safe_Delete(iter);

	m_vecFont_char.clear();
	m_vecFont_tchar.clear();

	

	m_UIRenderlist.clear();

	Safe_Release(m_pVIBufferCom);
}
