#include "stdafx.h"
#include "Stargazer.h"
#include "GameInstance.h"
#include "Pawn.h"
CStargazer::CStargazer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CStargazer::CStargazer(const CStargazer& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CStargazer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStargazer::Initialize(void* pArg)
{
	OBJECT_DEFAULT_DESC* pDesc = static_cast<OBJECT_DEFAULT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(pDesc->vScale.x, pDesc->vScale.y, pDesc->vScale.z);
	m_pTransformCom->Rotation(0.f, pDesc->vRotation.y, pDesc->vRotation.z);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&pDesc->vPosition));
	m_bShadow = pDesc->bShadow;
	m_iCurrnetCellNum = pDesc->iCurrentCellNum;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pPlayer = m_pGameInstance->Find_Player(LEVEL_GAMEPLAY);
	 
	m_iAnim_Close = m_pModelCom[RESTORED]->Find_AnimationIndex("AS_Close_Idle", 1.f);
	m_iAnim_Open = m_pModelCom[RESTORED]->Find_AnimationIndex("AS_Open", 0.5f);
	m_iAnim_OpenIdle = m_pModelCom[RESTORED]->Find_AnimationIndex("AS_Open_Idle", 1.f);
	m_iAnim_Broken = m_pModelCom[BROKEN]->Find_AnimationIndex("AS_Stargazer_broken_idle", 1.f);
	m_iAnim_Restore = m_pModelCom[BROKEN]->Find_AnimationIndex("AS_Stargazer_restore", 2.f);
	
	m_pModelCom[RESTORED]->SetUp_Animation(m_iAnim_Close, true);
	m_pModelCom[BROKEN]->SetUp_Animation(m_iAnim_Broken, true);

	m_pCurrentModel = m_pModelCom[BROKEN];

	m_strObjectTag = TEXT("Stargazer");

	return S_OK;
}

void CStargazer::Priority_Update(_float fTimeDelta)
{
}

void CStargazer::Update(_float fTimeDelta)
{
	m_pCurrentModel->Play_Animation(fTimeDelta);

	if (m_bCollison)
	{
		if (KEY_TAP(KEY::E))
		{
			//Ű �Է� Ȯ�� �κ��� ���߿� UI���� �޾ƿ;� ��
			static_cast<CPawn*>(m_pPlayer)->Set_Respawn_Cell_Num(m_iCurrnetCellNum);

			if (m_bRestored == false)
			{
				m_pCurrentModel->SetUp_Animation(m_iAnim_Restore);
			}
		}
	}

	//���� �ִϸ��̼� ���� ��
	if (m_bRestored == false && m_pCurrentModel->Get_CurrentAnimationIndex() == m_iAnim_Restore)
	{
		m_fRestoreTimer += fTimeDelta;

		if (m_fRestoreTimer > 3.3f)
		{
			m_pModelCom[RESTORED]->Play_Animation(fTimeDelta);
		}

		if (m_fRestoreTimer > 3.8f)
		{
			m_bRestored = true;
			m_pCurrentModel = m_pModelCom[RESTORED];
			m_fRestoreTimer = 0.f; 
		}
	}

	if (m_bRestored && m_isClose)
	{
		m_isClose = false;
		m_isOpening = true;
		m_pCurrentModel->SetUp_NextAnimation(m_iAnim_Open);
	}

	//������ ��->������ ���� �⺻ �ִϷ� ����
	if (m_isOpening && m_pCurrentModel->Get_IsEndAnimArray())
	{
		m_isOpening = false;
		m_isOpened = true;
		m_pCurrentModel->SetUp_NextAnimation(m_iAnim_OpenIdle, true);
	}

	if(m_pColliderCom != nullptr)
		m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix_Ptr());
}

void CStargazer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_ColliderList(m_pColliderCom);

#ifdef _DEBUG
	if (m_pColliderCom != nullptr)
		m_pGameInstance->Add_DebugObject(m_pColliderCom);
#endif
}

HRESULT CStargazer::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	_uint		iNumMeshes = m_pCurrentModel->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pCurrentModel->Bind_MeshBoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i);

		if (FAILED(m_pCurrentModel->Bind_Material(m_pShaderCom, "g_DiffuseTexture", DIFFUSE, (_uint)i)))
			return E_FAIL;

		if (nullptr != m_pCurrentModel->Find_Texture((_uint)i, TEXTURE_TYPE::ROUGHNESS))
		{
			if (FAILED(m_pCurrentModel->Bind_Material(m_pShaderCom, "g_ARMTexture", ROUGHNESS, (_uint)i)))
				return E_FAIL;
		}

		if (nullptr != m_pCurrentModel->Find_Texture((_uint)i, TEXTURE_TYPE::NORMALS))
		{
			if (FAILED(m_pCurrentModel->Bind_Material(m_pShaderCom, "g_NormalTexture", NORMALS, (_uint)i)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(2)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;
		}

		if (FAILED(m_pCurrentModel->Render((_uint)i)))
			return E_FAIL;
	}

	if (m_fRestoreTimer > 3.5f && m_bRestored == false)
	{
		_uint		iNumMeshes = m_pModelCom[RESTORED]->Get_NumMeshes();

		for (size_t i = 0; i < iNumMeshes; i++)
		{
			m_pModelCom[RESTORED]->Bind_MeshBoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i);

			if (FAILED(m_pModelCom[RESTORED]->Bind_Material(m_pShaderCom, "g_DiffuseTexture", DIFFUSE, (_uint)i)))
				return E_FAIL;

			if (nullptr != m_pModelCom[RESTORED]->Find_Texture((_uint)i, TEXTURE_TYPE::ROUGHNESS))
			{
				if (FAILED(m_pModelCom[RESTORED]->Bind_Material(m_pShaderCom, "g_ARMTexture", ROUGHNESS, (_uint)i)))
					return E_FAIL;
			}

			if (nullptr != m_pModelCom[RESTORED]->Find_Texture((_uint)i, TEXTURE_TYPE::NORMALS))
			{
				if (FAILED(m_pModelCom[RESTORED]->Bind_Material(m_pShaderCom, "g_NormalTexture", NORMALS, (_uint)i)))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Begin(2)))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pShaderCom->Begin(0)))
					return E_FAIL;
			}

			if (FAILED(m_pModelCom[RESTORED]->Render((_uint)i)))
				return E_FAIL;
		}
	}
	return S_OK;

}

void CStargazer::OnCollisionEnter(CGameObject* pOther)
{
	if (pOther->Get_Tag() == TEXT("Player"))
	{
		m_bCollison = true;
	}
}

void CStargazer::OnCollisionStay(CGameObject* pOther)
{
}

void CStargazer::OnCollisionExit(CGameObject* pOther)
{
	if (pOther->Get_Tag() == TEXT("Player"))
	{
		m_bCollison = false;
	}
}

HRESULT CStargazer::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* FOR.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stargazer_Broken"),
		TEXT("Com_Model0"), reinterpret_cast<CComponent**>(&m_pModelCom[BROKEN]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stargazer"),
		TEXT("Com_Model1"), reinterpret_cast<CComponent**>(&m_pModelCom[RESTORED]))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC			ColliderDesc{};
	ColliderDesc.vExtents = _float3(1.5f, 1.0f, 1.5f);
	ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;
	m_pColliderCom->Set_Owner(this);

	// �׻� �������� �����ϱ�
	CRigidBody::RIGIDBODY_DESC RigidBodyDesc{};
	RigidBodyDesc.isStatic = false;
	RigidBodyDesc.isGravity = false;
	RigidBodyDesc.pOwnerTransform = m_pTransformCom;
	RigidBodyDesc.pOwnerNavigation = nullptr;

	RigidBodyDesc.pOwner = this;
	RigidBodyDesc.fStaticFriction = 0.f;
	RigidBodyDesc.fDynamicFriction = 0.f;
	RigidBodyDesc.fRestituion = 0.f;
	RigidBodyDesc.PxLockFlags = PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z |
		PxRigidDynamicLockFlag::eLOCK_LINEAR_X |
		PxRigidDynamicLockFlag::eLOCK_LINEAR_Y |
		PxRigidDynamicLockFlag::eLOCK_LINEAR_Z
		;

	physX::GeometryCapsule CapsuleDesc;
	CapsuleDesc.fHeight = 0.5f;
	CapsuleDesc.fRadius = 0.25f;
	RigidBodyDesc.pGeometry = &CapsuleDesc;

	/* FOR.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		TEXT("Com_RigidBody"), reinterpret_cast<CComponent**>(&m_pRigidBodyCom), &RigidBodyDesc)))
		return E_FAIL;
	return S_OK;
}

CStargazer* CStargazer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStargazer* pInstance = new CStargazer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CStargazer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStargazer::Clone(void* pArg)
{
	CStargazer* pInstance = new CStargazer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CStargazer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStargazer::Free()
{
	__super::Free();
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pShaderCom);

	for (int i = 0; i < 2; ++i)
	{
		Safe_Release(m_pModelCom[i]);
	}

	Safe_Release(m_pRigidBodyCom);
}
