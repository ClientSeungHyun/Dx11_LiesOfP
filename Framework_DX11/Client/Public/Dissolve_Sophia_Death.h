#pragma once

#include "Dissolve_Effect.h"

BEGIN(Engine)
class CShader_NonVTX;
class CVIBuffer_Dissolve_Instance;
class CTexture;
class CShader_Compute;
class CModel;
END

BEGIN(Client)
class CDissolve_Sophia_Death final : public CDissolve_Effect
{
public:
	typedef struct : public CDissolve_Effect::DISSOLVE_EFFECT_DESC
	{
	}DISSOLVE_OBJECT_DESC;

private:
	CDissolve_Sophia_Death(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDissolve_Sophia_Death(const CDissolve_Sophia_Death& Prototype);
	virtual ~CDissolve_Sophia_Death() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg = nullptr);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	class CTexture* m_pTextureCom = nullptr;
	// ������ ��ǻƮ ���̴�
	class CShader_Compute* m_pActionCS = { nullptr };
	
	// ������ �ؽ�ó
	class CTexture* m_pDissolveTextureCom = { nullptr };

	// ���� ���̴��� ����
	_uint								m_iShaderIndex = { 0 };
	_uint								m_iGeomState = { 0 };
	_Vec2								m_vTexDevide = { 1.f ,1.f };
	_float								m_fSpriteSpeed = { 0.f };
	_Vec2								m_vStartScaling = { 1.f, 1.f };
	_Vec2								m_vScalingRatio = { 0.f, 0.f };
	_float								m_fStartRotation = { 0.f };
	_float								m_fRotationPerSecond = { 0.f };

	_float								m_fThreshold = {};
	_float2								m_vTextureSize = {};

private:
	HRESULT Ready_Componet();

public:
	static CDissolve_Sophia_Death* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END