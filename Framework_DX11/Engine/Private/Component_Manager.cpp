#include "..\Public\Component_Manager.h"
#include "Component.h"

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Initialize(_uint iNumLevels)
{
	if (nullptr != m_pPrototypes)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pPrototypes = new map<const _wstring, class CComponent*>[iNumLevels];
	m_pModelPrototypes = new map<const _char*, class CComponent*>[iNumLevels];

	return S_OK;
}

HRESULT CComponent_Manager::Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr != Find_Prototype(iLevelIndex, strPrototypeTag))
		return E_FAIL;

	m_pPrototypes[iLevelIndex].emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CComponent_Manager::Add_ModelPrototype(_uint iLevelIndex, const _char* strPrototypeTag, CComponent* pPrototype)
{
	if (nullptr != Find_ModelPrototype(iLevelIndex, strPrototypeTag))
		return E_FAIL;

	m_pModelPrototypes[iLevelIndex].emplace(strPrototypeTag, pPrototype);
	Safe_AddRef(pPrototype);

	return S_OK;
}

CComponent * CComponent_Manager::Clone_Component(_uint iLevelIndex, const _wstring & strPrototypeTag, void * pArg)
{
	CComponent*		pPrototype = Find_Prototype(iLevelIndex, strPrototypeTag);
	if(nullptr == pPrototype)		
		return nullptr;

	CComponent*		pComponent = pPrototype->Clone(pArg);
	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

void CComponent_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pPrototypes[iLevelIndex])
		Safe_Release(Pair.second);

	for (auto& Pair : m_pModelPrototypes[iLevelIndex])
		Safe_Release(Pair.second);

	m_pPrototypes[iLevelIndex].clear();
	m_pModelPrototypes[iLevelIndex].clear();
} 

CComponent * CComponent_Manager::Find_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag)
{
	auto	iter = m_pPrototypes[iLevelIndex].find(strPrototypeTag);

	if(iter == m_pPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CComponent* CComponent_Manager::Find_ModelPrototype(_uint iLevelIndex, const _char* strPrototypeTag)
{
	auto	iter = m_pModelPrototypes[iLevelIndex].find(strPrototypeTag);

	if (iter == m_pModelPrototypes[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CComponent_Manager * CComponent_Manager::Create(_uint iNumLevels)
{
	CComponent_Manager*		pInstance = new CComponent_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX(TEXT("Failed to Created : CComponent_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CComponent_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pPrototypes[i])
			Safe_Release(Pair.second);
		m_pPrototypes[i].clear();


		for (auto& Pair : m_pModelPrototypes[i])
			Safe_Release(Pair.second);
		m_pModelPrototypes[i].clear();
	}

	Safe_Delete_Array(m_pPrototypes);
	Safe_Delete_Array(m_pModelPrototypes);
}
