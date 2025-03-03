#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CRigidBody;
END

BEGIN(Client)

class CMoveBlockObj :
    public CGameObject
{
public:
	enum	BLOCKTYPE{RAXASIA1, RAXASIA2, MANUS1, TYPE_END};

	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		_uint		iTypeNum = { 0 };
	}MOVEBLOCK_DESC;

private:
	CMoveBlockObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMoveBlockObj(const CMoveBlockObj& Prototype);
	virtual ~CMoveBlockObj() = default;

public:
	_int	Get_Type() { return m_iType; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollisionEnter(CGameObject* pOther) override;
	virtual void OnCollisionStay(CGameObject* pOther) override;
	virtual void OnCollisionExit(CGameObject* pOther) override;

public:
	void		Open_Door();
	void		Close_Door();

private:
	CCollider* m_pColliderCom = { nullptr };
	CRigidBody* m_pRigidBodyCom = { nullptr };
	class CBossDoor_Veli* m_pVeliEffect = { nullptr };

private:
	_bool		m_bOpened = { false };
	_int		m_iType = {};

private:
	HRESULT Ready_Components(MOVEBLOCK_DESC* pDesc);
	HRESULT Ready_Effect(MOVEBLOCK_DESC* pDesc);

public:
	static CMoveBlockObj* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END