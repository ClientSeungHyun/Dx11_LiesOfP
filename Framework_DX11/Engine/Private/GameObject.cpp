#include "..\Public\GameObject.h"
#include "GameInstance.h"


CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance() } 
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject & Prototype)
	: m_pDevice { Prototype.m_pDevice }
	, m_pContext { Prototype.m_pContext }
	, m_pGameInstance{ Prototype.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{	
	GAMEOBJECT_DESC*	pDesc = nullptr != pArg ? static_cast<GAMEOBJECT_DESC*>(pArg) : nullptr;

	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext, pDesc);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	/* �ٸ� ��ü�� �� ������Ʈ�� �˻��� �� �ֵ��� �ʿ��ٰ� ������Ʈ�� ��Ƶд�. */
	m_Components.emplace(g_strTransformTag, m_pTransformCom);

	Safe_AddRef(m_pTransformCom);

	m_isCloned = true;

	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CComponent * CGameObject::Find_Component(const _wstring & strComponentTag, _uint iPartObjIndex)
{
	auto	iter = m_Components.find(strComponentTag);

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

CComponent* CGameObject::Find_Component(const COMPONENT_TYPE eType)
{
	for (auto& Component : m_Components)
	{
		if (eType == Component.second->Get_ComponentType())
			return Component.second;
	}

	return nullptr;
}

CComponent* CGameObject::Change_Component(const _wstring& strComponentTag, CComponent* pComponent, _uint iPartObjIndex)
{
	CComponent* pOut = Find_Component(strComponentTag);
	if (pOut != nullptr)
	{
		Safe_Release(pOut);
		m_Components.erase(strComponentTag);
	}
	m_Components.emplace(strComponentTag, pComponent);
	Safe_AddRef(pComponent);

	return pOut;
}

HRESULT CGameObject::Add_Component(const _wstring& strComponentTag, CComponent* pComponent)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _wstring & strPrototypeTag, const _wstring & strComponentTag, CComponent** ppOut, void * pArg)
{
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

 	CComponent*		pComponent = m_pGameInstance->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);
	
	return S_OK;
}

void CGameObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
