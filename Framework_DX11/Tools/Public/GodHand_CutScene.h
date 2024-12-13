#pragma once

#include "Tools_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Tools)

class CGodHand_CutScene final : public CGameObject
{
private:
	CGodHand_CutScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGodHand_CutScene(const CGodHand_CutScene& Prototype);
	virtual ~CGodHand_CutScene() = default;

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

	_vector		m_vRootMoveStack{};

	_float		m_fEmissive = {};
	_float		m_fRimLight = {};

	_bool		m_bRender = { false };

	_bool		m_bRemoteTuning = true;

	_bool		m_bSubRootMove = false;
	_int		m_iSubRootBone{};
	_vector		m_vSubRootMoveStack{};

private:
	HRESULT Ready_Components();
	virtual class CComponent* Change_Component(const _wstring& strComponentTag, CComponent* pComponent, _uint iPartObjIndex = 0);//�޾ƿ� ������Ʈ�� ��ü�ϰ� ������ ������Ʈ�� ��ȯ

public:
	static CGodHand_CutScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END