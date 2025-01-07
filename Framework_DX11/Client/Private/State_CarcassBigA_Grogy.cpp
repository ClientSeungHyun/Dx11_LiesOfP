#include "stdafx.h"
#include "State_CarcassBigA_Grogy.h"
#include "GameInstance.h"
#include "Model.h"
#include "CarcassBigA.h"

CState_CarcassBigA_Grogy::CState_CarcassBigA_Grogy(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_CarcassBigA_Grogy::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    FSM_INIT_DESC* pDesc = static_cast<FSM_INIT_DESC*>(pArg);

    return S_OK;
}

HRESULT CState_CarcassBigA_Grogy::Start_State(void* pArg)
{
    m_pMonster->Change_Animation(AN_GROGY_START, false);//�ִϸ��̼��� ���ۺ��� ���� ��Ʈ���� �̵����� �޶����� �ȵ�.

    return S_OK;
}

void CState_CarcassBigA_Grogy::Update(_float fTimeDelta)
{
    switch (m_iAnimCnt)
    {
    case 0:
        if (End_Check())
        {
            ++m_iAnimCnt;
            m_pMonster->Change_Animation(AN_GROGY_LOOP, true, 0.f);
        }
        break;

    case 1:
        if (m_fGrogyTime >= m_fGrogyDuration)
        {
            ++m_iAnimCnt;
            m_pMonster->Change_Animation(AN_GROGY_END, false, 0.f);
        }
        else
            m_fGrogyTime += fTimeDelta;
        break;

    case 2:
        if (End_Check())
        {
            m_iAnimCnt = 0;
            m_pMonster->Change_State(CCarcassBigA::IDLE);
        }
        break;

    default:
        break;
    }

}

void CState_CarcassBigA_Grogy::End_State()
{
    m_pMonster->Reset_GroggyPoint();
    m_iAnimCnt = 0;//Ȥ�� �Ϸ���� �ʰ� ���ϴ� ��쿡 ���
}

_bool CState_CarcassBigA_Grogy::End_Check()
{
    _uint iCurAnim = m_pMonster->Get_CurrentAnimIndex();
    _bool bEndCheck{ false };
    if ((AN_GROGY_START) == iCurAnim)
    {
        bEndCheck = m_pMonster->Get_EndAnim(AN_GROGY_START);
    }
    else if ((AN_GROGY_LOOP) == iCurAnim)
    {
        bEndCheck = m_pMonster->Get_EndAnim(AN_GROGY_LOOP);
    }
    else if ((AN_GROGY_END) == iCurAnim)
    {
        bEndCheck = m_pMonster->Get_EndAnim(AN_GROGY_END);
    }
    else
    {

    }
    //�ִϸ��̼� ��ȣ�� ��ġ���� �ʴ�?��

    return bEndCheck;
}

CState_CarcassBigA_Grogy* CState_CarcassBigA_Grogy::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_CarcassBigA_Grogy* pInstance = new CState_CarcassBigA_Grogy(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_CarcassBigA_Grogy"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_CarcassBigA_Grogy::Free()
{
    __super::Free();
}