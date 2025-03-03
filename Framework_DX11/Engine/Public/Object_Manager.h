#pragma once

#include "Base.h"

/* 객체들을 모아서 그룹(Layer)별로 저장한다. */
/* 모아 놓은 객체들의 반복적인 Update를 호출해준다. */
/* 모아 놓은 객체들의 반복적인 Render를 호출해준다.(x) : 객체들을 그리는 순서를 내가 따로 좀 지정을 해야할 필요가 있다. */

BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(const _wstring& strPrototypeTag, class CGameObject* pPrototype);	
	HRESULT Add_CloneObject_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strPrototypeTag, void* pArg);
	HRESULT Add_Object_ToLayer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* pGameObject);
	HRESULT Priority_Update(_float fTimeDelta);
	HRESULT Update(_float fTimeDelta);
	HRESULT Late_Update(_float fTimeDelta);
	void	Clear(_uint iLevelIndex);

public:
	class CGameObject*	Clone_GameObject(const _wstring& strPrototypeTag, void* pArg = nullptr);
	class CComponent*	Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex, _uint iPartObjIndex);
	class CLayer*		Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	class CGameObject*	Find_Object(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex);
	class CGameObject*	Find_Prototype(const _wstring& strPrototypeTag);

	class CGameObject*	Find_Player(_uint iLevelIndex = 0);
	class CCamera*		Find_Camera(_uint iLevelIndex = 0);
	class CGameObject*	Find_NearObject(class CGameObject* pOwner, _uint iLevelIndex, const _wstring& strLayerTag);

	_uint Get_Object_Layer_Count(_uint iLevelIndex);
	_wstring Get_LayerTag(_uint iLevelIndex, _uint index);

private:

	/* 원형객체들을 보관한다. */
	map<const _wstring, class CGameObject*>		m_Prototypes;

	/* 레벨별로, 사본객체들을 그룹별로 모아서 보관한다. */
	_uint										m_iNumLevels = {};
	map<const _wstring, class CLayer*>*			m_pLayers = { nullptr };
	typedef map<const _wstring, class CLayer*>		LAYERS;
	
	vector <_wstring> LayerTagList = {  };
public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END