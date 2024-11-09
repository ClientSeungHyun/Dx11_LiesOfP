#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader_BindStruct final : public CComponent
{
private:
	CShader_BindStruct(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader_BindStruct(const CShader_BindStruct& Prototype);
	virtual ~CShader_BindStruct() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Begin(_uint iPassIndex);

public:
	/* �� ���̴��� ����Ǿ��ִ� Ư�� ���������� ���� ������. */
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);	
	HRESULT Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices);
	HRESULT Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRVs, _uint iNumSRVs);
	HRESULT Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength);
	HRESULT Bind_CBuffer(const _char* pConstantName, ID3D11Buffer* pBuffer);

private:
	ID3DX11Effect*		m_pEffect = { nullptr };
	_uint				m_iNumPasses = { 0 };

public:
	static CShader_BindStruct* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END