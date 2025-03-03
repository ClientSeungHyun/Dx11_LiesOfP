#include "stdafx.h"
#include "State_RebornerMale_Idle.h"
#include "GameInstance.h"
#include "Model.h"
#include "RebornerMale.h"

#include "GameInterface_Controller.h"

CState_RebornerMale_Idle::CState_RebornerMale_Idle(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_RebornerMale_Idle::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    m_pDiscover = m_pMonster->Get_bDicover();
    m_pFirstMeetCheck = m_pMonster->Get_FirstMeetCheck();

    return S_OK;
}

HRESULT CState_RebornerMale_Idle::Start_State(void* pArg)
{
    m_pMonster->Change_Animation(AN_IDLE, true, 0.1f, 0);

    m_bRunning = false;
    m_bWalk = false;


    return S_OK;
}

void CState_RebornerMale_Idle::Update(_float fTimeDelta)
{
    if (m_pMonster->Get_TargetDead())
    {
        m_pMonster->Change_Animation(AN_IDLE, true, 0.1f);
        return;
    }

    _float fDist = m_pMonster->Calc_Distance_XZ();
    if (!(*m_pDiscover))
    {
        _Vec3 vTargetPos = m_pMonster->Get_TargetPos();
        _Vec3 vMonsterPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);

        if (fabs(vTargetPos.y - vMonsterPos.y) <= 0.6f)
        {
            _Vec3 vTargetDir = m_pMonster->Get_TargetDir();
            _Vec3 vLook = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);
            if (fDist <= m_fDiscoverDist)
            {
                _int iDir = m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_pMonster->Get_TargetDir(), 0.6f, fTimeDelta);
                switch (iDir)
                {
                case -1:
                    m_pMonster->Change_Animation(AN_TURN_LEFT, true, 0.1f);
                    break;

                case 0:
                    m_pMonster->Change_Animation(AN_IDLE, true, 0.1f);
                    break;

                case 1:
                    m_pMonster->Change_Animation(AN_TURN_RIGHT, true, 0.1f);
                    break;

                default:
                    break;
                }
            }
            else
            {
                m_pMonster->Change_Animation(AN_IDLE, true, 0.1f, 0);
            }

            if (fDist <= m_fRecognitionDist)
            {
                if (!(*m_pFirstMeetCheck))
                {
                    GET_GAMEINTERFACE->Set_OnOff_OrthoUI(true, m_pMonster);

                    (*m_pFirstMeetCheck) = true;
                }

                _float fRadian = vTargetDir.Dot(vLook);
                if (fRadian >= XMConvertToRadians(60.f))
                {
                    *m_pDiscover = true;
                    return;
                }

            }
        }
        else
        {
            return;
        }
    }
    else if (m_fIdleEndDuration <= m_fIdleTime)
    {
        if (fDist >= 30.f)
        {
            (*m_pFirstMeetCheck) = false;
            *m_pDiscover = false;
            return;
        }
        if (fDist < 3.f)
        {
            m_pMonster->Change_State(CRebornerMale::KICK);
            return;
        }

        if (fDist <= m_fNeedDist_ForAttack)
        {
            Calc_Act_Attack(fTimeDelta, fDist);
            return;
        }
        else if (fDist > m_fNeedDist_ForAttack + m_fRunningWeights || m_bRunning)
        {
            if (!m_bRunning)
            {
                m_pMonster->Change_Animation(AN_RUN, true, 0.1f, 0);
                m_bRunning = true;
            }
            m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_pMonster->Get_TargetDir(), 2.f, fTimeDelta);
            _Vec3 vDir = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);

            return;
        }
        else if (fDist > m_fNeedDist_ForAttack)
        {
            if (!m_bWalk)
            {
                m_pMonster->Change_Animation(AN_WALK, true, 0.1f, 0);
                m_bWalk = true;
            }
            m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_pMonster->Get_TargetDir(), 1.5, fTimeDelta);
            _Vec3 vDir = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);

            m_pMonster->Get_RigidBody()->Set_Velocity(XMVector3Normalize(vDir) * m_fWalkSpeed);
            return;
        }

    }
    else
    {
        _int iDir = m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_pMonster->Get_TargetDir(), 2, fTimeDelta);
        switch (iDir)
        {
        case -1:
            m_pMonster->Change_Animation(AN_TURN_LEFT, true, 0.1f);
            break;

        case 0:
            m_pMonster->Change_Animation(AN_IDLE, true, 0.1f);
            break;

        case 1:
            m_pMonster->Change_Animation(AN_TURN_RIGHT, true, 0.1f);
            break;

        default:
            break;
        }
        m_fIdleTime += fTimeDelta;
    }

}

void CState_RebornerMale_Idle::End_State()
{
    m_fIdleTime = 0.f;
}

void CState_RebornerMale_Idle::Calc_Act_Attack(_float fTimeDelta, _float fDist)
{
    if (m_iAtkCnt >= 2)
    {
        m_iAtkCnt = 0;
    }

    switch (m_iAtkCnt)
    {
    case 0:
        m_pMonster->Change_State(CRebornerMale::RAPIDFIRE);
        m_fNeedDist_ForAttack = 500.f;
        break;

    case 1:
        m_pMonster->Change_State(CRebornerMale::RELOAD);
        m_fNeedDist_ForAttack = 13.f;
        break;

    default:
        break;
    }
    ++m_iAtkCnt;
}

CState_RebornerMale_Idle* CState_RebornerMale_Idle::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_RebornerMale_Idle* pInstance = new CState_RebornerMale_Idle(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_RebornerMale_Idle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_RebornerMale_Idle::Free()
{
    __super::Free();
}