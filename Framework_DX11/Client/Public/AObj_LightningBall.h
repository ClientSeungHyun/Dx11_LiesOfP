#pragma once
#include "AttackObject.h"
#include "Effect_Container.h"

class CAObj_LightningBall	final : public CAttackObject
{
private:
	enum ACTIVESTATE { STATE_START, STATE_MIDDLE, STATE_LAST, STATE_END };

protected:
	CAObj_LightningBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAObj_LightningBall(const CGameObject& Prototype);
	virtual ~CAObj_LightningBall() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_LightDepth() override;

public:
	virtual void OnCollisionEnter(CGameObject* pOther) override;
	virtual void OnCollisionStay(CGameObject* pOther) override;
	virtual void OnCollisionExit(CGameObject* pOther) override;

private:
	class CEffect_Container* m_pEffects[STATE_END]{ nullptr };

	CTransform* m_pCopyPlayerTransformCom = { nullptr };

	_int			m_iRouteCnt = {};

	_int			m_iThunderCnt = {};
	_float			m_fThunderTime = {};

	_float			m_fUpSpeed = {};
	_Vec3			m_vUp{};

	_bool			m_bEffectAlive = { true };
	_bool			m_bEnd = { false };

	_bool			m_bChangeState = { false };
	_float			m_fChangeTime = {};

private:
	HRESULT Ready_Components();

public:
	static CAObj_LightningBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

