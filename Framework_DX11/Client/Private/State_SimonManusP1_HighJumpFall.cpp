#include "stdafx.h"
#include "State_SimonManusP1_HighJumpFall.h"
#include "GameInstance.h"
#include "Model.h"
#include "SimonManus.h"

CState_SimonManusP1_HighJumpFall::CState_SimonManusP1_HighJumpFall(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_SimonManusP1_HighJumpFall::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    CSimonManus::FSMSTATE_DESC* pDesc = static_cast<CSimonManus::FSMSTATE_DESC*>(pArg);

    return S_OK;
}

HRESULT CState_SimonManusP1_HighJumpFall::Start_State(void* pArg)
{
    m_pMonster->Change_Animation(AN_HIGHJUMPFALL, false, 0.1f, 0);

    return S_OK;
}

void CState_SimonManusP1_HighJumpFall::Update(_float fTimeDelta)
{
    m_fCurrentTime += fTimeDelta;
    
    _double CurTrackPos{};
    CurTrackPos = m_pMonster->Get_CurrentTrackPos();

    if (CurTrackPos >= 200.f && CurTrackPos < 230.f) //���� ���� ���� ü�� + �÷��̾���� ȸ��
    {
        m_vTargetDir = m_pMonster->Get_TargetDir();
        m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_vTargetDir, 2, fTimeDelta);

        m_vTargetDir -= 4 * XMVector3Normalize(m_vTargetDir);
    }
    else if(CurTrackPos >= 230.f && CurTrackPos <= 245.f) //�� ������
    {
        _Vec3 vPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);

        _Vec3 vMove = m_vTargetDir * (((_float)CurTrackPos - 230.f) / 20.f);
        m_pMonster->Get_Transform()->Set_State(CTransform::STATE_POSITION, vPos + vMove - m_vFlyMoveStack);

        m_vFlyMoveStack = vMove;
    }

    if (End_Check())//�ִϸ��̼��� ���� �޾ƿ����� �ؼ� ������ ����� ������
    {
        m_pMonster->Change_State(CSimonManus::IDLE);
        return;
    }

}

void CState_SimonManusP1_HighJumpFall::End_State()
{
    m_iAnimCnt = 0;//Ȥ�� �Ϸ���� �ʰ� ���ϴ� ��쿡 ���
    m_fParalizeTime = 0.f;
    m_vFlyMoveStack = _vector{0, 0, 0, 0};
}

_bool CState_SimonManusP1_HighJumpFall::End_Check()
{
    return m_pMonster->Get_EndAnim(AN_HIGHJUMPFALL);
}

CState_SimonManusP1_HighJumpFall* CState_SimonManusP1_HighJumpFall::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_SimonManusP1_HighJumpFall* pInstance = new CState_SimonManusP1_HighJumpFall(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_SimonManusP1_HighJumpFall"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SimonManusP1_HighJumpFall::Free()
{
    __super::Free();
}