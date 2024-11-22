#include "stdafx.h"
#include "State_SimonManusP2_HighJumpFall.h"
#include "GameInstance.h"
#include "Model.h"
#include "SimonManus.h"

CState_SimonManusP2_HighJumpFall::CState_SimonManusP2_HighJumpFall(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_SimonManusP2_HighJumpFall::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    m_fIdleDuration = 3.3f;
    CSimonManus::FSMSTATE_DESC* pDesc = static_cast<CSimonManus::FSMSTATE_DESC*>(pArg);

    return S_OK;
}

HRESULT CState_SimonManusP2_HighJumpFall::Start_State(void* pArg)
{
    m_pMonster->Change_Animation(AN_HIGHJUMPFALL, false, 0.1f, 0);

    return S_OK;
}

void CState_SimonManusP2_HighJumpFall::Update(_float fTimeDelta)
{
    m_fCurrentTime += fTimeDelta;

    _double CurTrackPos{};
    CurTrackPos = m_pMonster->Get_CurrentTrackPos();

    if (CurTrackPos >= 215.f && CurTrackPos < 230.f) //���� ���� ���� ü�� + �÷��̾���� ȸ��
    {
        m_vTargetDir = m_pMonster->Get_TargetDir();
        m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_vTargetDir, 3, fTimeDelta);
    }
    else if (CurTrackPos >= 230.f && CurTrackPos <= 245.f) //�� ������
    {
        _Vec3 vPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);

        _Vec3 vMove = m_vTargetDir * ((CurTrackPos - 230.f) / 15.f);
        m_pMonster->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos + vMove - m_vFlyMoveStack);

        m_vFlyMoveStack = vMove;
    }

    if (End_Check())//�ִϸ��̼��� ���� �޾ƿ����� �ؼ� ������ ����� ������
    {
        m_pMonster->Change_State(CSimonManus::IDLE);
        return;
    }
}

void CState_SimonManusP2_HighJumpFall::End_State()
{

}

_bool CState_SimonManusP2_HighJumpFall::End_Check()
{
    return m_pMonster->Get_EndAnim(AN_HIGHJUMPFALL);
}

CState_SimonManusP2_HighJumpFall* CState_SimonManusP2_HighJumpFall::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_SimonManusP2_HighJumpFall* pInstance = new CState_SimonManusP2_HighJumpFall(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_SimonManusP2_HighJumpFall"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SimonManusP2_HighJumpFall::Free()
{
    __super::Free();
}