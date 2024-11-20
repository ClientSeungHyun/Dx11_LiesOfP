#include "stdafx.h"

#include "PlayerCamera.h"
#include "GameInstance.h"

#include "Player.h"

CPlayerCamera::CPlayerCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{
}

CPlayerCamera::CPlayerCamera(const CCamera& Prototype)
	:CCamera{ Prototype }
{
}

HRESULT CPlayerCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerCamera::Initialize(void* pArg)
{
	CAMERA_PLAYER_DESC* pDesc = static_cast<CAMERA_PLAYER_DESC*>(pArg);
	m_pPlayer = pDesc->pPlayer;
	m_fSpeed = pDesc->fSpeed;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_Vec3 vInitPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE::STATE_POSITION) + _Vec4(0.0f, 5.0f, -3.0f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vInitPos);

	m_vOffset = _Vec3(0.f, 1.6f, 0.f);

	m_fRadian = m_vOffset.Dot(_Vec3(0.f, 1.f, 0.f));
	m_fRadian = acosf(m_fRadian);

	return S_OK;
}

void CPlayerCamera::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CPlayerCamera::Update(_float fTimeDelta)
{
	if (!m_isActive)
		return;


	Control_Player(fTimeDelta);

}

void CPlayerCamera::Late_Update(_float fTimeDelta)
{
}

HRESULT CPlayerCamera::Render()
{
	return S_OK;
}

void CPlayerCamera::Control_Player(_float fTimeDelta)
{	
	// ȸ���� �÷��̾� ������
	_vector		vPlayerPosition = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION) + m_vOffset;

	_matrix		WorldMatrix = m_pTransformCom->Get_WorldMatrix();

	/* �÷��̾� ��ġ�� �̵��� ī�޶��� ���� ��� */
	_matrix RotationMatrix = m_pTransformCom->Get_WorldMatrix();
	RotationMatrix = XMMatrixTranslationFromVector(vPlayerPosition);

	// �÷��̾� ��ġ�� ��ȯ�� ����� ���÷� ����
	_matrix		LocalMatrix = WorldMatrix * XMMatrixInverse(nullptr, RotationMatrix);

#pragma region ī�޶� ȸ��

	POINT		ptMouse = m_pGameInstance->Get_MosePos();

	_long	MouseMove = { 0 };

	// �� ��
	if (MouseMove = ptMouse.x - m_ptOldMousePos.x)
	{
		if(abs(MouseMove) > 1)
			m_fCurrentRotationX = MouseMove * 0.002f;

		if (fabs(m_fCurrentRotationX) > 0.03f)
			m_fCurrentRotationX = (m_fCurrentRotationX / fabs(m_fCurrentRotationX)) * 0.05f;
	}
	else
	{
		m_fCurrentRotationX = 0.0f;
	}

	// �� ��
	if (MouseMove = ptMouse.y - m_ptOldMousePos.y)
	{
		if (abs(MouseMove) > 1)
			m_fCurrentRotationY = MouseMove * 0.002f;

		if (fabs(m_fCurrentRotationY) > 0.03f)
			m_fCurrentRotationY = (m_fCurrentRotationY / fabs(m_fCurrentRotationY)) * 0.05f;
	}
	else if (MouseMove == 0)
	{
		m_fCurrentRotationY = 0.0f;
	}

	// ȸ���� ������
	if (m_fCurrentRotationX)
	{
		LocalMatrix = LocalMatrix * XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.f, 0.f), m_fCurrentRotationX));
	}

	if (m_fCurrentRotationY)
	{
		LocalMatrix = LocalMatrix * XMMatrixRotationQuaternion(XMQuaternionRotationAxis(LocalMatrix.r[0], m_fCurrentRotationY));
	}

#pragma endregion

	// ȸ�� ���¸� �����ص� ���� �����
	WorldMatrix = LocalMatrix * RotationMatrix;

	_vector vOffset = WorldMatrix.r[3] - vPlayerPosition;
	vOffset = XMVector3Normalize(vOffset);

	_vector		vPlayerUp = XMVector3Normalize(WorldMatrix.r[1]);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	//XMVector3Normalize(m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_UP));

// ���� ī�޶� ����
	if (m_fCurrentRotationY)
		m_fRadian = acosf(XMVectorGetX(XMVector3Dot(vUp, vOffset)));

	_float fAngle = XMConvertToDegrees(m_fRadian);

	if (fAngle < 30.0f)
	{
		m_fRadian = XMConvertToRadians(30.0f);
		m_fCurrentRotationY = 0.0f;
	}
	if (fAngle > 97.f)
	{
		m_fRadian = XMConvertToRadians(97.f);
		m_fCurrentRotationY = 0.0f;
	}

#pragma region �÷��̾� ���󰡱�


	_vector vRight = WorldMatrix.r[3] - vPlayerPosition;			// ���� ����
	vRight = XMVector3Normalize(XMVector3Cross(vRight, vUp));		// �������� right ���� ���ϱ�

	// ī�޶��� ��ǥ ��ġ�� ������
	_vector vTargetPosition = XMVector3Rotate(vUp, XMQuaternionRotationAxis(-vRight, m_fRadian));
	vTargetPosition = vPlayerPosition + vTargetPosition * 3.f;

	_vector vTargetLook = -vOffset;
	vTargetLook = XMVector3Normalize(vTargetLook);

	_vector vCameraPosition = WorldMatrix.r[3];
	_vector vCameraLook = XMVector3Normalize(WorldMatrix.r[2]);

	_float fLerpSpeed = 8.f * fTimeDelta;

	_vector vFinalPosition = XMVectorLerp(vCameraPosition, vTargetPosition, fLerpSpeed);
	_vector vFinalLook = XMVectorLerp(vCameraLook, vTargetLook, fLerpSpeed);
	vFinalLook = XMVector3Normalize(vFinalLook);

	XMVECTOR vFinalRight = XMVector3Cross(vUp, vFinalLook);
	vFinalRight = XMVector3Normalize(vFinalRight);

	XMVECTOR vFinalUp = XMVector3Cross(vFinalLook, vFinalRight);
	vFinalUp = XMVector3Normalize(vFinalUp);

	WorldMatrix.r[0] = vFinalRight;
	WorldMatrix.r[1] = vFinalUp;
	WorldMatrix.r[2] = vFinalLook;
	WorldMatrix.r[3] = vFinalPosition;
#pragma endregion

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);

	m_ptOldMousePos = ptMouse;

//	 //ȸ���� �÷��̾� ������
//	_Vec4		vPlayerPosition = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);
//
//	_Matrix		WorldMatrix = m_pTransformCom->Get_WorldMatrix();
//
//	/* �÷��̾� ��ġ�� �̵��� ī�޶��� ���� ��� */
//	_Matrix RotationMatrix = m_pTransformCom->Get_WorldMatrix();
//	RotationMatrix = XMMatrixTranslationFromVector(vPlayerPosition);
//
//	// �÷��̾� ��ġ�� ��ȯ�� ����� ���÷� ����
//	_Matrix		LocalMatrix = WorldMatrix * XMMatrixInverse(nullptr, RotationMatrix);
//
//#pragma region ī�޶� ȸ��
//
//	POINT		ptMouse = m_pGameInstance->Get_MosePos();
//
//	_long	MouseMove = { 0 };
//
//	// �� ��
//	if (MouseMove = ptMouse.x - m_ptOldMousePos.x)
//	{
//		if(abs(MouseMove) > 1)
//			m_fCurrentRotationX = MouseMove * 0.002f;
//
//		if (fabs(m_fCurrentRotationX) > 0.03f)
//			m_fCurrentRotationX = (m_fCurrentRotationX / fabs(m_fCurrentRotationX)) * 0.05f;
//	}
//	else
//	{
//		m_fCurrentRotationX = 0.0f;
//	}
//
//	// �� ��
//	if (MouseMove = ptMouse.y - m_ptOldMousePos.y)
//	{
//		if (abs(MouseMove) > 1)
//			m_fCurrentRotationY = MouseMove * 0.002f;
//
//		if (fabs(m_fCurrentRotationY) > 0.03f)
//			m_fCurrentRotationY = (m_fCurrentRotationY / fabs(m_fCurrentRotationY)) * 0.05f;
//	}
//	else if (MouseMove == 0)
//	{
//		m_fCurrentRotationY = 0.0f;
//	}
//
//	// ȸ���� ������
//	if (m_fCurrentRotationX)
//	{
//		LocalMatrix = LocalMatrix * XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.f, 0.f), m_fCurrentRotationX));
//	}
//
//	if (m_fCurrentRotationY)
//	{
//		LocalMatrix = LocalMatrix * XMMatrixRotationQuaternion(XMQuaternionRotationAxis(LocalMatrix.Right(), m_fCurrentRotationY));
//	}
//
//	WorldMatrix = LocalMatrix * RotationMatrix;
//#pragma endregion
//
//
//	_Vec3		vOffset = WorldMatrix.Translation() - vPlayerPosition;
//	vOffset.Normalize();
//	_Vec3		vUp = _Vec3(0.f, 1.f, 0.f);
//
//	if (m_fCurrentRotationY)
//		m_fRadian = acosf(vOffset.Dot(vUp));
//
//	_float fAngle = XMConvertToDegrees(m_fRadian);
//
//	if (fAngle < 30.0f)
//	{
//		m_fRadian = XMConvertToRadians(30.0f);
//		m_fCurrentRotationY = 0.0f;
//	}
//	if (fAngle > 97.f)
//	{
//		m_fRadian = XMConvertToRadians(97.f);
//		m_fCurrentRotationY = 0.0f;
//	}
//
//	//_Vec3 vRight = WorldMatrix.Translation() - vPlayerPosition;			// ���� ����
//	//vRight = XMVector3Normalize(XMVector3Cross(vRight, vUp));		// �������� right ���� ���ϱ�
//
//	//// ī�޶��� ��ǥ ��ġ�� ������
//	//_Vec3 vTargetPosition = XMVector3Rotate(vUp, XMQuaternionRotationAxis(-vRight, m_fRadian));
//	//vTargetPosition = vPlayerPosition + vTargetPosition * 5.f;
//
//	//_Vec3 vTargetLook = -vOffset;
//
//	//_Vec3 vCameraPosition = WorldMatrix.Translation();
//	//_Vec3 vCameraLook = XMVector3Normalize(WorldMatrix.Forward());
//
//	//_float fLerpSpeed = 1.f * fTimeDelta;
//
//	//_Vec4 vFinalPosition = XMVectorLerp(vCameraPosition, vTargetPosition, fLerpSpeed);
//	//vFinalPosition.w = 1.f;
//	//_Vec4 vFinalLook = XMVectorLerp(vCameraLook, vTargetLook, fLerpSpeed);
//	//vFinalLook = XMVector3Normalize(vFinalLook);
//
//	//_Vec4 vFinalRight = XMVector3Cross(vUp, vFinalLook);
//	//vFinalRight = XMVector3Normalize(vFinalRight);
//
//	//_Vec4 vFinalUp = XMVector3Cross(vFinalLook, vFinalRight);
//	//vFinalUp = XMVector3Normalize(vFinalUp);
//
//	//_matrix FinalWorldMatrix;
//	//FinalWorldMatrix.r[0] = vFinalRight;
//	//FinalWorldMatrix.r[1] = vFinalUp;
//	//FinalWorldMatrix.r[2] = vFinalLook;
//	//FinalWorldMatrix.r[3] = vFinalPosition;
//
//	//m_pTransformCom->Set_WorldMatrix(FinalWorldMatrix);
//	// 
//
//	_vector vRight = WorldMatrix.Translation() - vPlayerPosition;			// ���� ����
//	vRight = XMVector3Normalize(XMVector3Cross(vRight, vUp));		// �������� right ���� ���ϱ�
//
//	// ī�޶��� ��ǥ ��ġ�� ������
//	_vector vTargetPosition = XMVector3Rotate(vUp, XMQuaternionRotationAxis(-vRight, m_fRadian));
//	vTargetPosition = vPlayerPosition + vTargetPosition * 5.f;
//
//	_vector vTargetLook = -vOffset;
//	vTargetLook = XMVector3Normalize(vTargetLook);
//
//	_vector vCameraPosition = WorldMatrix.Translation();
//	_vector vCameraLook = XMVector3Normalize(WorldMatrix.Forward());
//
//	_float fLerpSpeed = 8.f * fTimeDelta;
//
//	_vector vFinalPosition = XMVectorLerp(vCameraPosition, vTargetPosition, fLerpSpeed);
//	_vector vFinalLook = XMVectorLerp(vCameraLook, vTargetLook, fLerpSpeed);
//	vFinalLook = XMVector3Normalize(vFinalLook);
//
//	XMVECTOR vFinalRight = XMVector3Cross(vUp, vFinalLook);
//	vFinalRight = XMVector3Normalize(vFinalRight);
//
//	XMVECTOR vFinalUp = XMVector3Cross(vFinalLook, vFinalRight);
//	vFinalUp = XMVector3Normalize(vFinalUp);
//
//	WorldMatrix.r[0] = vFinalRight;
//	WorldMatrix.r[1] = vFinalUp;
//	WorldMatrix.r[2] = vFinalLook;
//	WorldMatrix.r[3] = vFinalPosition;
//
//	//m_pTransformCom->Set_WorldMatrix(WorldMatrix);
//	m_ptOldMousePos = ptMouse;
}

void CPlayerCamera::Control_Camera(_float fTimeDelta)
{
}

void CPlayerCamera::Setting_CameraControl(_float3 vStartPos, _float3 vEndPos, _float3 vCameraAt, _float fFovy, _bool isKeepAt)
{
}

void CPlayerCamera::Setting_CameraControl(_fvector vStartPos, _fvector vEndPos, _fvector vCameraAt, _float fFovy, _bool isKeepAt)
{
}

void CPlayerCamera::Start_CameraControl()
{
	m_isControl = true;
}

void CPlayerCamera::Start_CameraLerp(_float fLerpDuration)
{
	m_isLerp = true;
	m_isLerpEnd = false;
}

void CPlayerCamera::End_CameraControl()
{
	m_isControl = false;
	m_isLerp = false;
	m_isLerpEnd = false;
}

HRESULT CPlayerCamera::Ready_Components()
{
	return S_OK;
}

CPlayerCamera* CPlayerCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerCamera* pInstance = new CPlayerCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject* CPlayerCamera::Clone(void* pArg)
{
	CPlayerCamera* pInstance = new CPlayerCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerCamera::Free()
{
	__super::Free();
}
