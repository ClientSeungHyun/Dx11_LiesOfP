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
    //FSM_INIT_DESC* pDesc = static_cast<FSM_INIT_DESC*>(pArg);

    return S_OK;
}

HRESULT CState_SimonManusP2_HighJumpFall::Start_State(void* pArg)
{
    m_pMonster->Change_Animation(AN_HIGHJUMPFALL, false, 0.1f, 0);

    m_pMonster->Set_RimLightColor(_Vec4{ 0.9f, 0.f, 0.f, 1.f });
    return S_OK;
}

void CState_SimonManusP2_HighJumpFall::Update(_float fTimeDelta)
{
    m_fCurrentTime += fTimeDelta;

    _double CurTrackPos{};
    CurTrackPos = m_pMonster->Get_CurrentTrackPos();

    if (CurTrackPos >= 200.f && CurTrackPos < 230.f) //���� ���� ���� ü�� + �÷��̾���� ȸ��
    {
        m_vTargetDir = m_pMonster->Get_TargetDir();
        m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_vTargetDir, 2.2f, fTimeDelta);
    }
    else if (CurTrackPos >= 230.f && CurTrackPos <= 245.f) //�� ������
    {
        _Vec3 vPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);

        _Vec3 vMove = m_vTargetDir * (((_float)CurTrackPos - 230.f) / 20.f);
        m_pMonster->Get_RigidBody()->Set_Velocity((vMove - m_vFlyMoveStack) / fTimeDelta);
        m_vFlyMoveStack = vMove;
    }

    if (!m_bResetRim)
    {
        if (CurTrackPos > 245.f)
        {
            m_pMonster->Set_RimLightColor(_Vec4{ 0.f, 0.f, 0.f, 0.f });
            m_bResetRim = true;
        }
    }

    if (End_Check())//�ִϸ��̼��� ���� �޾ƿ����� �ؼ� ������ ����� ������
    {
        m_pMonster->Change_State(CSimonManus::IDLE);
        return;
    }

    Collider_Check();

}

void CState_SimonManusP2_HighJumpFall::End_State()
{
    m_pMonster->Set_RimLightColor(_Vec4{ 0.f, 0.f, 0.f, 0.f });
}

_bool CState_SimonManusP2_HighJumpFall::End_Check()
{
    return m_pMonster->Get_EndAnim(AN_HIGHJUMPFALL);
}

void CState_SimonManusP2_HighJumpFall::Collider_Check()
{
    _double CurTrackPos = m_pMonster->Get_CurrentTrackPos();

    if (CurTrackPos >= 135.f && CurTrackPos <= 150.f)
    {
        m_pMonster->Active_CurrentWeaponCollider(1);
    }
    else
    {
        m_pMonster->DeActive_CurretnWeaponCollider();
    }
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