#pragma once

#include "Base.h"

BEGIN(Engine)

class CWorldOctree final : public CBase
{
private:
	CWorldOctree();
	virtual ~CWorldOctree() = default;

public:
	HRESULT Initialize(_Vec3 _vMinPos, _Vec3 _vMaxPos, vector<class CGameObject*>& m_pParentObjList, _int* iDepth = nullptr);
	
public:
	void Culling(CGameInstance* pGameInstance, unordered_set<_int>& pFrustumCulledNodes);
	_bool Get_haveChild() { return m_bHaveChild; }

private:
	vector<class CGameObject*> m_pObjectsList;
	CWorldOctree* m_pChildren[8] = { nullptr, };		//�ڽ� ���
	BoundingBox* m_pBoundBox;			//���� ��� AABB

	static int  iStaticOctreeIndex;
	_int m_iOctreeIndex = { 0 };
	_int m_iDepth = { -1 };				//��� ����
	_int m_iObjectCount = { 0 };		//��忡 ��� ������Ʈ ����

	_float m_fRadius = { 0.f };

	_bool m_bHaveChild = { false };

private:
	void Build_WorldOctree(vector<class CGameObject*>& m_pParentObjList);
	BoundingBox CalculateObjAABB(CModel* pModel);
	_bool IsIntersactWithBoundingBox(BoundingBox ObjBoundingBox);

public:
	static CWorldOctree* Create(_Vec3 _vMinPos, _Vec3 _vMaxPos, vector<class CGameObject*>& m_pParentObjList, _int* iDepth = nullptr);
	virtual void Free() override;
};


END