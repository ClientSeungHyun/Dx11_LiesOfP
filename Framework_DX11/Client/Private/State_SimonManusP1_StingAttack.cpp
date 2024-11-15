#include "stdafx.h"
#include "State_SimonManusP1_StingAttack.h"
#include "GameInstance.h"
#include "Model.h"
#include "SimonManusP1.h"

CState_CSimonManusP1_StingAttack::CState_CSimonManusP1_StingAttack(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_CSimonManusP1_StingAttack::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    m_fIdleDuration = 3.3f;
    CSimonManusP1::FSMSTATE_DESC* pDesc = static_cast<CSimonManusP1::FSMSTATE_DESC*>(pArg);

    m_pIsEndAnim = pDesc->pIsEndAnim;
    
    return S_OK;
}

HRESULT CState_CSimonManusP1_StingAttack::Start_State(void* pArg)
{
    m_pMonster->Look_Player();

    m_pMonster->Change_Animation(AN_STINGATTACK, false);

    return S_OK;
}

void CState_CSimonManusP1_StingAttack::Update(_float fTimeDelta)
{
    if (*m_pIsEndAnim == true)//�ִϸ��̼��� ���� �޾ƿ����� �ؼ� ������ ����� ������
    {
        m_pMonster->Change_State(CSimonManusP1::IDLE);
    }

}

void CState_CSimonManusP1_StingAttack::End_State()
{
    m_iAnimCnt = 0;//Ȥ�� �Ϸ���� �ʰ� ���ϴ� ��쿡 ���
    m_fParalizeTime = 0.f;
    *m_pResetRootMove = true;
}

CState_CSimonManusP1_StingAttack* CState_CSimonManusP1_StingAttack::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_CSimonManusP1_StingAttack* pInstance = new CState_CSimonManusP1_StingAttack(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_CSimonManusP1_StingAttack"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_CSimonManusP1_StingAttack::Free()
{
    __super::Free();
}