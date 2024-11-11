#include "Shader_Compute.h"

CShader_Compute::CShader_Compute(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CShader_Compute::CShader_Compute(const CShader_Compute& Prototype)
	: CComponent(Prototype)
	, m_pComputeShader(Prototype.m_pComputeShader)
{
	Safe_AddRef(m_pComputeShader);
}

HRESULT CShader_Compute::Initialize_Prototype(const _tchar* pShaderFilePath, const _char* pMainTag)
{
	ID3DBlob* pErrorBlob = { nullptr };
	ID3DBlob* pCS_Blob = { nullptr };
	if (GetFileAttributes(pShaderFilePath) == INVALID_FILE_ATTRIBUTES)
		int a = 0;

	// pMainTag : PS_MAINó�� � ���̴� ���� ������.
	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, nullptr, pMainTag, "cs_5_0", D3DCOMPILE_ENABLE_STRICTNESS,
		0, &pCS_Blob, &pErrorBlob)))
	{
		_char* pChar = static_cast<char*>(pErrorBlob->GetBufferPointer());
		string str = pChar;
		Safe_Release(pErrorBlob);
		return E_FAIL;
	}

	m_pDevice->CreateComputeShader(pCS_Blob->GetBufferPointer(), pCS_Blob->GetBufferSize(), nullptr, &m_pComputeShader);
	Safe_Release(pCS_Blob);

	return S_OK;
}

HRESULT CShader_Compute::Initialize(void* pArg)
{
	// ���� ���ø� ����� ����.
#pragma region FOR_EXAMPLE
	// - �̹��� ó��: �׷�� 16x16 �Ǵ� 32x32 ũ�� ����.
	// - ���� �ý���: �׷�� 256 �Ǵ� 512 ������ ũ�� ���� (1���� �迭 ����).
	_uint iNumThread = { 10 };

#pragma region Share Buffer
	//// ���� ���� ����
	//D3D11_BUFFER_DESC	BufferDesc = {};
	//BufferDesc.Usage = D3D11_USAGE_DEFAULT;		// GPU������ �����Ұž�
	//BufferDesc.ByteWidth = sizeof(_float) * iNumThread;	// �Ǽ��� 10�� �����.
	//BufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;	// UAV, SRV�� ���ε� �� �ž�.
	//BufferDesc.StructureByteStride = sizeof(_float);	// ���� 4����Ʈ��
	//BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// ������ ũ���� ����ü �迭�� ������ �ž�.

	//ID3D11Buffer* pSharedBuffer = nullptr;
	//if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, nullptr, &pSharedBuffer)))
	//	return E_FAIL;
#pragma endregion
	
#pragma region Constant Buffer
	//// Constant Buffer ����
	//D3D11_BUFFER_DESC	ConstantBufferDesc = {};
	//ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	
	//ConstantBufferDesc.ByteWidth = sizeof(_float2);	// 8����Ʈ¥����.
	//ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ������ۿ� ���ε� �� �ž�.
	//ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU���� ������ �ž�. 

	//ID3D11Buffer* pConstantBuffer = { nullptr };
	//if (FAILED(m_pDevice->CreateBuffer(&ConstantBufferDesc, nullptr, &pConstantBuffer)))
	//	return E_FAIL;
#pragma endregion

#pragma region UAV
	//// UAV ���� : ��ǻƮ ���̴����� ���� ������ �� ����.
	//D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	//UAVDesc.Format = DXGI_FORMAT_UNKNOWN;					// ����ȭ�� �����ͳ� Ư�� ������ ������ �� ���� ������
	//UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;	//UAV�� 1���� ���� Ÿ��
	//UAVDesc.Buffer.NumElements = iNumThread;

	//// �ؽ�ó�� ���
	//UAVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// �ؽ�ó ����
	//UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;	// 2D �ؽ�ó
	//UAVDesc.Texture2D.MipSlice = 0;							// ����� Mipmap ����

	//// �ؽ�ó �迭�� ���
	//_uint iArraySize = {};
	//UAVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;				// �ؽ�ó ����
	//UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;	// �ؽ�ó �迭
	//UAVDesc.Texture2DArray.ArraySize = iArraySize;				// �迭 ũ��
	//UAVDesc.Texture2D.MipSlice = 0;								// ����� Mipmap ����


	//ID3D11UnorderedAccessView* pUAV = nullptr;
	//if (FAILED(m_pDevice->CreateUnorderedAccessView(pSharedBuffer, &UAVDesc, &pUAV)))
	//	return E_FAIL;
#pragma endregion

#pragma region SRV
	//// SRV ���� : �б� ����.
	//D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	//SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	//SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	//SRVDesc.Buffer.NumElements = iNumThread;

	//ID3D11ShaderResourceView* pSRV = nullptr;
	//if (FAILED(m_pDevice->CreateShaderResourceView(pSharedBuffer, &SRVDesc, &pSRV)))
	//	return E_FAIL;
#pragma endregion
#pragma endregion

	return S_OK;
}

void CShader_Compute::Bind_UAV(ID3D11UnorderedAccessView* pUAV)
{
	m_pContext->CSSetUnorderedAccessViews(0, 1, &pUAV, nullptr);
}

void CShader_Compute::Bind_CBuffer(ID3D11Buffer* pConstantBuffer)
{
	m_pContext->CSSetConstantBuffers(0, 1, &pConstantBuffer);
}


void CShader_Compute::Execute_ComputeShader(_uint iThreadGroupX, _uint iThreadGroupY, _uint iThreadGroupZ)
{
	// ���̴� �����ϰ�
	m_pContext->CSSetShader(m_pComputeShader, nullptr, 0);

	// �����ϰ�
	m_pContext->Dispatch(iThreadGroupX, iThreadGroupY, iThreadGroupZ);

	// UAV �����ϰ�
	ID3D11UnorderedAccessView* nullUAV[] = { nullptr };
	m_pContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

	// SRV �����ϰ�
	ID3D11ShaderResourceView* nullSRV = { nullptr };
	m_pContext->CSSetShaderResources(0, 1, &nullSRV);

	// ���̴����� �����ϸ� ��
	m_pContext->CSSetShader(nullptr, nullptr, 0);
}

CShader_Compute* CShader_Compute::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const _char* pMainTag)
{
	CShader_Compute* pInstance = new CShader_Compute(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pMainTag)))
	{
		MSG_BOX(TEXT("Create Failed : CShader_Compute"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader_Compute::Clone(void* pArg)
{
	CShader_Compute* pInstance = new CShader_Compute(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Clone Failed : CShader_Compute"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader_Compute::Free()
{
	__super::Free();
	Safe_Release(m_pComputeShader);
}
