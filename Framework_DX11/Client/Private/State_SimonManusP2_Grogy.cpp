#include "stdafx.h"
#include "State_SimonManusP2_Grogy.h"
#include "GameInstance.h"
#include "Model.h"
#include "SimonManus.h"

CState_SimonManusP2_Grogy::CState_SimonManusP2_Grogy(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_SimonManusP2_Grogy::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    m_fIdleDuration = 3.3f;
    CSimonManus::FSMSTATE_DESC* pDesc = static_cast<CSimonManus::FSMSTATE_DESC*>(pArg);

    m_pIsEndAnim = pDesc->pIsEndAnim;
    m_pResetRootMove = pDesc->pIsResetRootMove;
    m_pTrackPos = pDesc->pGrogyTrackPos;

    return S_OK;
}

HRESULT CState_SimonManusP2_Grogy::Start_State(void* pArg)
{
    if (*m_pTrackPos > 0)
    {
        ++m_iAnimCnt;
        m_pMonster->Change_Animation(AN_GROGY_START - m_iAnimCnt, true, 0.1f, 0);
        m_fGrogyTime = (_float)*m_pTrackPos;
    }
    else
    {
        m_pMonster->Change_Animation(AN_GROGY_START, false, 0.1f, 0);
    }
    *m_pResetRootMove = false;  //�ִϸ��̼��� ���ۺ��� ���� ��Ʈ���� �̵����� �޶����� �ȵ�.

    return S_OK;
}

void CState_SimonManusP2_Grogy::Update(_float fTimeDelta)
{

    switch (m_iAnimCnt)
    {
    case 0:
        if (End_Check())
        {
            ++m_iAnimCnt;
            m_pMonster->Change_Animation(AN_GROGY_START - m_iAnimCnt, true, 0.1f, 0);
        }
        break;

    case 1:
        if (m_fGrogyTime >= m_fGrogyDuration)
        {
            ++m_iAnimCnt;
            m_pMonster->Change_Animation(AN_GROGY_START - m_iAnimCnt, false, 0.1f, 0);
        }
        else
            m_fGrogyTime += fTimeDelta;
        break;

    case 2:
        if (End_Check())
        {
            m_iAnimCnt = 0;
            *m_pTrackPos = 0.f;
            m_pMonster->Change_State(CSimonManus::IDLE);
            return;
        }
        break;

    default:
        break;
    }


    if (KEY_TAP(KEY::X))
    {
        m_pMonster->Change_State(CSimonManus::HITFATAL);
    }

    *m_pTrackPos = m_fGrogyTime;
}

void CState_SimonManusP2_Grogy::End_State()
{
    m_iAnimCnt = 0;//Ȥ�� �Ϸ���� �ʰ� ���ϴ� ��쿡 ���
    m_fGrogyTime = 0;
    *m_pResetRootMove = true;
}

_bool CState_SimonManusP2_Grogy::End_Check()
{
    _uint iCurAnim = m_pMonster->Get_CurrentAnimIndex();
    _bool bEndCheck{ false };

    switch (m_iAnimCnt)
    {
    case 0:
        if ((AN_GROGY_START) == iCurAnim)
        {
            bEndCheck = m_pMonster->Get_EndAnim(AN_GROGY_START);
        }
        break;

    case 1:
        if ((AN_GROGY_LOOP) == iCurAnim)
        {
            bEndCheck = m_pMonster->Get_EndAnim(AN_GROGY_LOOP);
        }
        break;

    case 2:
        if ((AN_GROGY_END) == iCurAnim)
        {
            bEndCheck = m_pMonster->Get_EndAnim(AN_GROGY_END);
        }
        break;

    default:
        break;
    }

    //�ִϸ��̼� ��ȣ�� ��ġ���� �ʴ�?��

    return bEndCheck;
}

CState_SimonManusP2_Grogy* CState_SimonManusP2_Grogy::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_SimonManusP2_Grogy* pInstance = new CState_SimonManusP2_Grogy(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_SimonManusP2_Grogy"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SimonManusP2_Grogy::Free()
{
    __super::Free();
}