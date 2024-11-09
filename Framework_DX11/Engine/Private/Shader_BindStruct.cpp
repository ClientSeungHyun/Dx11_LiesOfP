#include "..\Public\Shader_BindStruct.h"

CShader_BindStruct::CShader_BindStruct(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CShader_BindStruct::CShader_BindStruct(const CShader_BindStruct& Prototype)
	: CComponent{ Prototype }
	, m_pEffect{ Prototype.m_pEffect }
	, m_iNumPasses{ Prototype.m_iNumPasses }
{
	Safe_AddRef(m_pEffect);
}

HRESULT CShader_BindStruct::Initialize_Prototype(const _tchar* pShaderFilePath)
{
	_uint			iHlslFlag = { 0 };

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob* pErrorMsg = { nullptr };

	/* ���� �ۼ��� ���̴� ������ �����Ͽ� ��üȭ(ID3DX11Effect*) �ߴ�. */
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc{};

	pTechnique->GetDesc(&TechniqueDesc);

	m_iNumPasses = TechniqueDesc.Passes;

	return S_OK;
}

HRESULT CShader_BindStruct::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader_BindStruct::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_iNumPasses)
		return E_FAIL;

	ID3DX11EffectPass* pPass = m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex);
	if (nullptr == pPass)
		return E_FAIL;

	pPass->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader_BindStruct::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
	/* ���̴� �ȿ� ����Ǿ��ִ� ���������� �İ�ü �ڵ��� ���´�. */
	/* �������� �̸��� ���� ���������� �ִٸ� ��ȿ�� �����͸� ���´�.(Ÿ�Կ� ���� ���x) */
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	///* Ư�� �����鿡 ���ؼ��� ������ �ʰų� ��ȯ������ ������ �����Ѵ�.  */
	//pVariable->SetRawValue(void* , 0, �ѱ�����ϴ� �������� ũ��.);	

	/* ���� ���������� ����̾��ٸ� ��� Ÿ�Կ� ���� �ڵ��� �ٽ� ���´�. */
	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix((_float*)pMatrix);
}

HRESULT CShader_BindStruct::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices)
{
	/* ���̴� �ȿ� ����Ǿ��ִ� ���������� �İ�ü �ڵ��� ���´�. */
	/* �������� �̸��� ���� ���������� �ִٸ� ��ȿ�� �����͸� ���´�.(Ÿ�Կ� ���� ���x) */
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	///* Ư�� �����鿡 ���ؼ��� ������ �ʰų� ��ȯ������ ������ �����Ѵ�.  */
	//pVariable->SetRawValue(void* , 0, �ѱ�����ϴ� �������� ũ��.);	

	/* ���� ���������� ����̾��ٸ� ��� Ÿ�Կ� ���� �ڵ��� �ٽ� ���´�. */
	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray((_float*)pMatrices, 0, iNumMatrices);
}

HRESULT CShader_BindStruct::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	/* ���̴� �ȿ� ����Ǿ��ִ� ���������� �İ�ü �ڵ��� ���´�. */
	/* �������� �̸��� ���� ���������� �ִٸ� ��ȿ�� �����͸� ���´�.(Ÿ�Կ� ���� ���x) */
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResource(pSRV);
}

HRESULT CShader_BindStruct::Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRVs, _uint iNumSRVs)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResourceArray(ppSRVs, 0, iNumSRVs);
}

HRESULT CShader_BindStruct::Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iLength);
}

HRESULT CShader_BindStruct::Bind_CBuffer(const _char* pConstantName, ID3D11Buffer* pBuffer)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	// Constant Buffer�� ���ε��մϴ�.
	ID3DX11EffectConstantBuffer* pConstantBuffer = pVariable->AsConstantBuffer();
	if (nullptr == pConstantBuffer)
		return E_FAIL;

	pConstantBuffer->SetConstantBuffer(pBuffer);
	return S_OK;
}

CShader_BindStruct* CShader_BindStruct::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath)
{
	CShader_BindStruct* pInstance = new CShader_BindStruct(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CShader_BindStruct"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CComponent* CShader_BindStruct::Clone(void* pArg)
{
	CShader_BindStruct* pInstance = new CShader_BindStruct(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CShader_BindStruct"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CShader_BindStruct::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);
}
