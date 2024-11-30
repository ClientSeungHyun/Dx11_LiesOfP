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
	m_pBoundBox = new BoundingBox(vCenter, _Vec3(fLongestExtend, fLongestExtend, fLongestExtend));
	m_fRadius = fLongestExtend*0.5f*sqrt(2.0f);

	if(*iDepth != -1)
		m_iDepth = *iDepth;

	if (m_iDepth == -1)	//ó�� ��� ������ ��
	{
		m_iDepth = 4;
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

	_float fHalfExtend = fLongestExtend * 0.5f;

	//����
	m_pChildren[0] = CWorldOctree::Create(_Vec3(_vMinPos.x, _vMinPos.y + fHalfExtend, _vMinPos.z + fHalfExtend), _Vec3(_vMinPos.x + fHalfExtend, _vMaxPos.y, _vMaxPos.z), m_pOpjectsList, &m_iDepth);
	m_pChildren[1] = CWorldOctree::Create(_Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y + fHalfExtend, _vMinPos.z + fHalfExtend), _Vec3(_vMaxPos.x, _vMaxPos.y, _vMaxPos.z), m_pOpjectsList, &m_iDepth);
	m_pChildren[2] = CWorldOctree::Create(_Vec3(_vMinPos.x, _vMinPos.y + fHalfExtend, _vMinPos.z), _Vec3(_vMinPos.x + fHalfExtend, _vMaxPos.y, _vMinPos.z + fHalfExtend), m_pOpjectsList, &m_iDepth);
	m_pChildren[3] = CWorldOctree::Create(_Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y + fHalfExtend, _vMinPos.z), _Vec3(_vMaxPos.x, _vMaxPos.y, _vMinPos.z + fHalfExtend), m_pOpjectsList, &m_iDepth);
	//�Ʒ���
	m_pChildren[4] = CWorldOctree::Create(_Vec3(_vMinPos.x, _vMinPos.y, _vMinPos.z + fHalfExtend), _Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y + fHalfExtend, _vMaxPos.z), m_pOpjectsList, &m_iDepth);
	m_pChildren[5] = CWorldOctree::Create(_Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y, _vMinPos.z + fHalfExtend), _Vec3(_vMaxPos.x, _vMinPos.y + fHalfExtend, _vMaxPos.z), m_pOpjectsList, &m_iDepth);
	m_pChildren[6] = CWorldOctree::Create(_Vec3(_vMinPos.x, _vMinPos.y, _vMinPos.z), _Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y + fHalfExtend, _vMinPos.z + fHalfExtend), m_pOpjectsList, &m_iDepth);
	m_pChildren[7] = CWorldOctree::Create(_Vec3(_vMinPos.x + fHalfExtend, _vMinPos.y, _vMinPos.z), _Vec3(_vMaxPos.x, _vMinPos.y + fHalfExtend, _vMinPos.z + fHalfExtend), m_pOpjectsList, &m_iDepth);

	return 		S_OK;
}

void CWorldOctree::Build_WorldOctree(vector<class CGameObject*>& m_pParentObjList)
{
	m_iDepth--;
	m_iOctreeIndex = iStaticOctreeIndex;
	iStaticOctreeIndex++;

	for (size_t i = 0; i < m_pParentObjList.size(); ++i)
	{
		BoundingBox* ObjAABB = {nullptr};
		CalculateObjAABB(m_pParentObjList[i], ObjAABB);

		if (ObjAABB == nullptr)
			continue;

		//�ﰢ���� �ٿ�� �ڽ��� �浹�ϸ� �� ��忡 ��´�.
		if (IsIntersactWithBoundingBox(*ObjAABB)) {
			m_pOpjectsList.push_back(m_pParentObjList[i]);
			m_pParentObjList[i]->Add_WorldOctreeIndex(m_iOctreeIndex);				//��ü�� �ε��� �ֱ�
			m_iObjectCount++;
		}
	}
}

void CWorldOctree::CalculateObjAABB(class CGameObject* pObj, BoundingBox* pBoundingBox)
{
	CModel* pModel = static_cast<CModel*>(pObj->Find_Component(MODEL));

	if (pModel == nullptr)
		return;

	_Vec3 MinPos = pModel->Get_Model_MinSize();
	_Vec3 MaxPos = pModel->Get_Model_MaxSize();

	//�ٿ�� �ڽ� ����
	_Vec3 vCenter = { (MinPos.x + MaxPos.x) * 0.5f ,(MinPos.y + MaxPos.y) * 0.5f ,(MinPos.z + MaxPos.z) * 0.5f };
	_Vec3 vExtend = { abs(MinPos.x - MaxPos.x),abs(MinPos.y - MaxPos.y),abs(MinPos.z - MaxPos.z) };

	*pBoundingBox = BoundingBox(vCenter, vExtend);
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
