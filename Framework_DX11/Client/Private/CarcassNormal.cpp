#include "stdafx.h"
#include "CarcassNormal.h"

#include "GameInstance.h"
#include "Player.h"
#include "Fsm.h"

// 24-11-26 김성용
// 게임인터페이스 접근 코드 
// 정식 코드  
#include "GameInterface_Controller.h"


#include "State_CarcassNormal_Idle.h"
#include "State_CarcassNormal_Die.h"
#include "State_CarcassNormal_HitFatal.h"
#include "State_CarcassNormal_KnockBack.h"

#include "State_CarcassNormal_HeadingMultiple.h"
#include "State_CarcassNormal_TripleClaw.h"
#include "State_CarcassNormal_TripleClaw_2.h"

#include "State_CarcassNormal_ClawMultiple.h"
#include "State_CarcassNormal_Bite.h"
#include "State_CarcassNormal_ClawRush.h"

#include "Dissolve_Effect.h"


CCarcassNormal::CCarcassNormal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CCarcassNormal::CCarcassNormal(const CCarcassNormal& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CCarcassNormal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCarcassNormal::Initialize(void* pArg)
{
	OBJECT_DEFAULT_DESC* pDefaultDesc = static_cast<OBJECT_DEFAULT_DESC*>(pArg);

	CGameObject::GAMEOBJECT_DESC		Desc{};
	Desc.fSpeedPerSec = 1.5f;
	Desc.fRotationPerSec = 30.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;
	
	m_eStat.fHp = 120.f;
	m_eStat.fMaxHp = 120.f;
	m_eStat.fAtk = 120.f;
	//m_eStat.fDefence = 3.f;

	m_eStat.bWeakness = false;

	m_eStat.fGrogyPoint = 0.f;
	m_eStat.fMaxGrogyPoint = 80.f;

	m_iErgoPoint = 60;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Weapon()))
		return E_FAIL;

	if (FAILED(Ready_Effect()))
		return E_FAIL;

	m_iOriginCellNum = pDefaultDesc->iCurrentCellNum;

	m_pNavigationCom->Move_to_Cell(m_pRigidBodyCom, m_iOriginCellNum);
	m_iInitRoomNum = m_pNavigationCom->Get_Cell_AreaNum(m_iOriginCellNum);

	m_pModelCom->SetUp_Animation(95, true);

	if (FAILED(Ready_FSM()))
		return E_FAIL;

	m_strObjectTag = TEXT("Monster");

	m_vRimLightColor = { 0.f, 0.f, 0.f, 0.f };

	m_vCenterOffset = _Vec3{ 0.f, 1.2f, 0.f };

	m_bHaveGrogy = false;
	m_bDiscover = false;

	GET_GAMEINTERFACE->Register_Pointer_Into_OrthoUIPage(UI_ORTHO_OBJ_TYPE::ORTHO_CARCASS_NORMAL, this);

	GET_GAMEINTERFACE->Set_OnOff_OrthoUI(false, this);
	return S_OK;
}

void CCarcassNormal::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	for (_uint i = 0; i < TYPE_END; ++i)
	{
		m_pColliderObject[i]->Priority_Update(fTimeDelta);
	}

	for (auto& Effect : m_SurfaceEffect)
		Effect->Priority_Update(fTimeDelta);

}

void CCarcassNormal::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_Player_AreaNum() == m_iInitRoomNum)
		m_vCurRootMove = XMVector3TransformNormal(m_pModelCom->Play_Animation(fTimeDelta), m_pTransformCom->Get_WorldMatrix());
	else
		m_vCurRootMove = _Vec3(0.f, 0.f, 0.f);

	if (m_bDebuffed[SURFACE_ELECTRIC])
	{
		m_vCurRootMove *= 0.8f;
	}

	m_pRigidBodyCom->Set_Velocity(m_vCurRootMove / fTimeDelta);

	m_pFsmCom->Update(fTimeDelta);

	Update_Collider();
	Update_Debuff(fTimeDelta);

	for (_uint i = 0; i < TYPE_END; ++i)
	{
		m_pColliderObject[i]->Update(fTimeDelta);
	}

	m_pGameInstance->Add_ColliderList(m_pColliderCom);

	for (auto& Effect : m_SurfaceEffect)
		Effect->Update(fTimeDelta);

}

void CCarcassNormal::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_Player_AreaNum() == m_iInitRoomNum)
	{
		if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 50.f))
		{
			__super::Late_Update(fTimeDelta);

			m_pRigidBodyCom->Update(fTimeDelta);

			for (auto& Effect : m_SurfaceEffect)
				Effect->Late_Update(fTimeDelta);

			m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

			for (_uint i = 0; i < TYPE_END; ++i)
			{
				m_pColliderObject[i]->Late_Update(fTimeDelta);
			}

			for (_int i = 0; i < CT_END - 1; ++i)
			{
				m_pGameInstance->Add_ColliderList(m_EXCollider[i]);
			}
			m_pGameInstance->Add_ColliderList(m_pColliderCom);
		}
	}
}

HRESULT CCarcassNormal::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
	for (_int i = 0; i < CT_END - 1; ++i)
	{
		m_EXCollider[i]->Render();
	}

	for (_uint i = 0; i < TYPE_END; ++i)
	{
		//[i]->Active_Collider();
		m_pColliderObject[i]->Render();
	}
#endif
	return S_OK;
}

void CCarcassNormal::Resetting()
{
	m_vRimLightColor = { 0.f, 0.f, 0.f, 0.f };

	m_eStat.fHp = 120.f;
	m_eStat.fMaxHp = 120.f;
	m_eStat.fAtk = 120.f;

	m_eStat.fGrogyPoint = 0.f;
	m_eStat.fMaxGrogyPoint = 80.f;

	m_eStat.bWeakness = false;
	m_eStat.bFatalAttack = false;

	m_bDieState = false;

	m_bDiscover = false;
	m_bFirstMeetCheck = false;

	m_bFatalAttacked = false;

	m_pNavigationCom->Move_to_Cell(m_pRigidBodyCom, m_iOriginCellNum);
	m_iInitRoomNum = m_pNavigationCom->Get_Cell_AreaNum(m_iOriginCellNum);

	Reset_Debuff();

	Change_State(CMonster::IDLE);

	GET_GAMEINTERFACE->Set_OnOff_OrthoUI(false, this);

	m_pRigidBodyCom->Add_Actor();
}

void CCarcassNormal::Active_CurrentWeaponCollider(_float fDamageRatio, _uint iCollIndex, HIT_TYPE eHitType, ATTACK_STRENGTH eAtkStrength)
{
	m_pColliderObject[iCollIndex]->Active_Collider(fDamageRatio, 0, eHitType, eAtkStrength);
}

void CCarcassNormal::DeActive_CurrentWeaponCollider(_uint iCollIndex)
{
	m_pColliderObject[iCollIndex]->DeActive_Collider();
}

void CCarcassNormal::Active_Debuff(_int iIndex, _float fDebuffRatio)
{
	m_pColliderObject[iIndex]->Active_Debuff(fDebuffRatio);
}

void CCarcassNormal::DeActive_Debuff(_int iIndex)
{
	m_pColliderObject[iIndex]->DeActive_Debuff();
}

HRESULT CCarcassNormal::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CarcassNormal"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* FOR.Com_Collider */		//Body
	CBounding_OBB::BOUNDING_OBB_DESC			ColliderDesc{};
	ColliderDesc.vExtents = _float3(0.4f, 0.3f, 0.4f);
	ColliderDesc.vCenter = _float3(-0.1f, 0.f, 0.f);
	ColliderDesc.vAngles = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;
	m_pColliderCom->Set_Owner(this);
	m_pColliderBindMatrix[CT_BODY_UPPER] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(8);


	//LowerBody
	ColliderDesc.vExtents = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vCenter = _float3(0.2f, 0.f, 0.f);
	ColliderDesc.vAngles = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_BodyLower"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_BODY_LOWER]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_BODY_LOWER] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(5);

	//Head
	ColliderDesc.vExtents = _float3(0.15f, 0.15f, 0.15f);
	ColliderDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vAngles = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_Head"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_HEAD]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_HEAD] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_HEAD));


	//UPPERArmLeft
	ColliderDesc.vExtents = _float3(0.3f, 0.1f, 0.1f);
	ColliderDesc.vCenter = _float3(0.15f, 0.f, 0.f);
	ColliderDesc.vAngles = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_UpperArmLeft"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_UPPERARM_LEFT]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_UPPERARM_LEFT] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_HAND_LEFT) - 2);

	//UPPERArmRight
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_UpperArmRight"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_UPPERARM_RIGHT]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_UPPERARM_RIGHT] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_HAND_RIGHT) - 2);


	//LOWERArmLeft
	ColliderDesc.vExtents = _float3(0.45f, 0.1f, 0.1f);
	ColliderDesc.vCenter = _float3(0.3f, 0.f, 0.f);
	ColliderDesc.vAngles = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_LowerArmLeft"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_LOWERARM_LEFT]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_LOWERARM_LEFT] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_HAND_LEFT) - 1);


	//LOWERArmRight
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_LowerArmRight"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_LOWERARM_RIGHT]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_LOWERARM_RIGHT] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_HAND_RIGHT) - 1);



	//UPPERLegLeft
	ColliderDesc.vExtents = _float3(0.4f, 0.15f, 0.15f);
	ColliderDesc.vCenter = _float3(0.1f, 0.f, 0.f);
	ColliderDesc.vAngles = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_UpperLegLeft"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_UPPERLEG_LEFT]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_UPPERLEG_LEFT] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_FOOT_LEFT) - 2);


	//UPPERLegRight
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_UpperLegRight"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_UPPERLEG_RIGHT]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_UPPERLEG_RIGHT] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_FOOT_RIGHT) - 2);


	//LOWERLegLeft
	ColliderDesc.vExtents = _float3(0.45f, 0.1f, 0.1f);
	ColliderDesc.vCenter = _float3(0.25f, 0.f, 0.f);
	ColliderDesc.vAngles = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_LowerLegLeft"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_LOWERLEG_LEFT]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_LOWERLEG_LEFT] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_FOOT_LEFT) - 1);


	//LOWERLegRight
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider_LowerLegRight"), reinterpret_cast<CComponent**>(&m_EXCollider[CT_LOWERLEG_RIGHT]), &ColliderDesc)))
		return E_FAIL;
	m_pColliderBindMatrix[CT_LOWERLEG_RIGHT] = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_FOOT_RIGHT) - 1);

	for (_int i = 0; i < CT_END - 1; ++i)
	{
		m_EXCollider[i]->Set_Owner(this);
	}


	// 항상 마지막에 생성하기
	CRigidBody::RIGIDBODY_DESC RigidBodyDesc{};
	RigidBodyDesc.isStatic = false;
	RigidBodyDesc.isGravity = false;
	RigidBodyDesc.pOwnerTransform = m_pTransformCom;
	RigidBodyDesc.pOwnerNavigation = m_pNavigationCom;
	RigidBodyDesc.isCapsule = true;

	RigidBodyDesc.pOwner = this;
	RigidBodyDesc.fStaticFriction = 1.0f;
	RigidBodyDesc.fDynamicFriction = 0.0f;
	RigidBodyDesc.fRestituion = 0.0f;

	MyPhysX::GeometryCapsule CapsuleDesc;
	CapsuleDesc.fHeight = 1.5f;
	CapsuleDesc.fRadius = 0.4f;
	RigidBodyDesc.pGeometryDesc = &CapsuleDesc;
	RigidBodyDesc.PxLockFlags = PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;

	/* FOR.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		TEXT("Com_RigidBody"), reinterpret_cast<CComponent**>(&m_pRigidBodyCom), &RigidBodyDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCarcassNormal::Ready_FSM()
{
	if (FAILED(__super::Ready_FSM()))
		return E_FAIL;

	FSM_INIT_DESC Desc{};



	m_pFsmCom->Add_State(CState_CarcassNormal_Idle::Create(m_pFsmCom, this, IDLE, &Desc));
	m_pFsmCom->Add_State(CState_CarcassNormal_HitFatal::Create(m_pFsmCom, this, HITFATAL, &Desc));
	m_pFsmCom->Add_State(CState_CarcassNormal_Die::Create(m_pFsmCom, this, DIE, &Desc));
	m_pFsmCom->Add_State(CState_CarcassNormal_KnockBack::Create(m_pFsmCom, this, KNOCKBACK, &Desc));


	m_pFsmCom->Add_State(CState_CarcassNormal_TripleClaw::Create(m_pFsmCom, this, TRIPLECLAW, &Desc));
	m_pFsmCom->Add_State(CState_CarcassNormal_ClawMultiple::Create(m_pFsmCom, this, CLAWMULTIPLE, &Desc));
	m_pFsmCom->Add_State(CState_CarcassNormal_ClawRush::Create(m_pFsmCom, this, CLAWRUSH, &Desc));
	m_pFsmCom->Add_State(CState_CarcassNormal_TripleClaw_2::Create(m_pFsmCom, this, TRIPLECLAW_2, &Desc));
	m_pFsmCom->Add_State(CState_CarcassNormal_Bite::Create(m_pFsmCom, this, BITE, &Desc));
	m_pFsmCom->Add_State(CState_CarcassNormal_HeadingMultiple::Create(m_pFsmCom, this, HEADINGMULTIPLE, &Desc));
	
	m_pFsmCom->Set_State(IDLE);

	return S_OK;



}

HRESULT CCarcassNormal::Ready_Weapon()
{
	/* FOR.Com_Collider_OBB */
	CBounding_OBB::BOUNDING_OBB_DESC			ColliderOBBDesc_Obj{};

	ColliderOBBDesc_Obj.vAngles = _float3(0.f, 0.f, 0.f);
	ColliderOBBDesc_Obj.vCenter = _float3(0.2f, 0.f, 0.f);
	ColliderOBBDesc_Obj.vExtents = _float3(0.6f, 0.2f, 0.2f);

	CColliderObject::COLIDEROBJECT_DESC Desc{};

	Desc.pBoundingDesc = &ColliderOBBDesc_Obj;
	Desc.eType = CCollider::TYPE_OBB;
	Desc.pSocketBoneMatrix = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_HAND_LEFT) - 1);
	Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.pSocketBoneMatrix2 = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.fDamageAmount = m_eStat.fAtk;
	Desc.pOWner = this;

	Desc.iDebuffType = CPlayer::DEBUFF_ACID;
	Desc.fDebuffAmount = 7.f;

	m_pColliderObject[TYPE_HAND_LEFT] = dynamic_cast<CColliderObject*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_ColliderObj"), &Desc));

	ColliderOBBDesc_Obj.vAngles = _float3(0.f, 0.f, 0.f);
	ColliderOBBDesc_Obj.vCenter = _float3(0.2f, 0.f, 0.f);
	ColliderOBBDesc_Obj.vExtents = _float3(0.6f, 0.2f, 0.2f);

	Desc.pBoundingDesc = &ColliderOBBDesc_Obj;
	Desc.eType = CCollider::TYPE_OBB;
	Desc.pSocketBoneMatrix = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_HAND_RIGHT) - 1);
	Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.pSocketBoneMatrix2 = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.fDamageAmount = m_eStat.fAtk;

	m_pColliderObject[TYPE_HAND_RIGHT] = dynamic_cast<CColliderObject*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_ColliderObj"), &Desc));

	ColliderOBBDesc_Obj.vAngles = _float3(0.f, 0.f, 0.f);
	ColliderOBBDesc_Obj.vCenter = _float3(0.f, 0.f, 0.f);
	ColliderOBBDesc_Obj.vExtents = _float3(0.3f, 0.3f, 0.3f);

	Desc.pBoundingDesc = &ColliderOBBDesc_Obj;
	Desc.eType = CCollider::TYPE_OBB;
	Desc.pSocketBoneMatrix = m_pModelCom->Get_BoneCombindTransformationMatrix_Ptr(m_pModelCom->Get_UFBIndices(UFB_HEAD));
	Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.pSocketBoneMatrix2 = m_pTransformCom->Get_WorldMatrix_Ptr();
	Desc.fDamageAmount = m_eStat.fAtk;

	Desc.iDebuffType = CPlayer::DEBUFF_END;
	Desc.fDebuffAmount = 0.f;

	m_pColliderObject[TYPE_HEAD] = dynamic_cast<CColliderObject*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_ColliderObj"), &Desc));

	for (_int i = 0; i < TYPE_END; ++i)
	{
		m_pColliderObject[i]->DeActive_Collider();
	}

	return S_OK;
}

HRESULT CCarcassNormal::Ready_Effect()
{
	m_SurfaceEffect.resize(SURFACE_END);

	CDissolve_Effect::DISSOLVE_EFFECT_DESC DissolveDesc = {};
	DissolveDesc.fRotationPerSec = XMConvertToRadians(90.f);
	DissolveDesc.fSpeedPerSec = 1.f;
	DissolveDesc.iLevelIndex = LEVEL_GAMEPLAY;
	DissolveDesc.pTarget_ModelCom = m_pModelCom;
	DissolveDesc.pTarget_TransformCom = m_pTransformCom;
	DissolveDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Dissolve_CarcassNormal_Fire");
	m_SurfaceEffect[SURFACE_FIRE] = static_cast<CDissolve_Effect*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_Dissolve_Fire"), &DissolveDesc));
	if (nullptr == m_SurfaceEffect[SURFACE_FIRE])
		return E_FAIL;

	DissolveDesc.strVIBufferTag = TEXT("Prototype_Component_VIBuffer_Dissolve_CarcassNormal_Electric");
	m_SurfaceEffect[SURFACE_ELECTRIC] = static_cast<CDissolve_Effect*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Effect_Dissolve_Electric"), &DissolveDesc));
	if (nullptr == m_SurfaceEffect[SURFACE_ELECTRIC])
		return E_FAIL;

	//On_PowerAttack(true);
	//On_SurfaceEffect(SURFACE_FIRE, true);
	//On_SurfaceEffect(SURFACE_ELECTRIC, true);

	return S_OK;
}

void CCarcassNormal::Update_Collider()
{
	_float4x4 UpdateMat{};

	_Matrix WorldMat = XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr());
	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_BODY_UPPER]) * WorldMat);
	m_pColliderCom->Update(&UpdateMat);

	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_BODY_LOWER]) * WorldMat);
	m_EXCollider[CT_BODY_LOWER]->Update(&UpdateMat);

	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_HEAD]) * WorldMat);
	m_EXCollider[CT_HEAD]->Update(&UpdateMat);


	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_UPPERARM_LEFT]) * WorldMat);
	m_EXCollider[CT_UPPERARM_LEFT]->Update(&UpdateMat);
	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_UPPERARM_RIGHT]) * WorldMat);
	m_EXCollider[CT_UPPERARM_RIGHT]->Update(&UpdateMat);

	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_LOWERARM_LEFT]) * WorldMat);
	m_EXCollider[CT_LOWERARM_LEFT]->Update(&UpdateMat);
	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_LOWERARM_RIGHT]) * WorldMat);
	m_EXCollider[CT_LOWERARM_RIGHT]->Update(&UpdateMat);


	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_UPPERLEG_LEFT]) * WorldMat);
	m_EXCollider[CT_UPPERLEG_LEFT]->Update(&UpdateMat);
	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_UPPERLEG_RIGHT]) * WorldMat);
	m_EXCollider[CT_UPPERLEG_RIGHT]->Update(&UpdateMat);

	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_LOWERLEG_LEFT]) * WorldMat);
	m_EXCollider[CT_LOWERLEG_LEFT]->Update(&UpdateMat);
	XMStoreFloat4x4(&UpdateMat
		, *(m_pColliderBindMatrix[CT_LOWERLEG_RIGHT]) * WorldMat);
	m_EXCollider[CT_LOWERLEG_RIGHT]->Update(&UpdateMat);
}

CCarcassNormal* CCarcassNormal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCarcassNormal* pInstance = new CCarcassNormal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCarcassNormal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CPawn* CCarcassNormal::Clone(void* pArg)
{
	CCarcassNormal* pInstance = new CCarcassNormal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCarcassNormal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCarcassNormal::Free()
{
	for (_uint i = 0; i < CT_END - 1; ++i)
	{
		Safe_Release(m_EXCollider[i]);
	}

	for (_uint i = 0; i < TYPE_END; ++i)
	{
		Safe_Release(m_pColliderObject[i]);
	}

	__super::Free();
}