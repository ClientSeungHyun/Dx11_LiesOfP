#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "Level_Loading.h"
#include "GameInstance.h"
#include "GameInterface_Controller.h"
#include "Effect_Manager.h"
#include "Camera_Manager.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);	
}

HRESULT CMainApp::Initialize()
{	
	/*_float		fData = 0.0561 + 0.1353 + 0.278 + 0.4868 + 0.7261 + 0.9231 + 1.f + 0.9231 + 0.7261 + 0.4868 + 0.278 + 0.1353 + 0.0561;*/


	ENGINE_DESC			EngineDesc{};
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowsed = true;

	/* ������ ����ϱ����� ���� �ʱ�ȭ ������ �����Ѵ�. */
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Gara()))
		return E_FAIL;	

	if (FAILED(Ready_Prototype_Component_Static()))
		return E_FAIL;

	// 24-11-09 �輺��
	// GameInterface ���� 
	if (FAILED(GET_GAMEINTERFACE->Initialize_GameInterface(&m_pDevice, &m_pContext, m_pGameInstance)))
		return E_FAIL;

	CCamera_Manager::Get_Instance()->Initialize(m_pDevice, m_pContext);

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	_float fObject_Time = fTimeDelta;

	if (GET_GAMEINTERFACE->IsGamePause())
		fObject_Time = 0.f;


	m_pGameInstance->Update_Engine(fObject_Time);

	// 24-12-01 �輺�� 
	// UI ���� ���� �׽�Ʈ �ڵ� 
	if (KEY_HOLD(KEY::CTRL))
	{
		if (KEY_TAP(KEY::NUM1))
			GET_GAMEINTERFACE->Show_Script(TEXT("�׽�Ʈ"), TEXT("�׽�Ʈ"), 3.f);
		if (KEY_TAP(KEY::NUM2))
			GET_GAMEINTERFACE->Show_Script(TEXT("�׽�Ʈ�׽�Ʈ"), TEXT("�׽�Ʈ�׽�Ʈ"), 3.f);
		if (KEY_TAP(KEY::NUM3))
			GET_GAMEINTERFACE->Fade_Out(TEXT("FadeOut_Test"), TEXT("FadeOut_Desc"), { 0.f,0.f,0.f }, 2.f);
		if (KEY_TAP(KEY::NUM4))
			GET_GAMEINTERFACE->Fade_In(2.f);
		if (KEY_TAP(KEY::NUM4))
			GET_GAMEINTERFACE->Fade_In(2.f);
		if (KEY_TAP(KEY::NUM6))
			GET_GAMEINTERFACE->Fade_Out(TEXT("FadeOut_Test"), TEXT("FadeOut_Desc"), { 0.f,0.f,0.f }, 2.f);
		if (KEY_TAP(KEY::NUM7))
			GET_GAMEINTERFACE->Fade_In(2.f);
		if (KEY_TAP(KEY::NUM8))
			GET_GAMEINTERFACE->Show_Script_Npc_Talking(NPC_SCRIPT::SCR_SOPIA_FIRST);
		if (KEY_TAP(KEY::NUM9))
			GET_GAMEINTERFACE->UIPart_On();

		if (GET_GAMEINTERFACE->IsTlaking_WithNPC() == true)
			_int i = 0;
	}



	if (KEY_HOLD(KEY::ALT))
	{
		if (KEY_TAP(KEY::F5))
			GET_GAMEINTERFACE->Show_Region_Info(TEXT("�𳪽�Ʈ��"), TEXT("�帰 ����"));
		if (KEY_TAP(KEY::F6))
			GET_GAMEINTERFACE->Show_Inform(INFORM_MESSAGE::INFORM_BOSSKILL);
		if (KEY_TAP(KEY::F7))
			GET_GAMEINTERFACE->Show_Inform(INFORM_MESSAGE::INFORM_DEAD);
		if (KEY_TAP(KEY::F8))
			GET_GAMEINTERFACE->Show_Inform(INFORM_MESSAGE::INFORM_RECOVERY);
		if (KEY_TAP(KEY::F9))
			GET_GAMEINTERFACE->Show_Inform(INFORM_MESSAGE::INFORM_STARGAZER);
		if (KEY_TAP(KEY::NUM5))
			GET_GAMEINTERFACE->Show_Heart(TEXT("������ ��ģ��"));
	}

	

	
	if (KEY_TAP(KEY::M))
	{
		g_isHideCursor = !g_isHideCursor;

		if (g_isHideCursor)
		{
			//ShowCursor(FALSE);
			RECT rect;
			GetClientRect(g_hWnd, &rect); // ���� â�� Ŭ���̾�Ʈ ������ ����
			ClientToScreen(g_hWnd, (POINT*)&rect.left);  // ��ǥ�� ȭ�� ��ǥ�� ��ȯ
			ClientToScreen(g_hWnd, (POINT*)&rect.right);
			ClipCursor(&rect); // ���콺�� ���� â�� ������ ����
		}
		else
		{
			//ShowCursor(TRUE);
			ClipCursor(NULL);
		}
	}
	else if (KEY_TAP(KEY::O))
	{
		m_pGameInstance->Toggle_Shadow();
	}


	
	// 24-11-09 �輺��
	// GameInterface ������Ʈ 
	// �� �������̽� ������Ʈ�� �и��ϴ� ���� 
	// -> �ٸ� ��ü�� ������Ʈ�� ��� �����ϸ鼭 �ش� �������� Ȱ�� �� �������̽� ��û�� �Ϸ� 
	// -> ���Ŀ� �ش� �����ӿ� ��ϵ� ������ �ϰ� ó���Ͽ� �������̽��� �ݿ�
	// -> ���� �����ӿ� ���� ���� �ݿ��� �������̽� ���� 
	//  �� ������ �����ϱ� ���� �������̽� ���� ������Ʈ�� ������ �ٸ� ��ü���� ���߿� �����ϱ� ���� 
	GET_GAMEINTERFACE->Update_GameInterface(fTimeDelta);

	
//#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
//#endif
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Render_Begin();

	m_pGameInstance->Draw_Engine();

	/* MakeSpriteFont "�ؽ�lv1��� Bold" /FontSize:10 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 145ex.spritefont */
	

	++m_iNumDraw;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumDraw);
		m_fTimeAcc = 0.f;
		m_iNumDraw = 0;
	}

	m_pGameInstance->Render_Text(TEXT("Font_145"), m_szFPS, XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(1.f, 0.f, 0.f, 1.f));


	m_pGameInstance->Render_End();

	return S_OK;
}


HRESULT CMainApp::Ready_Gara()
{
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Gulim"), TEXT("../Bin/Resources/Fonts/Gulim.spritefont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_145"), TEXT("../Bin/Resources/Fonts/143ex.spritefont"))))
		return E_FAIL;

	//ID3D11Texture2D*		pTexture2D = { nullptr };

	//D3D11_TEXTURE2D_DESC	TextureDesc{};
	//ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	//TextureDesc.Width = 256;
	//TextureDesc.Height = 256;
	//TextureDesc.MipLevels = 1;
	//TextureDesc.ArraySize = 1;
	//TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//TextureDesc.SampleDesc.Quality = 0;
	//TextureDesc.SampleDesc.Count = 1;

	//TextureDesc.Usage = D3D11_USAGE_STAGING;
	///* ���Ŀ� � �뵵�� ���ε� �� �� �ִ� ViewŸ���� �ؽ��ĸ� ��������� Texture2D�Դϱ�? */
	//TextureDesc.BindFlags = 0;

	//TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	//TextureDesc.MiscFlags = 0;

	//_uint*			pPixel = new _uint[TextureDesc.Width * 	TextureDesc.Height];
	//ZeroMemory(pPixel, sizeof(_uint) * TextureDesc.Width * 	TextureDesc.Height);

	//D3D11_SUBRESOURCE_DATA	InitialDesc{};
	//InitialDesc.pSysMem = pPixel;
	//InitialDesc.SysMemPitch = TextureDesc.Width * sizeof(_uint);

	//if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialDesc, &pTexture2D)))
	//	return E_FAIL;	

	//D3D11_MAPPED_SUBRESOURCE	SubResource{};

	//m_pContext->Map(pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &SubResource);

	///* a b g r */
	//for (size_t i = 0; i < 256; i++)
	//{
	//	for (size_t j = 0; j < 256; j++)
	//	{
	//		_uint iIndex = (_uint)i * 256 + (_uint)j;

	//		if(j < 128)
	//			((_uint*)SubResource.pData)[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);			
	//		else
	//			((_uint*)SubResource.pData)[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
	//	}
	//}

	//m_pContext->Unmap(pTexture2D, 0);

	//if (FAILED(SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/MyMask.dds"))))
	//	return E_FAIL;

	//_ulong			dwByte = {};
	//HANDLE			hFile = CreateFile(TEXT("../Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	//_float3			vPoints[3];

	//vPoints[0] = _float3(0.0f, 0.f, 10.f);
	//vPoints[1] = _float3(10.f, 0.f, 0.f);
	//vPoints[2] = _float3(0.0f, 0.f, 0.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	//vPoints[0] = _float3(0.0f, 0.f, 10.f);
	//vPoints[1] = _float3(10.f, 0.f, 10.f);
	//vPoints[2] = _float3(10.f, 0.f, 0.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	//vPoints[0] = _float3(0.0f, 0.f, 20.f);
	//vPoints[1] = _float3(10.f, 0.f, 10.f);
	//vPoints[2] = _float3(0.f, 0.f, 10.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	//vPoints[0] = _float3(10.f, 0.f, 10.f);
	//vPoints[1] = _float3(20.f, 0.f, 0.f);
	//vPoints[2] = _float3(10.f, 0.f, 0.f);
	//WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	//CloseHandle(hFile);

	//Safe_Delete_Array(pPixel);
	//Safe_Release(pTexture2D);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_Static()
{
#pragma region Shader
	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;	

	/* For. Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxCubeTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX::Elements, VTXCUBETEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModelInstnace */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModelInstnace"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel_Instance.hlsl"), VTXMODELINSTANCE::Elements, VTXMODELINSTANCE::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_SSD"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_SSD.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
#pragma endregion

#pragma region VIBuffer
	/* For. Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	/* For. Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CVIBuffer_2DPolygon::UIPOLIGON_DESC tDesc{};
	tDesc.iPoint = 8;
	tDesc.fAngle = 0.f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_2DPolygon"),
		CVIBuffer_2DPolygon::Create(m_pDevice, m_pContext, tDesc))))
		return E_FAIL;
#pragma endregion

#pragma region Collider
	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;
#pragma endregion

#pragma region Component
	/* For. Prototype_Component_RigidBody */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		CRigidBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_FSM */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_FSM"),
		CFsm::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Sound */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Sound"),
		CSound::Create(m_pDevice, m_pContext))))
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

	// 24-11-09 �輺��
	// GameInterface ������
	GET_GAMEINTERFACE->Release_GameInterface();

	Safe_Release(m_pGameInstance);
	
	CEffect_Manager::Destroy_Instance();
	CCamera_Manager::Destroy_Instance();


	ClipCursor(NULL); // ���콺�� ���� ����
	ShowCursor(TRUE);  // ���콺 Ŀ���� �ٽ� ���̰� ��
}

