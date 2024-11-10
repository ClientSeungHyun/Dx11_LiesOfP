#pragma once


#include "VIBuffer_Point_Instance.h"
#include "VIBuffer_Rect_Instance.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "Navigation.h"
#include "Collider.h"
#include "Texture.h"
#include "Shader.h"
#include "Shader_NonVTX.h"
#include "Shader_Compute.h"
#include "Model.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

#include "Fsm.h"
#include "RigidBody.h"


/* ������Ʈ�� ������ �������� �����Ѵ�. */
/* �����Ͽ� �����Ѵ�.*/

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CComponent* pPrototype);
	HRESULT Add_ModelPrototype(_uint iLevelIndex, const _char* strPrototypeTag, class CComponent* pPrototype);
	map<const _char*, class CComponent*> Get_ModelPrototypes(_uint iLevelIndex) { return m_pModelPrototypes[iLevelIndex]; }

	class CComponent* Clone_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	void Clear(_uint iLevelIndex);

private:
	map<const _wstring, class CComponent*>*		m_pPrototypes = { nullptr };
	_uint										m_iNumLevels = { 0 };

	//�� �� ���п�
	map<const _char*, class CComponent*>*		m_pModelPrototypes = { nullptr };

private:
	class CComponent* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);
	class CComponent* Find_ModelPrototype(_uint iLevelIndex, const _char* strPrototypeTag);

public:
	static CComponent_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END