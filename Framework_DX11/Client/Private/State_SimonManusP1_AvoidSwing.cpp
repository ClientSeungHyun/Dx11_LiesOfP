#include "stdafx.h"
#include "State_SimonManusP1_AvoidSwing.h"
#include "GameInstance.h"
#include "Model.h"
#include "SimonManusP1.h"

CState_SimonManusP1_AvoidSwing::CState_SimonManusP1_AvoidSwing(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_SimonManusP1_AvoidSwing::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    m_fIdleDuration = 3.3f;
    CSimonManusP1::FSMSTATE_DESC* pDesc = static_cast<CSimonManusP1::FSMSTATE_DESC*>(pArg);

    m_pIsEndAnim = pDesc->pIsEndAnim;

    return S_OK;
}

HRESULT CState_SimonManusP1_AvoidSwing::Start_State(void* pArg)
{
    m_pMonster->Look_Player();

    m_pMonster->Change_Animation(AN_STINGATTACK, false);

    return S_OK;
}

void CState_SimonManusP1_AvoidSwing::Update(_float fTimeDelta)
{
    if (*m_pIsEndAnim == true)//�ִϸ��̼��� ���� �޾ƿ����� �ؼ� ������ ����� ������
    {
        m_pMonster->Change_State(CSimonManusP1::IDLE);
    }

}

void CState_SimonManusP1_AvoidSwing::End_State()
{
    m_iAnimCnt = 0;//Ȥ�� �Ϸ���� �ʰ� ���ϴ� ��쿡 ���
    m_fParalizeTime = 0.f;
    *m_pResetRootMove = true;
}

CState_SimonManusP1_AvoidSwing* CState_SimonManusP1_AvoidSwing::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_SimonManusP1_AvoidSwing* pInstance = new CState_SimonManusP1_AvoidSwing(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_SimonManusP1_AvoidSwing"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SimonManusP1_AvoidSwing::Free()
{
    __super::Free();
}