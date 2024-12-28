#pragma once
#include "Base.h"

// ����� ��忡�� ���� ������� crtdebug ������ PhysX�� new �����ǿ��� ������ ��
// ������  ��������� �����ϱ� ���� undef�� new �����Ǹ� ��� ������
#ifdef _DEBUG
#undef new
#endif

// PhysX ��������� ������ �� ��ũ�� �����Ǹ� �ٽ� Ȱ��ȭ
#ifdef _DEBUG
#define new DBG_NEW
#endif


BEGIN(Engine)

class CTransform;
class CGameObject;

class ENGINE_DLL CPhysX_Manager :
    public CBase
{

private:
    CPhysX_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CPhysX_Manager() = default;

public:
    PxPhysics*      Get_PhysX() const { return m_PhysX; }
    PxScene*        Get_PhysXScene() const { return m_PxScene; }
    PxFoundation*   Get_PhysXFoundation() const { return m_PxFoundation; }
    PxCudaContextManager* Get_PhysXCuda() const { return m_CudaContextManager; }

    void                Set_Gravity(_float fY) { m_PxScene->setGravity(PxVec3(0.f, fY, 0.f)); }

public:
    HRESULT Initialize();
    void    PhysX_Update(_float fTimeDelta);

public:
    _bool   RayCast(PxVec3 vRayPos, PxVec3 vRayDir, _vector* vHitPos, _vector* vNormal, _float* fHitDistance);
    _bool   RayCast(_vector vRayPos, _vector vRayDir, _vector* vHitPos, _vector* vNormal, _float* fHitDistance);

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

private:
    // PhysX Foundation ��ü - PhysX �ý����� �⺻ �ν��Ͻ�. �޸� �Ҵ��ڿ� ���� �ݹ��� �ʱ�ȭ�ϴµ� ���
    PxFoundation* m_PxFoundation = { nullptr };

    // PVD(PhysX Visual Debugger) ��ü - PhysX�� ����� ������ �ð������� �� �� �ְ� ���ִ� ����
    PxPvd* m_Pvd = { nullptr };
    PxPvdTransport* m_pTransport = { nullptr };
    PxPvdSceneClient* m_PvdClient = { nullptr };

    // PhysX SDK ��ü - ���� ������ �ٽ�. ���� �ùķ��̼��� ���� ��� ���� �� ���� ���
    PxPhysics* m_PhysX = { nullptr };

    // GPU ���
    PxCudaContextManager* m_CudaContextManager = nullptr;

    // ���� �� ��ü - ���� �ùķ��̼��� �̷������ ����. �浹, �߷�, �� ���� ��ȣ�ۿ��� ó��
    PxScene* m_PxScene = { nullptr };

    // PhysX �Ҵ��� �� ���� �ݹ� - �޸� �Ҵ�� ���� ó���� ���� �ݹ� Ŭ����
    PxDefaultAllocator m_PxAllocator;
    PxDefaultErrorCallback m_PXErrorCallback;

    // PVD ���� ���� - ����� ������ ������ IP �ּ�
    std::string m_pvdIPAddress = "127.0.0.1";

    // ����ϱ� ���� ��Ʈ��ȣ - PVD�� ����ϴ� ��Ʈ ��ȣ - �⺻ 5425
    _int m_iPvdPortNumber = 5425;

    // 10�� �ȿ� ���� �� �Ǹ� ������ - PVD ���� Ÿ�Ӿƿ� ����
    _uint m_iPvdTimeOutSeconds = 10;

    // CPU ����ó(��Ŀ ������) ��ü - ���� �ùķ��̼��� ���ķ� ó���ϱ� ���� CPU ����ó
    PxDefaultCpuDispatcher* m_PxDispatcher = { nullptr };

    vector<PxTriangleMeshGeometry*> m_ColMesheGeometries;

    // �ӽ÷� ���Ǵ� ���� Actor ���
    vector<PxRigidStatic*> m_StaticActors;

private:
    // �־��� ���� ������Ʈ�� PhysX ��ȯ ������ ��� ���� �Լ�
    PxTransform Get_PhysXTransform(CGameObject* pObject);


public:
    static CPhysX_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;
};

END

/* PhysX ���̺귯�� ���ϵ�
PhysX_64.lib
PhysXCommon_64.lib
PhysXCooking_64.lib
PhysXExtensions_static_64.lib
PhysXFoundation_64.lib
PhysXPvdSDK_static_64.lib */