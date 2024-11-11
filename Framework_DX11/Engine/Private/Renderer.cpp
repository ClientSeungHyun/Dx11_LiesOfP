#include "..\Public\Renderer.h"
#include "GameObject.h"

#include "GameInstance.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "Texture.h"

_uint		g_iSizeX = 8192;
_uint		g_iSizeY = 4608;

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

void CRenderer::OnOff_IsRenderBloom()
{
	m_isUseBloom = !m_isUseBloom;
}

HRESULT CRenderer::Initialize()
{
	_uint			iNumViewports = { 1 };

	D3D11_VIEWPORT	ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_fSamplerRatio = 5.f; // �ٿ� �� ���ø� �� ����

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Priority"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_ARM"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_PickDepth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_PickObjectDepth"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_SINT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_CascadeShadow"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Height"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_LightDepth"), g_iSizeX, g_iSizeY, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_BackBuffer"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	// Bloom
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_DownSample0"), (_uint)ViewportDesc.Width / (_uint)m_fSamplerRatio, (_uint)ViewportDesc.Height / (_uint)m_fSamplerRatio, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_DownSample1"), (_uint)ViewportDesc.Width / (_uint)pow(m_fSamplerRatio,2), (_uint)ViewportDesc.Height / (_uint)pow(m_fSamplerRatio, 2), DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_DownSample2"), (_uint)ViewportDesc.Width / (_uint)pow(m_fSamplerRatio, 3), (_uint)ViewportDesc.Height / (_uint)pow(m_fSamplerRatio, 3), DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_BlurX0"), (_uint)ViewportDesc.Width / (_uint)pow(m_fSamplerRatio, 3), (_uint)ViewportDesc.Height / (_uint)pow(m_fSamplerRatio, 3), DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_BlurXY0"), (_uint)ViewportDesc.Width / (_uint)pow(m_fSamplerRatio, 3), (_uint)ViewportDesc.Height / (_uint)pow(m_fSamplerRatio, 3), DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_BlurX1"), (_uint)ViewportDesc.Width / (_uint)pow(m_fSamplerRatio, 2), (_uint)ViewportDesc.Height / (_uint)pow(m_fSamplerRatio, 2), DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_BlurXY1"), (_uint)ViewportDesc.Width / (_uint)pow(m_fSamplerRatio, 2), (_uint)ViewportDesc.Height / (_uint)pow(m_fSamplerRatio, 2), DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_BlurX2"), (_uint)ViewportDesc.Width / (_uint)m_fSamplerRatio, (_uint)ViewportDesc.Height / (_uint)m_fSamplerRatio, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_BlurXY2"), (_uint)ViewportDesc.Width / (_uint)m_fSamplerRatio, (_uint)ViewportDesc.Height / (_uint)m_fSamplerRatio, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_UpSample0"), (_uint)ViewportDesc.Width / (_uint)pow(m_fSamplerRatio, 2), (_uint)ViewportDesc.Height / (_uint)pow(m_fSamplerRatio, 2), DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Bloom_UpSample1"), (_uint)ViewportDesc.Width / (_uint)m_fSamplerRatio, (_uint)ViewportDesc.Height / (_uint)m_fSamplerRatio, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Distortion"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_SSAO"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_SSAO_BlurX"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_SSAO_BlurXY"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_HDR"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_LDR"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget_Array(TEXT("Target_Cascade"), (_uint)2560, (_uint)1440, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f), 3)))
		return E_FAIL;

	//Decal
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_DecalDiffuse"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_DecalNormal"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_DecalARM"), (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* MRT_Priority */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Priority"), TEXT("Target_Priority"))))
		return E_FAIL;

	/* MRT_GameObjects */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_ARM"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_PickDepth"))))
		return E_FAIL;

	/* MRT_Lights */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Specular"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_CascadeShadow"))))
		return E_FAIL;

	/* MRT_Height */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Height"), TEXT("Target_Height"))))
		return E_FAIL;

	/* MRT_ShadowObj*/
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_ShadowObj"), TEXT("Target_LightDepth"))))
		return E_FAIL;

	/* MRT_Picking */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Picking"), TEXT("Target_PickObjectDepth"))))
		return E_FAIL;
	
	/* MRT_Decal */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Decal"), TEXT("Target_DecalDiffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Decal"), TEXT("Target_DecalNormal"))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Decal"), TEXT("Target_DecalARM"))))
		return E_FAIL;

#pragma region SSAO
	/* MRT_SSAO */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_SSAO"), TEXT("Target_SSAO"))))
		return E_FAIL;

	/* MRT_SSAO_BlurX */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_SSAO_BlurX"), TEXT("Target_SSAO_BlurX"))))
		return E_FAIL;

	/* MRT_SSAO_BlurXY */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_SSAO_BlurXY"), TEXT("Target_SSAO_BlurXY"))))
		return E_FAIL;
#pragma endregion 

	/* MRT_HDR*/
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_HDR"), TEXT("Target_HDR"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LDR"), TEXT("Target_LDR"))))
		return E_FAIL;

	/* MRT_Distortion */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		return E_FAIL;
#pragma region Bloom
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_DownSample0"), TEXT("Target_Bloom_DownSample0"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_DownSample1"), TEXT("Target_Bloom_DownSample1"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_DownSample2"), TEXT("Target_Bloom_DownSample2"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_BlurX0"), TEXT("Target_Bloom_BlurX0"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_BlurXY0"), TEXT("Target_Bloom_BlurXY0"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_BlurX1"), TEXT("Target_Bloom_BlurX1"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_BlurXY1"), TEXT("Target_Bloom_BlurXY1"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_BlurX2"), TEXT("Target_Bloom_BlurX2"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_BlurXY2"), TEXT("Target_Bloom_BlurXY2"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_UpSample0"), TEXT("Target_Bloom_UpSample0"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Bloom_UpSample1"), TEXT("Target_Bloom_UpSample1"))))
		return E_FAIL;
#pragma endregion

	// ĳ��ĳ�̵� ������Ʈ �׸���뵵
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Cascade"), TEXT("Target_Cascade"))))
		return E_FAIL;

	/* MRT_Final */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_BackBuffer"), TEXT("Target_BackBuffer"))))
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pBloomShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Bloom.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pBloomShader)
		return E_FAIL;

	m_pSSAOShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_SSAO.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pSSAOShader)
		return E_FAIL;
	m_pNoiseTexture_SSAO = CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/T_Tile_Noise_01_C_LGS.dds"), 1);

	m_pHDRShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_HDR.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements, 2);
	if (nullptr == m_pHDRShader)
		return E_FAIL;
	//m_pHDRShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_HDR.hlsl"));
	//if (nullptr == m_pHDRShader)
	//	return E_FAIL;

	m_pDistortionShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Distortion.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pDistortionShader)
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.0f, 1.f));

	if (FAILED(Ready_LightDepthStencilView()))
		return E_FAIL;

	if (FAILED(Ready_CascadeDepthStencilView()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_SSAO_BlurXY"), 100.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Bloom_BlurXY2"), 100.f, 500.f, 200.f, 200.f)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Specular"), 300.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_ARM"), 300.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_HDR"), 300.f,500.f, 200.f, 200.f)))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shade"), 300.f,500.f, 200.f, 200.f)))
	//	return E_FAIL;
#endif

	if (FAILED(Ready_HDR()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERGROUP eRenderGroupID, CGameObject * pRenderObject)
{
	if (nullptr == pRenderObject || 
		eRenderGroupID >= RG_END)
		return E_FAIL;

	/* ���� ������Ʈ �Ŵ����� �߰��� ��ü�� �ּҸ� ���⿡�ٰ� �����ߴ�. */
	m_RenderObjects[eRenderGroupID].emplace_back(pRenderObject);
	Safe_AddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::Draw()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_Height()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_Picking()))
		return E_FAIL;

	if (FAILED(Render_Decal()))
		return E_FAIL;

	if (FAILED(Render_Cascade()))
		return E_FAIL;
	if (FAILED(Render_Lights()))
		return E_FAIL;
	//if (FAILED(Render_ShadowObj()))
	//	return E_FAIL;


	if (FAILED(Render_Deferred()))
		return E_FAIL;

	if (FAILED(Render_SSAO()))
		return E_FAIL;

	if (FAILED(Render_HDR()))
		return E_FAIL;

	if (FAILED(Render_Bloom()))	// �ٽ� ��ġ��
		return E_FAIL;

	if(FAILED(Render_LDR()))
		return E_FAIL;

	if (FAILED(Render_Distortion()))
		return E_FAIL;

	if (FAILED(Render_NonLights()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

#ifdef _DEBUG
	if (KEY_TAP(KEY::F1))
		m_isDebugRender = !m_isDebugRender;

	if (FAILED(Render_Debug()))
		return E_FAIL;

#endif	

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Priority"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RG_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_PRIORITY].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Height()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Height"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RG_HEIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Height();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_HEIGHT].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_ShadowObj()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_ShadowObj"), m_pLightDepthStencilView)))
		return E_FAIL;

	D3D11_VIEWPORT			ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));
	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = (_float)g_iSizeX;
	ViewportDesc.Height = (_float)g_iSizeY;
	ViewportDesc.MinDepth = 0.f;
	ViewportDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewportDesc); 

	for (auto& pGameObject : m_RenderObjects[RG_SHADOWOBJ])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_LightDepth();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_SHADOWOBJ].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));
	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = (_float)1280.f;
	ViewportDesc.Height = (_float)720.f;
	ViewportDesc.MinDepth = 0.f;
	ViewportDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewportDesc);

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	/* Diffuse + Normal */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RG_NONBLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_NONBLEND].clear();

	m_pGameInstance->Draw_Instance(0);
	

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Decal()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Decal"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RG_DECAL])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_DECAL].clear();


	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Lights()
{
	/* ShadeŸ���� ���ε��ϰ� �������� ����� �׷��ְڴ�. */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Lights"))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Inverse(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Inverse(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrices("g_CascadeViewMatrix", m_pGameInstance->Get_CascadeViewMatirx(), 3)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrices("g_CascadeProjMatrix", m_pGameInstance->Get_CascadeProjMatirx(), 3)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Vec4(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Diffuse"), "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_ARM"), "g_ARMTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Normal"), "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Depth"), "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Cascade"), "g_CascadeTextureArr")))
		return E_FAIL;

	m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4x4		ViewMatrix;
	XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(0.f, 20.f, -15.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));

	if (FAILED(m_pShader->Bind_Matrix("g_LightViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_LightProjMatrix", &m_pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Priority"), "g_PriorityTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Diffuse"), "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Normal"), "g_NormalTexture"))) // �Ƹ� ��� �ɰŰ����ѵ� Ȯ���غ���
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Shade"), "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Specular"), "g_SpecularTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_LightDepth"), "g_LightDepthTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_Bloom_BlurXY2"), "g_BloomTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_CascadeShadow"), "g_CascadeShadowTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_DecalDiffuse"), "g_DecalDiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, TEXT("Target_DecalDiffuse"), "g_DecalNormalTexture")))
		return E_FAIL;

	m_pShader->Begin(3);

	m_pVIBuffer->Bind_Buffers();

	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::Render_SSAO()
{
	if (FAILED(m_pSSAOShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pSSAOShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pSSAOShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pSSAOShader->Bind_Matrix("g_CameraViewMatrix", &m_pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pSSAOShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Inverse(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pSSAOShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Inverse(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_SSAO"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pSSAOShader, TEXT("Target_Normal"), "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pSSAOShader, TEXT("Target_Depth"), "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pNoiseTexture_SSAO->Bind_ShadeResource(m_pSSAOShader, "g_NoiseTexture", 0)))
		return E_FAIL;

	m_pSSAOShader->Begin(0);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_SSAO_BlurX"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pSSAOShader, TEXT("Target_SSAO"), "g_SSAOTexture")))
		return E_FAIL;

	m_pSSAOShader->Begin(1);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_SSAO_BlurXY"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pSSAOShader, TEXT("Target_SSAO_BlurX"), "g_SSAOTexture")))
		return E_FAIL;

	m_pSSAOShader->Begin(2);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;


	if (FAILED(Copy_BackBuffer()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pSSAOShader, TEXT("Target_SSAO_BlurXY"), "g_SSAOTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pSSAOShader, TEXT("Target_BackBuffer"), "g_BackTexture")))
		return E_FAIL;

	m_pSSAOShader->Begin(3);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::Render_HDR()
{
	Copy_BackBuffer();

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_HDR"))))
		return E_FAIL;

	if (FAILED(m_pHDRShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pHDRShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pHDRShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pHDRShader, TEXT("Target_BackBuffer"), "g_BackTexture")))
		return E_FAIL;

	m_pHDRShader->Begin(0);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	//_uint vRes[2] = { _uint(1280.f / 4), _uint(720.f / 4) };

	//m_pHDRShader->Bind_RawValue("g_vRes", vRes, sizeof(_uint) * 2);

	//_uint iDomain = vRes[0] * vRes[1];
	//if (FAILED(m_pHDRShader->Bind_RawValue("g_iDomain", &iDomain, sizeof(_uint))))
	//return E_FAIL;

	//if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pHDRShader, TEXT("Target_BackBuffer"), "g_BackTexture")))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->BInd_RT_UnorderedView(m_pHDRShader, TEXT("Target_Test0"), "g_OutputAvgLum")))
	//	return E_FAIL;

	//if (FAILED(m_pHDRShader->Begin(0)))
	//	return E_FAIL;
	//m_pContext->Dispatch((UINT)ceil((_float)(1280.f / 4 * 720.f / 4) / 1024.f), 1, 1);

	//// ���� ��꿡 ������ ���� �ʱ� ���� Ŭ�����Ŵ �ִ� 128�� 8��
	//m_pContext->CSSetShaderResources(0, 128, m_pClearSRV);
	//m_pContext->CSSetUnorderedAccessViews(0, 8, m_pClearUAV, nullptr);

	//_uint iGroupSize = (UINT)ceil((_float)(1280.f / 4.f * 720.f / 4.f) / 1024.f);
	//m_pHDRShader->Bind_RawValue("g_iGroupSize", &iGroupSize, sizeof(_uint));

	//if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pHDRShader, TEXT("Target_BackBuffer"), "g_BackTexture")))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->BInd_RT_UnorderedView(m_pHDRShader, TEXT("Target_Test1"), "g_OutputAvgLum")))
	//	return E_FAIL;
	//m_pHDRShader->Begin(1);
	//m_pContext->Dispatch(1, 1, 1);	

	//m_pContext->CSSetShaderResources(0, 128, m_pClearSRV);
	//m_pContext->CSSetUnorderedAccessViews(0, 8, m_pClearUAV, nullptr);

	//D3D11_BUFFER_DESC bufferDesc = {};
	//bufferDesc.Usage = D3D11_USAGE_STAGING;          // CPU ������ ���� Staging
	//bufferDesc.BindFlags = 0;                       // Bind flags ����
	//bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // �б� ���� CPU ����
	//bufferDesc.MiscFlags = 0;
	//bufferDesc.StructureByteStride = sizeof(float);   // ������ ũ��

	//// UAV�� ����� ���ۿ� ���� ũ��� Staging Buffer�� �����մϴ�.
	//bufferDesc.ByteWidth = sizeof(float); // ���� ���, UAV�� 1���� float�� ����Ǿ� �ִٸ� �� ũ�⸸ŭ

	//ID3D11Buffer* pStagingBuffer = nullptr;
	//HRESULT hr = m_pDevice->CreateBuffer(&bufferDesc, nullptr, &pStagingBuffer);
	//if (FAILED(hr)) {
	//	return hr; // ���� ���� ���� �� ó��
	//}

	//// 2. GPU���� Staging Buffer�� ������ ����
	//// GPU���� GPU�� �����͸� �����ϱ� ���� CopyResource ���
	//ID3D11Buffer* pBuffer = m_pGameInstance->Get_Buffer(TEXT("Target_Test1"));
	//m_pContext->CopyResource(pStagingBuffer, pBuffer); // pHDR0UAV�� UAV ���ҽ�

	//// 3. CPU���� Staging Buffer ������ �б�
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//hr = m_pContext->Map(pStagingBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	//if (FAILED(hr)) {
	//	return hr; // Map ���� �� ó��
	//}

	//// Staging Buffer�� �����͸� ���� �� �ֽ��ϴ�.
	//float* pData = static_cast<float*>(mappedResource.pData);

	//// pData�� ����Ͽ� CPU���� �����͸� ó���մϴ�.
	//// ���� ���, ù ��° ���� �д´ٸ�:
	//float value = pData[0];

	//// 4. Map ����
	//m_pContext->Unmap(pStagingBuffer, 0);

	//// �ʿ��ϴٸ� Staging Buffer�� �����մϴ�.
	//pStagingBuffer->Release();


	return S_OK;
}

HRESULT CRenderer::Render_Bloom()
{
	if (!m_isUseBloom)
	{
		m_pGameInstance->Clear_MRT(TEXT("MRT_Bloom_BlurXY2"));

		return S_OK;
	}

	if (FAILED(Copy_BackBuffer()))
		return E_FAIL;

	if (FAILED(m_pBloomShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pBloomShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pBloomShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float fInitWidth = ViewportDesc.Width;
	_float fInitHeight = ViewportDesc.Height;

	_bool isSelectBright = true;

#pragma region �ٿ� ���ø�
	// �ٿ� ���� 1��
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_DownSample0"), m_pDownSampleDepthStencilView0)))
		return E_FAIL;

	ViewportDesc.Width = fInitWidth / m_fSamplerRatio;
	ViewportDesc.Height = fInitHeight / m_fSamplerRatio;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_HDR"), "g_DownSampleTexture")))
		return E_FAIL;

	if (FAILED(m_pBloomShader->Bind_RawValue("g_isSelectBright", &isSelectBright, sizeof(_bool))))
		return E_FAIL;

	m_pBloomShader->Begin(1);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	// �ٿ� ���� 2��
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_DownSample1"), m_pDownSampleDepthStencilView1)))
		return E_FAIL;

	isSelectBright = false;
	ViewportDesc.Width = (_float)(fInitWidth / pow(m_fSamplerRatio, 2));
	ViewportDesc.Height = (_float)(fInitHeight / pow(m_fSamplerRatio, 2));

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_DownSample0"), "g_DownSampleTexture")))
		return E_FAIL;

	if (FAILED(m_pBloomShader->Bind_RawValue("g_isSelectBright", &isSelectBright, sizeof(_bool))))
		return E_FAIL;

	m_pBloomShader->Begin(1);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	// �ٿ� ���� 3��
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_DownSample2"), m_pDownSampleDepthStencilView2)))
		return E_FAIL;

	ViewportDesc.Width = (_float)(fInitWidth / pow(m_fSamplerRatio, 3));
	ViewportDesc.Height = (_float)(fInitHeight / pow(m_fSamplerRatio, 3));

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_DownSample1"), "g_DownSampleTexture")))
		return E_FAIL;

	m_pBloomShader->Begin(1);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
#pragma endregion

#pragma region �� �����ø�1
	// ��X
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_BlurX0"), m_pDownSampleDepthStencilView2)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_DownSample2"), "g_DownSampleTexture")))
		return E_FAIL;

	_Vec2 vScreenSize = _Vec2((_float)(fInitWidth / pow(m_fSamplerRatio, 3)), ViewportDesc.Height = (_float)(fInitHeight / pow(m_fSamplerRatio, 3)));
	if (FAILED(m_pBloomShader->Bind_RawValue("g_vScreenSize", &vScreenSize, sizeof(_Vec2))))
		return E_FAIL;

	if (FAILED((m_pBloomShader->Begin(2))))
		return E_FAIL;

	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	// ��Y
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_BlurXY0"), m_pDownSampleDepthStencilView2)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_BlurX0"), "g_DownSampleTexture")))
		return E_FAIL;

	m_pBloomShader->Begin(3);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	// �� ����
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_UpSample0"), m_pDownSampleDepthStencilView1)))
		return E_FAIL;

	ViewportDesc.Width = (_float)(fInitWidth / pow(m_fSamplerRatio, 2));
	ViewportDesc.Height = (_float)(fInitHeight / pow(m_fSamplerRatio, 2));

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_BlurXY0"), "g_BlurTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_DownSample2"), "g_DownSampleTexture")))
		return E_FAIL;

	m_pBloomShader->Begin(4);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
#pragma endregion

#pragma region �� �����ø�2
	// ��X
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_BlurX1"), m_pDownSampleDepthStencilView1)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_UpSample0"), "g_DownSampleTexture")))
		return E_FAIL;

	vScreenSize = _Vec2((_float)(fInitWidth / pow(m_fSamplerRatio, 2)), ViewportDesc.Height = (_float)(fInitHeight / pow(m_fSamplerRatio, 2)));
	if (FAILED(m_pBloomShader->Bind_RawValue("g_vScreenSize", &vScreenSize, sizeof(_Vec2))))
		return E_FAIL;

	if (FAILED((m_pBloomShader->Begin(2))))
		return E_FAIL;

	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	// ��Y
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_BlurXY1"), m_pDownSampleDepthStencilView1)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_BlurX1"), "g_DownSampleTexture")))
		return E_FAIL;

	m_pBloomShader->Begin(3);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	// �� ����
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_UpSample1"), m_pDownSampleDepthStencilView0)))
		return E_FAIL;

	ViewportDesc.Width = (_float)(fInitWidth / m_fSamplerRatio);
	ViewportDesc.Height = (_float)(fInitHeight / m_fSamplerRatio);

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_BlurXY1"), "g_BlurTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_UpSample0"), "g_DownSampleTexture")))
		return E_FAIL;

	m_pBloomShader->Begin(4);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
#pragma endregion

#pragma region ������ �� ���� �����̶� ����
	// ��X
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_BlurX2"), m_pDownSampleDepthStencilView0)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_UpSample1"), "g_DownSampleTexture")))
		return E_FAIL;

	vScreenSize = _Vec2((_float)(fInitWidth / m_fSamplerRatio), ViewportDesc.Height = (_float)(fInitHeight / m_fSamplerRatio));
	if (FAILED(m_pBloomShader->Bind_RawValue("g_vScreenSize", &vScreenSize, sizeof(_Vec2))))
		return E_FAIL;

	if (FAILED((m_pBloomShader->Begin(2))))
		return E_FAIL;

	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	// ��Y
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Bloom_BlurXY2"), m_pDownSampleDepthStencilView0)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pBloomShader, TEXT("Target_Bloom_BlurX2"), "g_DownSampleTexture")))
		return E_FAIL;

	m_pBloomShader->Begin(3);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;
#pragma endregion

	/* ������� ������ */
	ViewportDesc.Width = fInitWidth;
	ViewportDesc.Height = fInitHeight;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	return S_OK;
}

HRESULT CRenderer::Render_LDR()
{
	//if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LDR"))))
	//	return E_FAIL;

	if (FAILED(m_pHDRShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pHDRShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pHDRShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pHDRShader, TEXT("Target_HDR"), "g_BackTexture")))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pHDRShader, TEXT("Target_Test1"), "g_Avg")))
	//	return E_FAIL;
	
	m_pHDRShader->Begin(1);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	//if (FAILED(m_pGameInstance->End_MRT()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Distortion()
{
	if (FAILED(Copy_BackBuffer()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Distortion"))))
		return E_FAIL;

	if (FAILED(m_pDistortionShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pDistortionShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pDistortionShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RG_DISTORTION])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}

	m_RenderObjects[RG_DISTORTION].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pDistortionShader, TEXT("Target_BackBuffer"), "g_BackTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pDistortionShader, TEXT("Target_Distortion"), "g_DistortionTexture")))
		return E_FAIL;

	m_pDistortionShader->Begin(0);

	m_pVIBuffer->Bind_Buffers();

	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::Render_Final()
{
	
	return S_OK;
}

HRESULT CRenderer::Render_Picking()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Picking"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RG_PICKING])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Picking();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_PICKING].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonLights()
{
	for (auto& pGameObject : m_RenderObjects[RG_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	/*WeightBlend*/

	_Vec3 vCamPosition = m_pGameInstance->Get_CamPosition_Vec3();
	m_RenderObjects[RG_BLEND].sort([&](CGameObject* pSour, CGameObject* pDest)->_bool
		{
			_float fDistSour, fDistDest;
			fDistSour = XMVectorGetX(XMVector3Length(pSour->Get_Transform()->Get_State(CTransform::STATE_POSITION) - vCamPosition));
			fDistDest = XMVectorGetX(XMVector3Length(pDest->Get_Transform()->Get_State(CTransform::STATE_POSITION) - vCamPosition));

			return fDistSour > fDistDest;
		});

	for (auto& pGameObject : m_RenderObjects[RG_BLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	/*m_RenderObjects[RG_UI].sort();*/

	for (auto& pGameObject : m_RenderObjects[RG_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Cascade()
{
	D3D11_VIEWPORT			ViewportDesc;
	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));
	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = 2560;
	ViewportDesc.Height = 1440;
	ViewportDesc.MinDepth = 0.f;
	ViewportDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewportDesc);

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Cascade"), m_pCascadeDepthStencilViewArr)))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[RG_SHADOWOBJ])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_LightDepth();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_SHADOWOBJ].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	ZeroMemory(&ViewportDesc, sizeof(D3D11_VIEWPORT));
	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = (_float)1280.f;
	ViewportDesc.Height = (_float)720.f;
	ViewportDesc.MinDepth = 0.f;
	ViewportDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewportDesc);

	return S_OK;
}

HRESULT CRenderer::Ready_LightDepthStencilView()
{
	ID3D11Texture2D*		pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = g_iSizeX;
	TextureDesc.Height = g_iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;	/* ���Ŀ� � �뵵�� ���ε� �� �� �ִ� ViewŸ���� �ؽ��ĸ� ��������� Texture2D�Դϱ�? */
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL	/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pLightDepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);



	/* Bloom */
	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = (_uint)(ViewportDesc.Width / m_fSamplerRatio);
	TextureDesc.Height = (_uint)(ViewportDesc.Height / m_fSamplerRatio);
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;	/* ���Ŀ� � �뵵�� ���ε� �� �� �ִ� ViewŸ���� �ؽ��ĸ� ��������� Texture2D�Դϱ�? */
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL	/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDownSampleDepthStencilView0)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);
	

	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = (_uint)(ViewportDesc.Width / pow(m_fSamplerRatio, 2));
	TextureDesc.Height = (_uint)(ViewportDesc.Height / pow(m_fSamplerRatio, 2));
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;	/* ���Ŀ� � �뵵�� ���ε� �� �� �ִ� ViewŸ���� �ؽ��ĸ� ��������� Texture2D�Դϱ�? */
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL	/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDownSampleDepthStencilView1)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);


	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = (_uint)(ViewportDesc.Width / pow(m_fSamplerRatio, 3));
	TextureDesc.Height = (_uint)(ViewportDesc.Height / pow(m_fSamplerRatio, 3));
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;	/* ���Ŀ� � �뵵�� ���ε� �� �� �ִ� ViewŸ���� �ؽ��ĸ� ��������� Texture2D�Դϱ�? */
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL	/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDownSampleDepthStencilView2)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

HRESULT CRenderer::Copy_BackBuffer()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_BackBuffer"))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_SRV("g_BackTexture", m_pGameInstance->Get_BackBuffer_SRV())))
		return E_FAIL;

	m_pShader->Begin(4);

	m_pVIBuffer->Bind_Buffers();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Ready_CascadeDepthStencilView()
{
	ID3D11Texture2D* pDepthStencilArrTexture = nullptr;


	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 2560;
	TextureDesc.Height = 1440;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 3;
	TextureDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilArrTexture)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	ZeroMemory(&DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	DepthStencilViewDesc.Flags = 0;
	DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	DepthStencilViewDesc.Texture2DArray.MipSlice = 0;
	DepthStencilViewDesc.Texture2DArray.ArraySize = 3; // 1�� �ؾ��ϳ�?
	DepthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilArrTexture, &DepthStencilViewDesc, &m_pCascadeDepthStencilViewArr)))
		return E_FAIL;

	Safe_Release(pDepthStencilArrTexture);


	return S_OK;
}

HRESULT CRenderer::Ready_HDR()
{
	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_uint iWidth = (_uint)ViewportDesc.Width;

	D3D11_BUFFER_DESC BufferDesc{};
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	BufferDesc.StructureByteStride = sizeof(_float);
	BufferDesc.ByteWidth = iWidth * BufferDesc.StructureByteStride;
	BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.NumElements = iWidth;

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;;
	ZeroMemory(&UAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.NumElements = iWidth;

	if (FAILED(m_pGameInstance->Add_RenderTarget_For_Desc(TEXT("Target_Test0"), &BufferDesc, nullptr, &SRVDesc, &UAVDesc, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	BufferDesc.StructureByteStride = sizeof(float);
	BufferDesc.ByteWidth = sizeof(_float);
	BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.NumElements = 1;

	ZeroMemory(&UAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.NumElements = 1;

	if (FAILED(m_pGameInstance->Add_RenderTarget_For_Desc(TEXT("Target_Test1"), &BufferDesc, nullptr, &SRVDesc, &UAVDesc, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Render_Debug()
{
	if (!m_isDebugRender)
		return S_OK;

	for (auto& pDebugCom : m_DebugObjects)
	{
			pDebugCom->Render();

		Safe_Release(pDebugCom);
	}

	m_DebugObjects.clear();

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Lights"), m_pShader, m_pVIBuffer); 
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Bloom_DownSample0"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Bloom_BlurXY0"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Bloom_BlurXY1"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Bloom_BlurXY2"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_SSAO_BlurXY"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_HDR"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_LDR"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_ShadowObj"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Bloom_BlurXY2"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Cascade"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Bloom_UpSample1"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Decal"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Picking"), m_pShader, m_pVIBuffer);

	return S_OK;
}

#endif

CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer*		pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CRenderer::Free()
{
	__super::Free();

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pGameObject : RenderObjects)
			Safe_Release(pGameObject);

		RenderObjects.clear();
	}

	Safe_Release(m_pDownSampleDepthStencilView2);
	Safe_Release(m_pDownSampleDepthStencilView1);
	Safe_Release(m_pDownSampleDepthStencilView0);

	Safe_Release(m_pLightDepthStencilView);
	Safe_Release(m_pCascadeDepthStencilViewArr);

	Safe_Release(m_pHDRShader);
	Safe_Release(m_pSSAOShader);
	Safe_Release(m_pNoiseTexture_SSAO);
	Safe_Release(m_pDistortionShader);

	Safe_Release(m_pBloomShader);
	Safe_Release(m_pShader);

	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
