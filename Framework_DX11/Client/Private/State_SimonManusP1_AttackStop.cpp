#include "stdafx.h"
#include "State_SimonManusP1_AttackStop.h"
#include "GameInstance.h"
#include "Model.h"
#include "SimonManusP1.h"

CState_SimonManusP1_AttackStop::CState_SimonManusP1_AttackStop(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_SimonManusP1_AttackStop::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    m_fIdleDuration = 3.3f;
    CSimonManusP1::FSMSTATE_DESC* pDesc = static_cast<CSimonManusP1::FSMSTATE_DESC*>(pArg);

    m_pIsEndAnim = pDesc->pIsEndAnim;
    return S_OK;
}

HRESULT CState_SimonManusP1_AttackStop::Start_State(void* pArg)
{
    m_pMonster->Change_Animation(AN_ATTACKSTOP, true);

    return S_OK;
}

void CState_SimonManusP1_AttackStop::Update(_float fTimeDelta)
{
    //�ݶ��̴� ����
    if (*m_pIsEndAnim == true)//�ִϸ��̼��� ���� �޾ƿ����� �ؼ� ������ ����� ������
    {
        m_pMonster->Change_State(CSimonManusP1::IDLE);
    }

}

void CState_SimonManusP1_AttackStop::End_State()
{

}

CState_SimonManusP1_AttackStop* CState_SimonManusP1_AttackStop::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_SimonManusP1_AttackStop* pInstance = new CState_SimonManusP1_AttackStop(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_SimonManusP1_AttackStop"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SimonManusP1_AttackStop::Free()
{
    __super::Free();
}