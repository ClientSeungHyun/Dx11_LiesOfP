#pragma once

#include "Tools_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Tools)

class CAnimModel final : public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		_char		szLayerTag[MAX_PATH];
		_char		szPrototypeTag[MAX_PATH];
		_char		szModelTag[MAX_PATH];
		_float3		vPosition;
		_float3		vScale;
		_float3		vRotation;

		//�̺��� �߰�
		_bool*		pUpdateCtr;
	}ANIMMODEL_DESC;

private:
	CAnimModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAnimModel(const CAnimModel& Prototype);
	virtual ~CAnimModel() = default;

public:
	ANIMMODEL_DESC Get_NonAniModelDesc() { return m_tDesc; }
	void Set_NonAniModelDesc(ANIMMODEL_DESC* Desc) { memcpy(&m_tDesc, Desc, sizeof(ANIMMODEL_DESC)); }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };

	ANIMMODEL_DESC	m_tDesc;
	_vector		m_vRootMoveStack{};

	OUTPUT_EVKEY	m_EventKey{};
	OUTPUT_EVKEY	m_EventKey_Boundary{};

private:
	HRESULT Ready_Components(ANIMMODEL_DESC* pNonAnimDesc);
	virtual class CComponent* Change_Component(const _wstring& strComponentTag, CComponent* pComponent, _uint iPartObjIndex = 0);//�޾ƿ� ������Ʈ�� ��ü�ϰ� ������ ������Ʈ�� ��ȯ

public:
	static CAnimModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END