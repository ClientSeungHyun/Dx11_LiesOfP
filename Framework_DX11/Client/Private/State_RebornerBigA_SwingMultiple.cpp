#include "stdafx.h"
#include "State_RebornerBigA_SwingMultiple.h"
#include "GameInstance.h"
#include "Model.h"
#include "Monster.h"
#include "Camera.h"

#include "RebornerBigA.h"

CState_RebornerBigA_SwingMultiple::CState_RebornerBigA_SwingMultiple(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_RebornerBigA_SwingMultiple::Initialize(_uint iStateNum, void* pArg)
{
    //FSM_INIT_DESC* pDesc = static_cast<FSM_INIT_DESC*>(pArg);

    m_iStateNum = iStateNum;

    return S_OK;
}

HRESULT CState_RebornerBigA_SwingMultiple::Start_State(void* pArg)
{
    m_pMonster->Change_Animation(AN_LINKED_START, false, 0.1f, 0, true);
    m_iRouteTrack = 0;
    m_vRimLightColor = _Vec4(0.f, 0.f, 0.f, 0.5f);

    m_isRimLight = false;
    m_bSwingSound = false;
    m_bSwing = false;

    return S_OK;
}

void CState_RebornerBigA_SwingMultiple::Update(_float fTimeDelta)
{
    _double CurTrackPos = m_pMonster->Get_CurrentTrackPos();

    if (m_iRouteTrack == 0)
    {
        if (CurTrackPos >= 80.f)
        {
            if (m_pMonster->Get_TargetDead())
            {
                m_pMonster->Change_Animation(44, true, 0.5f, 0, true);
                m_pMonster->Change_State(CMonster::IDLE);
                return;
            }

            ++m_iRouteTrack;
            m_bSwingSound = false;
            m_bSwing = false;
            m_pMonster->SetUp_Animation(AN_LINKED_MIDDLE, false, 0, true);
            return;
        }
        if (CurTrackPos <= 45.f)
        {
            m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_pMonster->Get_TargetDir(), 1, fTimeDelta);
        }
    }
    else if (m_iRouteTrack == 1)
    {
        if (CurTrackPos >= 75.f)
        {
            if (m_pMonster->Get_TargetDead())
            {
                m_pMonster->Change_Animation(51, true, 0.5f, 0, true);
                m_pMonster->Change_State(CMonster::IDLE);
                return;
            }

            ++m_iRouteTrack;
            m_bSwingSound = false;
            m_bSwing = false;
            m_isRimLight = true;
            m_pMonster->On_PowerAttack(true);
            m_pMonster->SetUp_Animation(AN_LINKED_LAST, false, 0, true);
            return;
        }

        if ((CurTrackPos <= 20.f) ||
            (CurTrackPos >= 45.f && CurTrackPos <= 53.f))
        {
            m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_pMonster->Get_TargetDir(), 1, fTimeDelta);
        }
    }
    else
    {
        if (End_Check())
        {
            m_pMonster->Change_State(CMonster::IDLE);
            return;
        }

        if ((CurTrackPos <= 31.f) ||
            (CurTrackPos >= 80.f && CurTrackPos <= 140.f) ||
            (CurTrackPos >= 188.f && CurTrackPos <= 223.f))
        {
            m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_pMonster->Get_TargetDir(), 1, fTimeDelta);
        }
    }


    Collider_Check(CurTrackPos);
    Sound_Check(CurTrackPos);
    Effect_Check(CurTrackPos);
    Update_Rimlight(fTimeDelta, CurTrackPos);

}

void CState_RebornerBigA_SwingMultiple::End_State()
{
    m_pMonster->DeActive_CurrentWeaponCollider(0);

    m_vRimLightColor = _Vec4(0.f, 0.f, 0.f, 0.5f);
    m_pMonster->On_PowerAttack(false);
    m_pMonster->Set_RimLightColor(m_vRimLightColor);
}

_bool CState_RebornerBigA_SwingMultiple::End_Check()
{
    return m_pMonster->Get_EndAnim(AN_LINKED_LAST);
}

void CState_RebornerBigA_SwingMultiple::Collider_Check(_double CurTrackPos)
{
    if (m_iRouteTrack == 0)
    {
        if (CurTrackPos >= 55.f && CurTrackPos <= 70.f)
        {
            m_pMonster->Active_CurrentWeaponCollider(0.9f, 0, HIT_TYPE::HIT_METAL, ATTACK_STRENGTH::ATK_WEAK);
        }
        else
        {
            m_pMonster->DeActive_CurrentWeaponCollider(0);
        }
    }
    else if (m_iRouteTrack == 1)
    {
        if ((CurTrackPos >= 20.f && CurTrackPos <= 30.f) ||
            (CurTrackPos >= 45.f && CurTrackPos <= 55.f))
        {
            m_pMonster->Active_CurrentWeaponCollider(0.9f, 0, HIT_TYPE::HIT_METAL, ATTACK_STRENGTH::ATK_WEAK);
        }
        else
        {
            m_pMonster->DeActive_CurrentWeaponCollider(0);
        }
    }
    else
    {
        if ((CurTrackPos >= 37.f && CurTrackPos <= 45.f) ||
            (CurTrackPos >= 142.f && CurTrackPos <= 149.f))
        {
            m_pMonster->Active_CurrentWeaponCollider(1.3f, 0, HIT_TYPE::HIT_METAL, ATTACK_STRENGTH::ATK_NORMAL);
        }
        else
        {
            m_pMonster->DeActive_CurrentWeaponCollider(0);
        }
    }

}

void CState_RebornerBigA_SwingMultiple::Sound_Check(_double CurTrackPos)
{
    if (m_iRouteTrack == 0)
    {
        if (CurTrackPos >= 55.f && CurTrackPos <= 70.f)
        {
            if (!m_bSwingSound)
            {
                m_pMonster->Play_Sound(CPawn::PAWN_SOUND_EFFECT1, TEXT("SE_NPC_SK_WS_Staff_03.wav"), false);
                m_bSwingSound = true;
            }
        }
    }
    else if (m_iRouteTrack == 1)
    {
        if ((CurTrackPos >= 20.f && CurTrackPos <= 30.f) ||
            (CurTrackPos >= 45.f && CurTrackPos <= 55.f))
        {
            if (!m_bSwingSound)
            {
                m_pMonster->Play_Sound(CPawn::PAWN_SOUND_EFFECT1, TEXT("SE_NPC_SK_WS_Staff_03.wav"), false);
                m_bSwingSound = true;
            }
        }
        else
        {
            m_bSwingSound = false;
        }
    }
    else
    {
        if ((CurTrackPos >= 37.f && CurTrackPos <= 45.f) ||
            (CurTrackPos >= 142.f && CurTrackPos <= 149.f))
        {
            if (!m_bSwingSound)
            {
                m_pMonster->Play_Sound(CPawn::PAWN_SOUND_EFFECT1, TEXT("SE_NPC_SK_WS_Staff_03.wav"), false);
                m_bSwingSound = true;
            }
        }
        else
        {
            m_bSwingSound = false;
        }
    }
}

void CState_RebornerBigA_SwingMultiple::Effect_Check(_double CurTrackPos)
{
    if (m_iRouteTrack == 0)
    {
        if (CurTrackPos >= 55.f && CurTrackPos <= 70.f)
        {
            if (!m_bSwing)
            {
                m_pMonster->Active_Effect(0, true);
                m_bSwing = true;
            }
        }
        else
        {
            m_pMonster->DeActive_Effect(0);
        }
    }
    else if (m_iRouteTrack == 1)
    {
        if ((CurTrackPos >= 20.f && CurTrackPos <= 30.f) ||
            (CurTrackPos >= 45.f && CurTrackPos <= 55.f))
        {
            if (!m_bSwing)
            {
                m_pMonster->Active_Effect(0, true);
                m_bSwing = true;
            }
        }
        else
        {
            m_pMonster->DeActive_Effect(0);
            m_bSwing = false;
        }
    }
    else
    {
        if ((CurTrackPos >= 37.f && CurTrackPos <= 45.f) ||
            (CurTrackPos >= 142.f && CurTrackPos <= 149.f))
        {
            if (!m_bSwing)
            {
                m_pMonster->Active_Effect(0, true);
                m_bSwing = true;
            }
        }
        else
        {
            m_pMonster->DeActive_Effect(0);
            m_bSwing = false;
        }
    }
}

void CState_RebornerBigA_SwingMultiple::Update_Rimlight(_float fTimeDelta, _double CurTrackPos)
{
    if (m_isRimLight)
    {
        if (CurTrackPos < 145.f)
        {
            m_vRimLightColor.x = max(m_vRimLightColor.x + 0.5f * fTimeDelta, 1.f);
            m_vRimLightColor.w = max(m_vRimLightColor.w - 0.6f * fTimeDelta, 0.1f);
        }
        else
        {
            m_vRimLightColor.x = max(m_vRimLightColor.x - 0.7f * fTimeDelta, 0.f);
            m_vRimLightColor.w = min(m_vRimLightColor.w + 0.7f * fTimeDelta, 0.5f);
        }

        if (m_vRimLightColor.x == 0.f && m_vRimLightColor.w == 0.5f)
            m_pMonster->On_PowerAttack(false);

        m_pMonster->Set_RimLightColor(m_vRimLightColor);
    }
}

CState_RebornerBigA_SwingMultiple* CState_RebornerBigA_SwingMultiple::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_RebornerBigA_SwingMultiple* pInstance = new CState_RebornerBigA_SwingMultiple(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_RebornerBigA_SwingMultiple"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_RebornerBigA_SwingMultiple::Free()
{
    __super::Free();
}