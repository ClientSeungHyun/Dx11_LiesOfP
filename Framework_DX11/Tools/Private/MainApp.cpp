#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "Level_Loading.h"
#include "GameInstance.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	/* ���� ���۷��� ī��Ʈ�� ������Ű�� ���� : */
	/* ���� �����ϱ����� �����Ϳ� ������ �Ҷ��� ������Ű�� �ʴ´�. */
	/* ���� ������ ��ġ�� �ִ� �ּҸ� �ٸ� �������������� ������ �ϰԵǸ� �̶��� ������Ų��. */
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC			EngineDesc{};
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowsed = true;

	/* ������ ����ϱ����� ���� �ʱ�ȭ ������ �����Ѵ�. */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component_Static()))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_TOOL)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_145"), TEXT("../Bin/Resources/Fonts/143ex.spritefont"))))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	m_pGameInstance->Update_Engine(fTimeDelta);

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif
}

HRESULT CMainApp::Render()
{

	
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Render_Begin();

	m_pGameInstance->Draw_Engine();

#ifdef _DEBUG
	++m_iNumDraw;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumDraw);
		m_fTimeAcc = 0.f;
		m_iNumDraw = 0;
	}

	m_pGameInstance->Render_Text(TEXT("Font_145"), m_szFPS, XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(1.f, 0.f, 0.f, 1.f));
#endif

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_Static()
{
	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Cell"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	///* For.Prototype_Component_Shader_VtxNorTex */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
	//	return E_FAIL;

	/* For. Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma region SHADER
	/* For. Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_SSD"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_SSD.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
#pragma endregion
	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eStartLevelID)
{	
	/* � ������ �����ϴ� �ε� ������ �� �����Ѵ�.  */
	/* �ε��������� ���� ������ ������ �ʿ��� �ڿ��� �غ��Ѵ�. */
	/* �ε� ������ ��������(���� ������ ����)�� ���������� �����ؾ��Ѵ�. */
	if (FAILED(m_pGameInstance->Change_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);
	
}

