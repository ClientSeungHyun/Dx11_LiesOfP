#include "stdafx.h"
#include "State_CarcassBigA_Attack.h"
#include "GameInstance.h"
#include "Model.h"
#include "CarcassBigA.h"

CState_CarcassBigA_Attack::CState_CarcassBigA_Attack(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_CarcassBigA_Attack::Initialize(_uint iStateNum)
{
   //m_iAnimation_Idle = m_pMonster->Get_Model()->Get_AnimationIndex("Kurama_Idle_Loop");
    m_iStateNum = iStateNum;
    m_fIdleDuration = 3.3f;

    return S_OK;
}

HRESULT CState_CarcassBigA_Attack::Start_State(void* pArg)
{
    if (m_iAttackCnt < 3)
    {
        ++m_iAttackCnt;
    }
    else
    {
        m_iAttackCnt = 0;

        m_pMonster->Change_State(CCarcassBigA::IMPACT);
    }

    m_pMonster->Look_Player();
    m_vAttackDir = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);

    m_iAtkAnimNum = rand();
    m_iAtkAnimNum = rand() % 7;
    if (m_iAtkAnimNum >= 5)
        ++m_iAtkAnimNum;

    m_pMonster->Change_Animation(m_iAtkAnimNum, true);;
    return S_OK;
}

void CState_CarcassBigA_Attack::Update(_float fTimeDelta)
{
    m_fCurrentTime += fTimeDelta;
    //�ݶ��̴� ����
    int a = 0;
    if (m_fCurrentTime >= 10.f)//�ִϸ��̼��� ���� �޾ƿ����� �ؼ� ������ ����� ������
    {
        m_pMonster->Change_State(CCarcassBigA::IDLE);
    }
    //3ȸ���� �ٸ� ���� �ϰ� �����?
    
}

void CState_CarcassBigA_Attack::End_State()
{
    m_fCurrentTime = 0.f;
}

CState_CarcassBigA_Attack* CState_CarcassBigA_Attack::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum)
{
    CState_CarcassBigA_Attack* pInstance = new CState_CarcassBigA_Attack(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_CarcassBigA_Attack"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_CarcassBigA_Attack::Free()
{
    __super::Free();
}