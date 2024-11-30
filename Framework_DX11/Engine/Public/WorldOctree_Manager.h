#pragma once
#include "Base.h"


/* ������Ʈ�� ������ �������� �����Ѵ�. */
/* �����Ͽ� �����Ѵ�.*/

BEGIN(Engine)

class CWorldOctree_Manager final : public CBase
{
private:
	CWorldOctree_Manager();
	virtual ~CWorldOctree_Manager() = default;

public:
	HRESULT		Initialize();
	void		Update();

	void Create_Octree(_Vec3 vMinPos, _Vec3 vMaxPos);		//��Ʈ�� ����
	//ȭ�鿡 �ɸ��� octree ��� ��� �Լ�
	//m_FrustumCulledNodes�� �������� ������Ʈ���� �޾ƿ��� �װ� �� ��� �ؾ���
private:
	vector<_int> m_FrustumCulledNodes;
	class CWorldOctree* m_pWorldOctree = { nullptr };

public:
	static CWorldOctree_Manager* Create();
	virtual void Free() override;
};

END