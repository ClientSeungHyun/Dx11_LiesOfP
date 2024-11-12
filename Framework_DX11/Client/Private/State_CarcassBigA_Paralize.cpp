#include "stdafx.h"
#include "State_CarcassBigA_Paralize.h"
#include "GameInstance.h"
#include "Model.h"
#include "CarcassBigA.h"

CState_CarcassBigA_Paralize::CState_CarcassBigA_Paralize(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_CarcassBigA_Paralize::Initialize(_uint iStateNum, void* pArg)
{
   //m_iAnimation_Idle = m_pMonster->Get_Model()->Get_AnimationIndex("Kurama_Idle_Loop");
    m_iStateNum = iStateNum;
    m_fIdleDuration = 3.3f;
    CCarcassBigA::FSMSTATE_DESC* pDesc = static_cast<CCarcassBigA::FSMSTATE_DESC*>(pArg);

    m_pIsEndAnim = pDesc->pIsEndAnim;
    m_pResetRootMove = pDesc->pIsResetRootMove;
    
    return S_OK;
}

HRESULT CState_CarcassBigA_Paralize::Start_State(void* pArg)
{
    m_pMonster->Change_Animation(28, false);
    *m_pResetRootMove = false;//�ִϸ��̼��� ���ۺ��� ���� ��Ʈ���� �̵����� �޶����� �ȵ�.
    m_iAnimCnt = 0.f;

    return S_OK;
}

void CState_CarcassBigA_Paralize::Update(_float fTimeDelta)
{
    switch (m_iAnimCnt)
    {
    case 0:
        if (*m_pIsEndAnim)
        {
            ++m_iAnimCnt;
            m_pMonster->Change_Animation(28 - m_iAnimCnt, true, true);
        }
        break;

    case 1:
        if (m_fParalizeTime >= m_fParalizeDuration)
        {
            ++m_iAnimCnt;
            m_pMonster->Change_Animation(28 - m_iAnimCnt, false, true);
        }
        else
            m_fParalizeTime += fTimeDelta;
        break;

    case 2:
        if (*m_pIsEndAnim)
        {
            m_iAnimCnt = 0.f;
            m_pMonster->Change_State(CCarcassBigA::IDLE);
        }
        break;

    default:
        break;
    }

    
}

void CState_CarcassBigA_Paralize::End_State()
{
    m_iAnimCnt = 0.f;//Ȥ�� �Ϸ���� �ʰ� ���ϴ� ��쿡 ���
    m_fParalizeTime = 0.f;
    *m_pResetRootMove = true;
}

CState_CarcassBigA_Paralize* CState_CarcassBigA_Paralize::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_CarcassBigA_Paralize* pInstance = new CState_CarcassBigA_Paralize(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_CarcassBigA_Paralize"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_CarcassBigA_Paralize::Free()
{
    __super::Free();
}