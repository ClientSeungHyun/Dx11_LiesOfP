#include "stdafx.h"
#include "State_CarcassBigA_HitFatal.h"
#include "GameInstance.h"
#include "Model.h"
#include "CarcassBigA.h"

CState_CarcassBigA_HitFatal::CState_CarcassBigA_HitFatal(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_CarcassBigA_HitFatal::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    //FSM_INIT_DESC* pDesc = static_cast<FSM_INIT_DESC*>(pArg);

    return S_OK;
}

HRESULT CState_CarcassBigA_HitFatal::Start_State(void* pArg)
{
    _Vec3 vRight = XMVectorSetY(m_pMonster->Get_Transform()->Get_State(CTransform::STATE_RIGHT), 0);
    _Vec3 vDir = m_pMonster->Get_TargetDir();
    vDir.Normalize();
    vRight.Normalize();

    _Vec3 fDirCheck{};
    fDirCheck = vRight.Cross(vDir);

    if (fDirCheck.y < 0)
    {
        m_iDirCnt = DIR::DIR_FRONT;
    }
    else
    {
        m_iDirCnt = DIR::DIR_BEHIND;
    }
    m_pMonster->Change_Animation(AN_FATAL_START_F - m_iDirCnt - (2 * m_iAnimCnt), false, 0.f);

    return S_OK;
}

void CState_CarcassBigA_HitFatal::Update(_float fTimeDelta)
{
    if (End_Check())
    {
        if (m_iAnimCnt < 2)
        {
            ++m_iAnimCnt;
            m_pMonster->Change_Animation(AN_FATAL_START_F - m_iDirCnt - (2 * m_iAnimCnt), false, 0.f);
        }
        else
        {
            m_iAnimCnt = 0;
            m_pMonster->Change_State(CCarcassBigA::GROGY);
        }
    }

}

void CState_CarcassBigA_HitFatal::End_State()
{
    m_fHitFatalTime = 0.f;
}

_bool CState_CarcassBigA_HitFatal::End_Check()
{
    _uint iCurAnim = m_pMonster->Get_CurrentAnimIndex();
    _bool bEndCheck{ false };
    if ((AN_FATAL_START_F) == iCurAnim)
    {
        bEndCheck = m_pMonster->Get_EndAnim(AN_FATAL_START_F);
    }
    else if ((AN_FATAL_START_B) == iCurAnim)
    {
        bEndCheck = m_pMonster->Get_EndAnim(AN_FATAL_START_B);
    }
    else if ((AN_FATAL_LOOP_F) == iCurAnim)
    {
        bEndCheck = m_pMonster->Get_EndAnim(AN_FATAL_LOOP_F);
    }
    else if ((AN_FATAL_LOOP_B) == iCurAnim)
    {
        bEndCheck = m_pMonster->Get_EndAnim(AN_FATAL_LOOP_B);
    }
    else if ((AN_FATAL_END_F) == iCurAnim)
    {
        bEndCheck = m_pMonster->Get_EndAnim(AN_FATAL_END_F);
    }
    else if ((AN_FATAL_END_B) == iCurAnim)
    {
        bEndCheck = m_pMonster->Get_EndAnim(AN_FATAL_END_B);
    }
    else
    {

    }
    //�ִϸ��̼� ��ȣ�� ��ġ���� �ʴ�?��

    return bEndCheck;
}

CState_CarcassBigA_HitFatal* CState_CarcassBigA_HitFatal::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_CarcassBigA_HitFatal* pInstance = new CState_CarcassBigA_HitFatal(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_CarcassBigA_HitFatal"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_CarcassBigA_HitFatal::Free()
{
    __super::Free();
}