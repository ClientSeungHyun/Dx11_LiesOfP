#include "stdafx.h"
#include "State_RaxasiaP1_Discharge.h"
#include "GameInstance.h"
#include "Model.h"
#include "Raxasia.h"

#include "AttackObject.h"
#include "Effect_Manager.h"

CState_RaxasiaP1_Discharge::CState_RaxasiaP1_Discharge(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_RaxasiaP1_Discharge::Initialize(_uint iStateNum, void* pArg)
{
    m_iStateNum = iStateNum;
    //CSimonManus::FSMSTATE_DESC* pDesc = static_cast<CSimonManus::FSMSTATE_DESC*>(pArg);

    return S_OK;
}

HRESULT CState_RaxasiaP1_Discharge::Start_State(void* pArg)
{
    m_iRouteTrack = 0;
    m_pMonster->Change_Animation(AN_SWINGDOWN, false, 0.1f, 0);

    m_bResetRim = false;
    m_bControlRim = false;

    m_fGoalRimAlpha = 0.1f;
    m_fCurtRimAlpha = 1.f;

    m_bSwingSound = false;
    m_bChargeActive = false;
    m_bStampBlast = false;

    m_bSwing = false;
    m_bStamp = false;

    return S_OK;
}

void CState_RaxasiaP1_Discharge::Update(_float fTimeDelta)
{
    _double CurTrackPos = m_pMonster->Get_CurrentTrackPos();

    switch (m_iRouteTrack)
    {
    case 0:
        if (CurTrackPos >= 130.f)
        {
            ++m_iRouteTrack;
            m_bSwing = false;
            m_bControlRim = true;
            m_pMonster->Change_Animation(AN_DISCHARGE, false, 0.1f, 0);
            return;
        }

        if (CurTrackPos <= 30.f ||
            CurTrackPos >= 100.f)
        {
            m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_pMonster->Get_TargetDir(), 1.f, fTimeDelta);
        }

        break;

    case 1:
        if (End_Check())
        {
            m_iRouteTrack = 0;
            m_pMonster->Change_State(CRaxasia::IDLE);
            return;
        }

        if (CurTrackPos <= 30.f ||
           (CurTrackPos >= 60.f && CurTrackPos <= 100.f))
        {
            m_pMonster->Get_Transform()->LookAt_Lerp_NoHeight(m_pMonster->Get_TargetDir(), 1.f, fTimeDelta);
        }

        if (!m_bResetRim)
        {
            if (CurTrackPos >= 120.f)
            {
                m_fGoalRimAlpha = 1.f;
                m_bResetRim = true;
            }
        }
        break;

    default:
        break;
    }

    Collider_Check(CurTrackPos);
    Effect_Check(CurTrackPos);
    Control_Sound(CurTrackPos);

    if (m_bControlRim)
    {
        Update_Rimlight();
    }

}

void CState_RaxasiaP1_Discharge::End_State()
{
    m_pMonster->DeActive_Effect(CRaxasia::EFFECT_INCHENTSWORD);
}

_bool CState_RaxasiaP1_Discharge::End_Check()
{
    return m_pMonster->Get_EndAnim(AN_DISCHARGE);
}

void CState_RaxasiaP1_Discharge::Collider_Check(_double CurTrackPos)
{
    if (m_iRouteTrack == 0)
    {
        if ((CurTrackPos >= 35.f && CurTrackPos <= 44.f))
        {
            m_pMonster->Active_CurrentWeaponCollider(1.3f, 0, HIT_TYPE::HIT_METAL, ATTACK_STRENGTH::ATK_NORMAL);
        }
        else
        {
            m_pMonster->DeActive_CurretnWeaponCollider();
        }
    }
    else if (m_iRouteTrack == 1)
    {
        if ((CurTrackPos >= 105.f && CurTrackPos <= 120.f))
        {
            m_pMonster->Active_CurrentWeaponCollider(1.6f, 0, HIT_TYPE::HIT_METAL, ATTACK_STRENGTH::ATK_STRONG);
        }
        else
        {
            m_pMonster->DeActive_CurretnWeaponCollider();
        }
    }
}

void CState_RaxasiaP1_Discharge::Effect_Check(_double CurTrackPos)
{
    if (m_iRouteTrack == 0)
    {
        if (!m_bStamp)
        {
            if (CurTrackPos >= 44.f)
            {
                _float4x4 WorldMat{};
                _Vec3 vPos = { 0.f, 0.f, -1.75f };
                XMStoreFloat4x4(&WorldMat,
                    (*m_pMonster->Get_BoneCombinedMat(m_pMonster->Get_Model()->Get_UFBIndices(UFB_WEAPON))
                        * (m_pMonster->Get_Transform()->Get_WorldMatrix())));
                vPos = XMVector3TransformCoord(vPos, XMLoadFloat4x4(&WorldMat));
                vPos.y = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION).y;

                CEffect_Manager::Get_Instance()->Add_Effect_ToLayer(LEVEL_GAMEPLAY, TEXT("Raxasia_Attack_Stamp"),
                    vPos, _Vec3{ 0.f, 0.f, 1.f });

                m_bStamp = true;
            }
        }
    }
    else
    {
        if (!m_bChargeActive)
        {
            if (CurTrackPos >= 35.f)
            {
                m_pMonster->Active_Effect(CRaxasia::EFFECT_INCHENTSWORD, true);
                m_pMonster->Active_Effect(CRaxasia::EFFECT_THUNDERDISCHARGE, false);
                m_bChargeActive = true;
            }
        }

        if (!m_bStampBlast)
        {
            if (CurTrackPos >= 120.f)
            {
                _float4x4 WorldMat{};
                _Vec3 vPos = { 0.f, 0.f, -1.75f };
                XMStoreFloat4x4(&WorldMat,
                    (*m_pMonster->Get_BoneCombinedMat(m_pMonster->Get_Model()->Get_UFBIndices(UFB_WEAPON))
                        * (m_pMonster->Get_Transform()->Get_WorldMatrix())));
                vPos = XMVector3TransformCoord(vPos, XMLoadFloat4x4(&WorldMat));
                vPos.y = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION).y;

                CEffect_Manager::Get_Instance()->Add_Effect_ToLayer(LEVEL_GAMEPLAY, TEXT("Raxasia_Attack_ThunderStamp_Small"),
                    vPos, _Vec3{ 0.f, 0.f, 1.f });

                //���ÿ�����Ʈ ���� ��ũ �� ����

                CAttackObject::ATKOBJ_DESC Desc;

                _Vec3 vTargetDir = m_pMonster->Get_TargetPos() - vPos;

                Desc.vPos = vPos;
                Desc.vDir = vTargetDir;
                m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Monster_Attack"), TEXT("Prototype_GameObject_ThunderStampMark"), &Desc);

                m_bStampBlast = true;
            }
        }
    }
}

void CState_RaxasiaP1_Discharge::Update_Rimlight()
{
    m_fCurtRimAlpha += (m_fGoalRimAlpha - m_fCurtRimAlpha) / 15;
    m_pMonster->Set_RimLightColor(_Vec4{ 0.9f, 0.f, 0.f, m_fCurtRimAlpha });
    if (abs(m_fGoalRimAlpha - m_fCurtRimAlpha) < 0.1f)
    {
        m_fCurtRimAlpha = m_fGoalRimAlpha;
        if (m_fGoalRimAlpha == 1.f)
        {
            m_pMonster->Set_RimLightColor(_Vec4{ 0.f, 0.f, 0.f, 1.f });
        }
    }
}

void CState_RaxasiaP1_Discharge::Control_Sound(_double CurTrackPos)
{

}

CState_RaxasiaP1_Discharge* CState_RaxasiaP1_Discharge::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_RaxasiaP1_Discharge* pInstance = new CState_RaxasiaP1_Discharge(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_RaxasiaP1_Discharge"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_RaxasiaP1_Discharge::Free()
{
    __super::Free();
}