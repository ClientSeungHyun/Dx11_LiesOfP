#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRigidBody final : public CComponent
{
public:
    typedef struct
    {
        _bool               isStatic = false;
        _bool               isGravity = false;
        _bool               isOnCell = true;
        _bool               isLockCell = true;
        _bool               isCapsule = false;
        _bool               isUseClient = false;

        _float              fStaticFriction = 0.5f;
        _float              fDynamicFriction = 0.5f;
        _float              fRestituion = 0.f;
        
        _Vec3               vOffset = { 0.f,0.f,0.f };

        MyPhysX::Physx_Geometry_Desc*    pGeometryDesc = { nullptr };
        PxRigidDynamicLockFlags PxLockFlags = {};

        class CGameObject* pOwner = { nullptr };
        class CTransform* pOwnerTransform = { nullptr };
        class CNavigation* pOwnerNavigation = { nullptr };

    }RIGIDBODY_DESC;

private:
    CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CRigidBody(const CRigidBody& Prototype);
    virtual ~CRigidBody() = default;

public:
    void        Set_IsOnCell(_bool isOnCell) { m_isOnCell = isOnCell; }
    void        Set_IsLockCell(_bool isLockCell) { m_isLockCell = isLockCell; }
    _Vec3&      Get_Velocity() { return m_vVelocity; }

    void        Set_IsUseForce(_bool isUseForce) { m_isUseForce = isUseForce; }

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg) override;
    void Update(_float fTimeDelta);

public:
    void Set_Velocity(const _Vec3& vVelocity);
    void Add_Velocity(const _Vec3& vVelocity);
    void Add_Force(const _Vec3& vForce, PxForceMode::Enum _eMode = PxForceMode::eFORCE);
    void Set_Mass(_float fMass);
    void Set_GloblePose(const _Vec3& vPos);
    void Set_Gravity(_bool isGravity);
    void Set_Kinematic(_bool isKinematic);
    void Add_Actor();
    void Remove_Actor();

    void Set_NavOffsetY(_float fOffsetY) { m_fNavOffsetY = fOffsetY; }

private:
    class CGameObject* m_pOwner = { nullptr };
    class CTransform* m_pOwnerTransform = { nullptr };
    class CNavigation* m_pOwnerNavigation = { nullptr };

private:
    _bool			m_isStatic = { false };
    _bool           m_isOnCell = { true };
    _bool           m_isLockCell = { true };
    _bool           m_isUseClient = { false };
    _bool           m_isUseForce = { false };

    PxPhysics*      m_pPhysX = { nullptr };
    PxScene*        m_PxScene = { nullptr };
    PxCudaContextManager* m_PxCudaContextManager = { nullptr };

    PxRigidActor* m_PxActor = { nullptr };
    PxMaterial* m_PxMaterial = { nullptr };
    vector<PxShape*> m_PxShapes;  //여러개 붙일 수도 있음

    _Vec3 m_vVelocity = {};
    _Vec3 m_vForce = {};
    _Vec3 m_vOffset = {};

    _float  m_fNavOffsetY = {};

private:
    HRESULT Add_PxActor(RIGIDBODY_DESC* pDesc);
    HRESULT Add_PxGeometry(RIGIDBODY_DESC* pDesc);


    PxVec3 ConvertToPxVec3(const _Vec3& _vector)
    {
        return PxVec3(_vector.x, _vector.y, _vector.z);
    }

    PxVec4 ConvertToPxVec4(const _Vec4& _vector)
    {
        return PxVec4(_vector.x, _vector.y, _vector.z, _vector.w);
    }

    PxQuat ConvertToPxQuat(const _Quaternion& _Quaternion)
    {
        return PxQuat(_Quaternion.x, _Quaternion.y, _Quaternion.z, _Quaternion.w);
    }

    PxTransform ConvertToPxTransform(const _Vec3& _vector, const _Quaternion& _Quaternion)
    {
        return PxTransform(ConvertToPxVec3(_vector), ConvertToPxQuat(_Quaternion));
    }

public:
    static CRigidBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END