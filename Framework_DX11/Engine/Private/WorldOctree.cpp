#include "WorldOctree.h"
#include "GameInstance.h"
#include "GameObject.h"

_int CWorldOctree::iStaticOctreeIndex = 0;

CWorldOctree::CWorldOctree()
{
}

HRESULT CWorldOctree::Initialize(_Vec3 _vMinPos, _Vec3 _vMaxPos, vector<class CGameObject*>& m_pParentObjList, _int* iDepth)
{
	//������ü �ٿ�� �ڽ� ����
	_Vec3 vCenter = { (_vMinPos.x + _vMaxPos.x) * 0.5f ,(_vMinPos.y + _vMaxPos.y) * 0.5f ,(_vMinPos.z + _vMaxPos.z) * 0.5f };

	//���� �� ���� �������� �ڽ� ����
	_Vec3 vExtend = { abs(_vMinPos.x - _vMaxPos.x),abs(_vMinPos.y - _vMaxPos.y),abs(_vMinPos.z - _vMaxPos.z) };
	_float fLongestExtend = max(max(vExtend.x, vExtend.y), vExtend.z);
	_float fHalfExtend = fLongestExtend * 0.5f;

	m_pBoundBox = new BoundingBox(vCenter, _Vec3(fHalfExtend, fHalfExtend, fHalfExtend));
	m_fRadius = fHalfExtend *sqrt(2.0f);

	if(*iDepth != -1)
		m_iDepth = *iDepth;

	if (*iDepth == -1 )	//ó�� ��� ������ ��
	{
		m_iDepth = 3;
	}

	//��Ʈ�� ����
	Build_WorldOctree(m_pParentObjList);

	if (m_iDepth <= 0 || m_iObjectCount <= 20)	//�ڽ� ���� ����
	{
		m_bHaveChild = false;
		return 		S_OK;
	}
	else
		m_bHaveChild = true;

	//����
	m_pChildren[0] = CWorldOctree::Create(_Vec3(_vMinPos.x, _vMinPos.y + fHalfExtend, _vMinPos.z + fHalfExtend), _Vec3(_vMinPos.x + fHalfExtend, _vMaxPos.y, _vMaxPos.z), m_pObjectsList, &m_iDepth);
	m_pChildren[1] = CWorldOctree::Create(_Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y + fHalfExtend, _vMinPos.z + fHalfExtend), _Vec3(_vMaxPos.x, _vMaxPos.y, _vMaxPos.z), m_pObjectsList, &m_iDepth);
	m_pChildren[2] = CWorldOctree::Create(_Vec3(_vMinPos.x, _vMinPos.y + fHalfExtend, _vMinPos.z), _Vec3(_vMinPos.x + fHalfExtend, _vMaxPos.y, _vMinPos.z + fHalfExtend), m_pObjectsList, &m_iDepth);
	m_pChildren[3] = CWorldOctree::Create(_Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y + fHalfExtend, _vMinPos.z), _Vec3(_vMaxPos.x, _vMaxPos.y, _vMinPos.z + fHalfExtend), m_pObjectsList, &m_iDepth);
	//�Ʒ���
	m_pChildren[4] = CWorldOctree::Create(_Vec3(_vMinPos.x, _vMinPos.y, _vMinPos.z + fHalfExtend), _Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y + fHalfExtend, _vMaxPos.z), m_pObjectsList, &m_iDepth);
	m_pChildren[5] = CWorldOctree::Create(_Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y, _vMinPos.z + fHalfExtend), _Vec3(_vMaxPos.x, _vMinPos.y + fHalfExtend, _vMaxPos.z), m_pObjectsList, &m_iDepth);
	m_pChildren[6] = CWorldOctree::Create(_Vec3(_vMinPos.x, _vMinPos.y, _vMinPos.z), _Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y + fHalfExtend, _vMinPos.z + fHalfExtend), m_pObjectsList, &m_iDepth);
	m_pChildren[7] = CWorldOctree::Create(_Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y, _vMinPos.z), _Vec3(_vMaxPos.x, _vMinPos.y + fHalfExtend, _vMinPos.z + fHalfExtend), m_pObjectsList, &m_iDepth);

	return 		S_OK;
}

void CWorldOctree::Culling(CGameInstance* pGameInstance, unordered_set<_int>& pFrustumCulledNodes)
{
	//����ü�� �ɸ����� Ȯ��
	if (!pGameInstance->isIn_Frustum_WorldSpace(XMLoadFloat3(&m_pBoundBox->Center), m_fRadius ))
		return;

	_Vec3 vCorners[8];
	m_pBoundBox->GetCorners(vCorners);

	//�ٿ�� �ڽ��� ����ü�� �󸶳� ������ Ȯ��
	_bool isIn[8];
	_bool anyIn = false;
	_bool allIn = true;

	_float fZero = 0.f;
	for (int i = 0; i < 8; ++i) {
		isIn[i] = pGameInstance->isIn_Frustum_WorldSpace(vCorners[i], m_fRadius);
		if (isIn[i])
		{
			anyIn = true;
		}
		else
		{
			allIn = false;
		}
	}

	if (allIn) {
		// ��� ���Ե� ���
		pFrustumCulledNodes.insert(m_iOctreeIndex);
	}
	else if (anyIn ) 	// �Ϻ� ���Ե� ���
	{
		for (int i = 0; i < 8; i++)
		{
			if (m_bHaveChild)
				m_pChildren[i]->Culling(pGameInstance, pFrustumCulledNodes);
			else
				pFrustumCulledNodes.insert(m_iOctreeIndex);
		}
	}

	return;
}

void CWorldOctree::Build_WorldOctree(vector<class CGameObject*>& m_pParentObjList)
{
	m_iDepth--;
	m_iOctreeIndex = iStaticOctreeIndex;
	iStaticOctreeIndex++;

	for (size_t i = 0; i < m_pParentObjList.size(); ++i)
	{
		CModel* pModel = static_cast<CModel*>(m_pParentObjList[i]->Find_Component(MODEL));
		if (pModel == nullptr)
			continue;

		BoundingBox ObjAABB = CalculateObjAABB(pModel);

		//�ﰢ���� �ٿ�� �ڽ��� �浹�ϸ� �� ��忡 ��´�.
		if (IsIntersactWithBoundingBox(ObjAABB)) {
			m_pObjectsList.push_back(m_pParentObjList[i]);
			m_pParentObjList[i]->Add_WorldOctreeIndex(m_iOctreeIndex);				//��ü�� �ε��� �ֱ�
			m_iObjectCount++;
		}
	}
}

BoundingBox CWorldOctree::CalculateObjAABB(CModel* pModel)
{
	_Vec3 MinPos = pModel->Get_Model_MinSize();
	_Vec3 MaxPos = pModel->Get_Model_MaxSize();

	//�ٿ�� �ڽ� ����
	_Vec3 vCenter = { (MinPos.x + MaxPos.x) * 0.5f ,(MinPos.y + MaxPos.y) * 0.5f ,(MinPos.z + MaxPos.z) * 0.5f };
	_Vec3 vExtend = { abs(MinPos.x - MaxPos.x),abs(MinPos.y - MaxPos.y),abs(MinPos.z - MaxPos.z) };

	return BoundingBox(vCenter, vExtend * 0.5f);
}

_bool CWorldOctree::IsIntersactWithBoundingBox(BoundingBox ObjBoundingBox)
{
	return m_pBoundBox->Intersects(ObjBoundingBox);
}


CWorldOctree* CWorldOctree::Create(_Vec3 _vMinPos, _Vec3 _vMaxPos, vector<class CGameObject*>& m_pParentObjList,  _int* iDepth)
{
	CWorldOctree* pInstance = new CWorldOctree();

	if (FAILED(pInstance->Initialize(_vMinPos, _vMaxPos, m_pParentObjList, iDepth)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CWorldOctree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWorldOctree::Free()
{
	__super::Free();

	if (m_pBoundBox)
		delete m_pBoundBox;
	m_pBoundBox = nullptr;

	for (int i = 0; i < 8; ++i) {
		Safe_Release(m_pChildren[i]);
	}
}
