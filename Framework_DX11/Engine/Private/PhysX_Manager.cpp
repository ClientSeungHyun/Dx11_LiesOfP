#include "PhysX_Manager.h"
#include "GameInstance.h"

#include "Transform.h"

#include "GameObject.h"
#include "Model.h"
#include "Mesh.h"

CPhysX_Manager::CPhysX_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice }
    , m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CPhysX_Manager::Initialize()
{
    // PhysX Foundation ����
    // PhysX�� ���õ� ��� ��ü���� ����� �Ǵ� �̱��� ��ü
    m_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxAllocator, m_PXErrorCallback);

    //CUresult result = cuInit(0);
    //if (result != CUDA_SUCCESS)
    //{
    //    return -1;
    //}

    //int deviceCount = 0;
    //cudaError_t cudaErr = cudaGetDeviceCount(&deviceCount);
    //if (cudaErr != cudaSuccess || deviceCount == 0) {
    //    // CUDA ����̽��� ���� ���
    //    return -1;
    //}

    //cudaErr = cudaSetDevice(0);
    //if (cudaErr != cudaSuccess) {
    //    return -1;
    //}
    //cudaErr = cudaFree(0); // CUDA ���ؽ�Ʈ �ʱ�ȭ
    //if (cudaErr != cudaSuccess) {
    //    return -1;
    //}

    //CUdevice cuDevice;
    //result = cuDeviceGet(&cuDevice, 0);  // 0�� ����̽� ����
    //if (result != CUDA_SUCCESS) {
    //    return -1;
    //}
    //cudaErr = cudaSetDevice(cuDevice);

    //cudaGetDevice(&deviceCount);

    //CUcontext m_pCudaContext = nullptr;
    //result = cuCtxCreate(&m_pCudaContext, 0, cuDevice);  // �ùٸ� ��ġ�� �����Ͽ� ���ؽ�Ʈ ����
    //if (result != CUDA_SUCCESS) {
    //    return -1;
    //}

    //// Cuda�� ����� GPU ���� ����
    //PxCudaContextManagerDesc cudaContextManagerDesc; 
    //cudaContextManagerDesc.graphicsDevice = m_pDevice;    
    //cudaContextManagerDesc.ctx = &m_pCudaContext;

    //cudaErr = cudaGetLastError();

    //m_CudaContextManager = PxCreateCudaContextManager(*m_PxFoundation, cudaContextManagerDesc);
    //if (m_CudaContextManager)
    //{
    //    if (!m_CudaContextManager->contextIsValid())
    //    {
    //        PX_RELEASE(m_CudaContextManager);
    //    }
    //       
    //}
    // PVD(PhysX Visual Debugger) ����
    m_Pvd = PxCreatePvd(*m_PxFoundation);
    // PVD ���� ���� ����
    m_pTransport = PxDefaultPvdSocketTransportCreate(m_pvdIPAddress.c_str(), m_iPvdPortNumber, m_iPvdTimeOutSeconds);
    m_Pvd->connect(*m_pTransport, PxPvdInstrumentationFlag::eALL);

    // PhysX SDK ����
    // ������ ��ü���� ������ �� �ִ� �̱��� ���丮 Ŭ����
    // ���̳� ���͸����� �����ϰų� ���� ��ü�� ��ü ����(Shape), ��ü �ν��Ͻ� �� ���� ��κ��� PhysX ��ü���� ������ �� ���
    m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, PxTolerancesScale(), true, m_Pvd);
    PxInitExtensions(*m_PhysX, m_Pvd);

    // �� ���� ����
    PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

    // CPU ����ó(��Ŀ ������) ����
    m_PxDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = m_PxDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;

    sceneDesc.solverBatchSize = 256;
    sceneDesc.solverArticulationBatchSize = 32;

    sceneDesc.contactReportStreamBufferSize = 16384;
    sceneDesc.contactPairSlabSize = 512;

    // GPU ���� ����
    sceneDesc.cudaContextManager = m_CudaContextManager;
    sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;
    sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;
    sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
    sceneDesc.gpuMaxNumPartitions = 8;
    sceneDesc.gpuMaxNumStaticPartitions = 255;

    // �� �����Ӹ��� ���� ��� - �ʿ��ұ�?
    //sceneDesc.flags |= PxSceneFlag::eENABLE_FRICTION_EVERY_ITERATION;

    // �� ���� PhysX ���͵��� ��ġ�� �� �ִ� ��
    m_PxScene = m_PhysX->createScene(sceneDesc);

    // PVD Ŭ���̾�Ʈ ����
    // Physx Visual Debugger ���α׷��� �� ���� ������ ������ �� � ������ ������ ���� �÷��׸� �������ִ� ��ü
    m_PvdClient = m_PxScene->getScenePvdClient();
    if (m_PvdClient)
    {
        // PVD Ŭ���̾�Ʈ �÷��� ����
        m_PvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true); //��ü ����
        m_PvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true); // �浹 ����
        m_PvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true); // Scene ����
    }

    // Material�� �������� ǥ�� ������ ���¸� ��Ÿ���� ��ü
    // ǥ���� �����������, ��������, �ݹ߰�� �� ǥ�� ������ �پ��� ������ ������ �� �ִ�
    //gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    //PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
    //gScene->addActor(*groundPlane);


    return S_OK;
}

void CPhysX_Manager::PhysX_Update(_float fTimeDelta)
{
   

    //for (_uint i = 0; i < m_Monsters.size(); ++i)
    //{
    //    CRigidBody* pMonsterRigidBody = dynamic_cast<CRigidBody*>(m_Monsters[i]->Find_Component(RIGIDBODY));
    //    CNavigation* pNavigation = dynamic_cast<CNavigation*>(m_Monsters[i]->Find_Component(NAVIGATION));

    //    // 1. ���� �ӵ� ��������
    //    _Vec3 vVelocity = pMonsterRigidBody->Get_Velocity();

    //    // 2. ���� PhysX ��ġ ��������
    //    PxTransform MonsterPxTransform = m_MonsterRigids[i]->getGlobalPose();
    //    PxVec3 vPxMonsterPos = MonsterPxTransform.p;
    //    _Vec3 vPos = _Vec3(vPxMonsterPos.x, vPxMonsterPos.y, vPxMonsterPos.z);
    //   
    //    // 3. �̵� ���� ���� �Ǵ�
    //    if (!pNavigation->isMove(vPos + vVelocity * fTimeDelta)) {
    //        // �̵� �Ұ�: �ӵ��� 0���� ����
    //        m_MonsterRigids[i]->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));

    //        // PhysX ��ġ�� ���� (�߰� �̵� ����)
    //        MonsterPxTransform.p = PxVec3(vPos.x, vPos.y, vPos.z);
    //        m_MonsterRigids[i]->setGlobalPose(MonsterPxTransform);
    //    }
    //    else {
    //        // �̵� ����: ���� �ӵ� ����
    //        m_MonsterRigids[i]->setLinearVelocity(PxVec3(vVelocity.x, vVelocity.y, vVelocity.z));
    //    }

    //    MonsterPxTransform = m_MonsterRigids[i]->getGlobalPose();
    //    vPxMonsterPos = MonsterPxTransform.p;

    //    m_Monsters[i]->Get_Transform()->Set_State(CTransform::STATE_POSITION, _Vec3(vPxMonsterPos.x, vPxMonsterPos.y, vPxMonsterPos.z));

    //    // 4. y�� ���� ���� (�׺���̼�)
    //    vPxMonsterPos.y = pNavigation->SetUp_OnCell(m_pPlayer->Get_Transform(), 0.f, fTimeDelta);

    //    // 5. y�� ���� �� ��ġ ����ȭ
    //    MonsterPxTransform.p = vPxMonsterPos;
    //    m_MonsterRigids[i]->setGlobalPose(MonsterPxTransform);

    //    // 6. ���� Transform ����ȭ
    //    m_Monsters[i]->Get_Transform()->Set_State(CTransform::STATE_POSITION, _Vec3(vPxMonsterPos.x, vPxMonsterPos.y, vPxMonsterPos.z));
    //}

    //CRigidBody* pPlayerRigidBody = dynamic_cast<CRigidBody*>(m_pPlayer->Find_Component(RIGIDBODY));
    //CNavigation* pNavigation = dynamic_cast<CNavigation*>(m_pPlayer->Find_Component(NAVIGATION));

    //// 1. ���� �ӵ� ��������
    //_Vec3 vVelocity = pPlayerRigidBody->Get_Velocity();

    //// 2. ���� PhysX ��ġ ��������
    //PxTransform PlayerPxTransform = m_pPlayerRigid->getGlobalPose();
    //PxVec3 vPxPlayerPos = PlayerPxTransform.p;
    //_Vec3 vPos = _Vec3(vPxPlayerPos.x, vPxPlayerPos.y, vPxPlayerPos.z);

    //// 3. �̵� ���� ���� �Ǵ�
    //if (!pNavigation->isMove(vPos + vVelocity * fTimeDelta)) {
    //    // �̵� �Ұ�: �ӵ��� 0���� ����
    //    m_pPlayerRigid->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));

    //    // PhysX ��ġ�� ���� (�߰� �̵� ����)
    //    PlayerPxTransform.p = PxVec3(vPos.x, vPos.y, vPos.z);
    //    m_pPlayerRigid->setGlobalPose(PlayerPxTransform);
    //}
    //else {
    //    // �̵� ����: ���� �ӵ� ����
    //    m_pPlayerRigid->setLinearVelocity(PxVec3(vVelocity.x, vVelocity.y, vVelocity.z));
    //}

    //PlayerPxTransform = m_pPlayerRigid->getGlobalPose();
    //vPxPlayerPos = PlayerPxTransform.p;

    //m_pPlayer->Get_Transform()->Set_State(CTransform::STATE_POSITION, _Vec3(vPxPlayerPos.x, vPxPlayerPos.y, vPxPlayerPos.z));

    //// 4. y�� ���� ���� (�׺���̼�)
    //vPxPlayerPos.y = pNavigation->SetUp_OnCell(m_pPlayer->Get_Transform(), 0.f, fTimeDelta);

    //// 5. y�� ���� �� ��ġ ����ȭ
    //PlayerPxTransform.p = vPxPlayerPos;
    //m_pPlayerRigid->setGlobalPose(PlayerPxTransform);

    //// 6. ���� Transform ����ȭ
    //m_pPlayer->Get_Transform()->Set_State(CTransform::STATE_POSITION, _Vec3(vPxPlayerPos.x, vPxPlayerPos.y, vPxPlayerPos.z));


    // ���� �� ������Ʈ
    m_PxScene->simulate(fTimeDelta);
    m_PxScene->fetchResults(true);
}

_bool CPhysX_Manager::RayCast(PxVec3 vRayPos, PxVec3 vRayDir, _vector* vHitPos, _vector* vNormal, _float* fHitDistance)
{
    _bool isHit = false;

    PxRaycastHit hit;
    PxRaycastBuffer hitBuffer(&hit);
    PxReal fLength = 100.f;

    PxVec3 vRayEnd = vRayPos + vRayDir * fLength;

    PxQueryFilterData filterData(PxQueryFlag::eSTATIC);

    // ����ĳ��Ʈ ����
    // ���� ó�� �浹�� ���� ��ȯ eMESH_ANY
    // ����ƽ ��ü���� ���� eSTATIC
    if (isHit = m_PxScene->raycast(vRayPos, vRayDir, fLength, hitBuffer, PxHitFlag::eMESH_ANY, filterData))
    {
        hit = hitBuffer.block;
        PxShape* shape = hitBuffer.block.shape;

        if (shape)
        {
            const PxGeometryHolder geometry = shape->getGeometry();
            PxGeometryType::Enum eType = geometry.getType();

            if (eType == PxGeometryType::eTRIANGLEMESH)
            {
                const PxTriangleMeshGeometry& triangleMeshGeom = geometry.triangleMesh();
                PxTriangleMesh* triangleMesh = triangleMeshGeom.triangleMesh;

                // �浹 ������ ��ġ
                PxVec3 collisionPoint = hitBuffer.block.position;

                // �ﰢ���� ���� ������ ��������
                PxU32 triangleIndex = hitBuffer.block.faceIndex;
                const void* indexBuffer = triangleMesh->getTriangles();
                const PxVec3* vertices = triangleMesh->getVertices();

                // �ﰢ�� �ε��� Ÿ�Կ� ���� ĳ����
                PxU32 idx0, idx1, idx2;

                if (triangleMesh->getTriangleMeshFlags() & PxTriangleMeshFlag::e16_BIT_INDICES) {
                    const PxU16* indices = reinterpret_cast<const PxU16*>(indexBuffer);
                    idx0 = indices[3 * triangleIndex];
                    idx1 = indices[3 * triangleIndex + 1];
                    idx2 = indices[3 * triangleIndex + 2];
                }
                else {
                    const PxU32* indices = reinterpret_cast<const PxU32*>(indexBuffer);
                    idx0 = indices[3 * triangleIndex];
                    idx1 = indices[3 * triangleIndex + 1];
                    idx2 = indices[3 * triangleIndex + 2];
                }

                // �ﰢ���� ���� ��ġ ��������
                PxVec3 v0 = vertices[idx0];
                PxVec3 v1 = vertices[idx1];
                PxVec3 v2 = vertices[idx2];

                // �ﰢ���� ���� ���

                // �� ���͸� ����
                PxVec3 edge1 = v1 - v0;
                PxVec3 edge2 = v2 - v1;

                PxVec3 normal = edge1.cross(edge2);
                normal.normalize(); // ������ ����ȭ

                *vNormal = XMVectorSet(normal.x, normal.y, normal.z, 0.f);
                *vHitPos = XMVectorSet(hitBuffer.block.position.x, hitBuffer.block.position.y, hitBuffer.block.position.z, 1.f);
                *fHitDistance = hitBuffer.block.distance;
            }
        }
    }

#ifdef _DEBUG

    if (m_PvdClient && isHit)
    {
        PxDebugLine lines = { PxDebugLine(vRayPos, hitBuffer.block.position, PxDebugColor::eARGB_YELLOW) };
        m_PvdClient->drawLines(&lines, 1);
    }
#endif

    return isHit;
}

_bool CPhysX_Manager::RayCast(_vector vRayPos, _vector vRayDir, _vector* vHitPos, _vector* vNormal, _float* fHitDistance)
{
    _bool isHit = false;

    PxRaycastHit hit;
    PxRaycastBuffer hitBuffer(&hit);
    PxReal fLength = 100.f;

    //PxVec3 vRayEnd = vRayPos + vRayDir * fLength;

    PxVec3 PXvRayPos = PxVec3(XMVectorGetX(vRayPos), XMVectorGetY(vRayPos), XMVectorGetZ(vRayPos));
    PxVec3 PXvRayDir = PxVec3(XMVectorGetX(vRayDir), XMVectorGetY(vRayDir), XMVectorGetZ(vRayDir));

    PxQueryFilterData filterData(PxQueryFlag::eSTATIC);

    // ����ĳ��Ʈ ����
    // ���� ó�� �浹�� ���� ��ȯ eMESH_ANY
    // ����ƽ ��ü���� ���� eSTATIC
    if (isHit = m_PxScene->raycast(PXvRayPos, PXvRayDir, fLength, hitBuffer, PxHitFlag::eMESH_ANY, filterData))
    {
        hit = hitBuffer.block;
        PxShape* shape = hitBuffer.block.shape;

        if (shape)
        {
            const PxGeometryHolder geometry = shape->getGeometry();
            PxGeometryType::Enum eType = geometry.getType();

            if (eType == PxGeometryType::eTRIANGLEMESH)
            {
                const PxTriangleMeshGeometry& triangleMeshGeom = geometry.triangleMesh();
                PxTriangleMesh* triangleMesh = triangleMeshGeom.triangleMesh;

                // �浹 ������ ��ġ
                PxVec3 collisionPoint = hitBuffer.block.position;

                // �ﰢ���� ���� ������ ��������
                PxU32 triangleIndex = hitBuffer.block.faceIndex;
                const void* indexBuffer = triangleMesh->getTriangles();
                const PxVec3* vertices = triangleMesh->getVertices();

                // �ﰢ�� �ε��� Ÿ�Կ� ���� ĳ����
                PxU32 idx0, idx1, idx2;

                if (triangleMesh->getTriangleMeshFlags() & PxTriangleMeshFlag::e16_BIT_INDICES) {
                    const PxU16* indices = reinterpret_cast<const PxU16*>(indexBuffer);
                    idx0 = indices[3 * triangleIndex];
                    idx1 = indices[3 * triangleIndex + 1];
                    idx2 = indices[3 * triangleIndex + 2];
                }
                else {
                    const PxU32* indices = reinterpret_cast<const PxU32*>(indexBuffer);
                    idx0 = indices[3 * triangleIndex];
                    idx1 = indices[3 * triangleIndex + 1];
                    idx2 = indices[3 * triangleIndex + 2];
                }

                // �ﰢ���� ���� ��ġ ��������
                PxVec3 v0 = vertices[idx0];
                PxVec3 v1 = vertices[idx1];
                PxVec3 v2 = vertices[idx2];

                // �ﰢ���� ���� ���

                // �� ���͸� ����
                PxVec3 edge1 = v1 - v0;
                PxVec3 edge2 = v2 - v1;

                PxVec3 normal = edge1.cross(edge2);
                normal.normalize(); // ������ ����ȭ

                *vNormal = XMVectorSet(normal.x, normal.y, normal.z, 0.f);
                *vHitPos = XMVectorSet(hitBuffer.block.position.x, hitBuffer.block.position.y, hitBuffer.block.position.z, 1.f);
                *fHitDistance = hitBuffer.block.distance;
            }
        }
    }

#ifdef _DEBUG

    if (m_PvdClient && isHit)
    {
        PxDebugLine lines = { PxDebugLine(PXvRayPos, hitBuffer.block.position, PxDebugColor::eARGB_YELLOW) };
        m_PvdClient->drawLines(&lines, 1);
    }
#endif

    return isHit;
}

PxTransform CPhysX_Manager::Get_PhysXTransform(CGameObject* pObject)
{
    //�̰� �³�?
    //��ư �������� Transfrom���� ��ȯ
    const _float4x4* WorldMat = pObject->Get_Transform()->Get_WorldMatrix_Ptr();

    PxVec3 position(WorldMat->_41, WorldMat->_42, WorldMat->_43);

    // ȸ�� ���� (3x3 ��� ����� ���ʹϾ����� ��ȯ)
    PxMat33 rotation(
        PxVec3(WorldMat->_11, WorldMat->_12, WorldMat->_13),
        PxVec3(WorldMat->_21, WorldMat->_22, WorldMat->_23),
        PxVec3(WorldMat->_31, WorldMat->_32, WorldMat->_33)
    );
    PxQuat quaternion = PxQuat(rotation);

    return PxTransform(position, quaternion);
}

CPhysX_Manager* CPhysX_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPhysX_Manager* pInstance = new CPhysX_Manager(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed to Created : CPhysX_Manager"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPhysX_Manager::Free()
{
    __super::Free();

    PhysX_RELEASE(m_PxScene);

    // PhysX Extensions �ݱ�
    PxCloseExtensions();

    PhysX_RELEASE(m_PxDispatcher);
    PhysX_RELEASE(m_PhysX);


    PxPvdTransport* transport = m_Pvd->getTransport();
    m_pTransport->disconnect();
    PhysX_RELEASE(m_Pvd);
    PhysX_RELEASE(m_pTransport);

    PhysX_RELEASE(m_CudaContextManager);
    // ���� �������� Release
    PhysX_RELEASE(m_PxFoundation);

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
}