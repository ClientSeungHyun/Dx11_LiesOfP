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

HRESULT CState_CarcassBigA_Attack::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    m_fIdleDuration = 3.3f;

   FSM_INIT_DESC* pDesc = static_cast<FSM_INIT_DESC*>(pArg);
    
    m_pIsEndAnim = pDesc->pIsEndAnim;
    m_pColliderCtrs = pDesc->pColliderCtrs;

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
        return S_OK;
    }

    m_pMonster->Look_Player();
    m_vAttackDir = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);

    m_iAtkAnimNum = rand();
    m_iAtkAnimNum = rand() % 7;
    if (m_iAtkAnimNum >= 5)
        ++m_iAtkAnimNum;

    if (m_iAtkAnimNum == 5)
    {
        m_pColliderCtrs[CCarcassBigA::TYPE_LEFTHAND] = true;
        m_pColliderCtrs[CCarcassBigA::TYPE_RIGHTHAND] = true;
    }
    else    if (m_iAtkAnimNum <= 6)
    {
        m_pColliderCtrs[CCarcassBigA::TYPE_LEFTHAND] = true;
        m_pColliderCtrs[CCarcassBigA::TYPE_RIGHTHAND] = false;
        //��� ������ �ݶ��̴��� �߰� �ʿ�
    }
    else    if (m_iAtkAnimNum == 7)
    {
        m_pColliderCtrs[CCarcassBigA::TYPE_LEFTHAND] = false;
        m_pColliderCtrs[CCarcassBigA::TYPE_RIGHTHAND] = true;
    }
    else    if (m_iAtkAnimNum <= 8)
    {
        m_pColliderCtrs[CCarcassBigA::TYPE_LEFTHAND] = false;
        m_pColliderCtrs[CCarcassBigA::TYPE_RIGHTHAND] = false;;
    }
    m_pMonster->Change_Animation(m_iAtkAnimNum, true);;
    return S_OK;
}

void CState_CarcassBigA_Attack::Update(_float fTimeDelta)
{
    //�ݶ��̴� ����
    if (*m_pIsEndAnim == true)//�ִϸ��̼��� ���� �޾ƿ����� �ؼ� ������ ����� ������
    {
        m_pMonster->Change_State(CCarcassBigA::IDLE);
    }
    //3ȸ���� �ٸ� ���� �ϰ� �����?
    
}

void CState_CarcassBigA_Attack::End_State()
{
    m_pColliderCtrs[CCarcassBigA::TYPE_LEFTHAND] = true;
    m_pColliderCtrs[CCarcassBigA::TYPE_RIGHTHAND] = true;
}

CState_CarcassBigA_Attack* CState_CarcassBigA_Attack::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_CarcassBigA_Attack* pInstance = new CState_CarcassBigA_Attack(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
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