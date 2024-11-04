#include "stdafx.h"
#include "NavigationController.h"

#include "GameInstance.h"

IMPLEMENT_SINGLETON(CNavigationController)

CNavigationController::CNavigationController()
	:m_pGameInstance{ CGameInstance::Get_Instance() }
{
}

void CNavigationController::Change_Cell_Inform(_uint _index, _uint _iType, _uint _iRoomNum)
{
	if (m_Cells[_index] == nullptr)
		return;

//	m_Cells[_index]->Set_RoomNum(_iRoomNum);
}

_bool CNavigationController::isCell_Picked(_float3 vPickPos)
{
	//if (Get_CellSize() == 0) //Cell�� ���� ���
	//{
	//	m_iSelectCellIndex = -1;
	//	return false;
	//}

	//for (_uint i = 0; i < m_Cells.size(); ++i)
	//{
	//	CCell* pCell = m_Cells[i];
	//	if (nullptr == pCell)
	//		continue;

	//	_int			iNeighborIndex = { -1 };

	//	if (true == m_Cells[i]->isIn(XMLoadFloat3(&vPickPos), &iNeighborIndex))
	//	{
	//		return true;
	//	}
	//}
	//m_iSelectCellIndex = -1;
	return false;
}

_int CNavigationController::Get_WhereCell(_float3 vPickPos)
{
	//if (Get_CellSize() == 0) //Cell�� ���� ���
	//{
	//	return -1;
	//}

	//for (_uint i = 0; i < m_Cells.size(); ++i)
	//{
	//	CCell* pCell = m_Cells[i];
	//	if (nullptr == pCell)
	//		continue;

	//	_int			iNeighborIndex = { -1 };

	//	if (true == m_Cells[i]->isIn(XMLoadFloat3(&vPickPos), &iNeighborIndex))
	//	{
	//		return i;
	//	}
	//}
	return -1;
}

void CNavigationController::Delete_All_Cell()
{
	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();
}

HRESULT CNavigationController::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigationController::Render()
{
	/*if (m_Cells.size() == 0)
		return S_OK;

	_float4x4			WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());
	WorldMatrix._42 += 0.1f;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float4		vColor = _float4(0.f, 1.f, 0.f, 1.f);

	int i = 0;

#ifdef _DEBUG

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
		{
			if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
				return E_FAIL;

			if (i == m_iSelectCellIndex)
				vColor = _float4(1.f, 0.f, 0.f, 1.f);
			else
				vColor = _float4(0.f, 1.f, 0.f, 1.f);

			m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

			m_pShader->Begin(0);
			pCell->Render();
		}
		i++;
	}*/


//#endif // _DEBUG
	return S_OK;
}

void CNavigationController::Add_Point(_fvector vPos)
{
	//_float3 vClickPos;

	//// ���� ���� �Ÿ��� ���� ������ �� �� ��ġ�� �ٲ�
	//vClickPos = Find_ClosetPoint(vPos);

	//// �Ÿ��� �ָ� �׳� �״�� ����
	//if (m_fClosetDistance > 0.5f)
	//	XMStoreFloat3(&vClickPos, vPos);

	//if (2 < m_vClickPoints.size())
	//	return;

	////if (0 == m_vClickedPoints.size())
	////	Add_ClickedSymbol(vClickPosition, SYMBOL1);
	////else if (1 == m_vClickedPoints.size())
	////	Add_ClickedSymbol(vClickPosition, SYMBOL2);

	//m_vClickPoints.push_back(vClickPos);
	//m_vClickPos = vClickPos;

	//if (3 == m_vClickPoints.size())
	//{
	//	_float3 vPoints[3];

	//	for (_uint i = 0; i < 3; ++i)
	//		vPoints[i] = m_vClickPoints[i];

	//	Add_Cell(vPoints);

	//	m_vClickPoints.clear();
	//}
}

HRESULT CNavigationController::Add_Cell(_float3* vPoints)
{
//	Sort_Cell(vPoints);
//
//	CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_int)m_Cells.size());
//	if (nullptr == pCell)
//		return E_FAIL;
//
////	pCell->Set_RoomNum(m_iRoomNum);
//
//	m_Cells.push_back(pCell);

	return S_OK;
}

void CNavigationController::Sort_Cell(_float3* vPoints)
{
	//_float3 vCurrenPoints[3];
	//memcpy(vCurrenPoints, vPoints, sizeof(_float3) * 3);

	//// ���� ���� x ã��
	//// ���� ���� x�� 0 �߰��� 1 ū ���� 2�� ���ĵ�
	//for (_uint i = 0; i < 2; ++i)
	//{
	//	_uint iIndex_MinX = i;
	//	_bool isSwap = false;
	//	for (_uint j = i + 1; j < 3; ++j)
	//	{
	//		if (vCurrenPoints[iIndex_MinX].x > vCurrenPoints[j].x)
	//		{
	//			iIndex_MinX = j;
	//			isSwap = true;
	//		}
	//	}

	//	if (isSwap)
	//	{
	//		_float3 vTemp;
	//		vTemp = vCurrenPoints[i];
	//		vCurrenPoints[i] = vCurrenPoints[iIndex_MinX];
	//		vCurrenPoints[iIndex_MinX] = vTemp;
	//	}
	//}

	//// ���� ���� x�� z���� �߰� x�� z������ ũ�� �װ� 2��°�� �ǰ�, ������ 3��°�� ��
	////x�� ���� �߰� ���� �׻� 0��°(Cell �� ����� ������)�� ��
	//vPoints[0] = vCurrenPoints[1];
	//if (vCurrenPoints[0].z < vCurrenPoints[1].z)
	//{
	//	vPoints[1] = vCurrenPoints[2];
	//	vPoints[2] = vCurrenPoints[0];
	//}
	//else
	//{
	//	vPoints[1] = vCurrenPoints[0];
	//	vPoints[2] = vCurrenPoints[2];
	//}
}

CCell* CNavigationController::Get_SelectCell()
{
	if (m_Cells.size() == 0 || m_iSelectCellIndex >= m_Cells.size() || m_iSelectCellIndex == -1)
		return nullptr;

	return m_Cells[m_iSelectCellIndex];
}

_float3 CNavigationController::Find_ClosetPoint(_fvector vPos)
{
	_float3 vClilckPos = {};
	//XMStoreFloat3(&vClilckPos, vPos);

	//m_fClosetDistance = 100000.f;

	//if (m_Cells.size() == 0)
	//	return vClilckPos;

	//XMVECTOR vRayPos_Local = m_pGameInstance->Get_RayPos_Local();
	//XMVECTOR vRayDir_Local = XMVector3Normalize(m_pGameInstance->Get_RayDir_Local());

	//for (_uint i = 0; i < m_Cells.size(); ++i)
	//{
	//	CCell* pCell = m_Cells[i];
	//	if (nullptr == pCell)
	//		continue;

	//	_vector vPoint[CCell::POINT_END];

	//	//���� ���� ������ ������ ����
	//	vPoint[CCell::POINT_A] = pCell->Get_Point(CCell::POINT_A);
	//	vPoint[CCell::POINT_B] = pCell->Get_Point(CCell::POINT_B);
	//	vPoint[CCell::POINT_C] = pCell->Get_Point(CCell::POINT_C);

	//	//��ŷ�� ������ ��������� �Ÿ� ���, ���� ª�� �Ÿ� ����
	//	_float fDistance_A = XMVectorGetX(XMVector3Length(vPoint[CCell::POINT_A] - vPos));
	//	_float fDistance_B = XMVectorGetX(XMVector3Length(vPoint[CCell::POINT_B] - vPos));
	//	_float fDistance_C = XMVectorGetX(XMVector3Length(vPoint[CCell::POINT_C] - vPos));
	//	_float fClosetDistance = min(fDistance_A, fDistance_B);
	//	fClosetDistance = min(fClosetDistance, fDistance_C);

	//	//���� ������ ��ȸ�� ������ �Ÿ��� ũ�ٸ� continue,
	//	//�� �۴ٸ� �ش� ���� click �� ������ Store���ش�.
	//	if (m_fClosetDistance < fClosetDistance)
	//		continue;

	//	m_fClosetDistance = fClosetDistance;
	//	if (fDistance_A == m_fClosetDistance)
	//		XMStoreFloat3(&vClilckPos, vPoint[CCell::POINT_A]);
	//	else if (fDistance_B == m_fClosetDistance)
	//		XMStoreFloat3(&vClilckPos, vPoint[CCell::POINT_B]);
	//	else if (fDistance_C == m_fClosetDistance)
	//		XMStoreFloat3(&vClilckPos, vPoint[CCell::POINT_C]);
	//}

	return vClilckPos;

}

_int CNavigationController::SelectCell(_float3 vPickPos)
{
	//if (Get_CellSize() == 0) //Cell�� ���� ���
	//{
	//	m_iSelectCellIndex = -1;
	//	return -1;
	//}

	//for (_uint i = 0; i < m_Cells.size(); ++i)
	//{
	//	CCell* pCell = m_Cells[i];
	//	if (nullptr == pCell)
	//		continue;

	//	_int			iNeighborIndex = { -1 };

	//	if (true == m_Cells[i]->isIn(XMLoadFloat3(&vPickPos), &iNeighborIndex))
	//	{
	//		m_iSelectCellIndex = i;
	//		return i;
	//	}
	//}
	//m_iSelectCellIndex = -1;
	return -1;
}

void CNavigationController::Delete_Selected(_uint iIndex)
{
	/*if (m_Cells[iIndex] != nullptr && iIndex < m_Cells.size())
		m_Cells.erase(remove(m_Cells.begin(), m_Cells.end(), m_Cells[iIndex]), m_Cells.end());*/
	//remove�� erase�� ������ �����ϱ� ���� �� �� ���
}

_float3 CNavigationController::Select_Vertex(_fvector vPos)
{
	//���� ����� ������ vPos�� �̵���Ų��.
	_float3 selectVertex = Find_ClosetPoint(vPos);

	m_vSelectVertexPos = selectVertex;

	return selectVertex;
}

void CNavigationController::Set_All_Selected_Vertex_to_this(_float3 vChangePos)
{
	/*for (auto& pCell : m_Cells)
	{
		pCell->CompareAndChange(m_vSelectVertexPos, vChangePos);

		Sort_Cell(pCell->Get_All_Points());
	}*/
}

void CNavigationController::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);
}
