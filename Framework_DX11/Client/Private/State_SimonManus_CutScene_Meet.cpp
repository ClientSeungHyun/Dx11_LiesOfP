#include "stdafx.h"
#include "State_SimonManus_CutScene_Meet.h"
#include "GameInstance.h"
#include "Model.h"
#include "SimonManus.h"
#include "CutScene.h"

#include "GameInterface_Controller.h"
#include "Effect_Manager.h"

CState_SimonManus_CutScene_Meet::CState_SimonManus_CutScene_Meet(CFsm* pFsm, CMonster* pMonster)
    :CState{ pFsm }
    , m_pMonster{ pMonster }
{
}

HRESULT CState_SimonManus_CutScene_Meet::Initialize(_uint iStateNum, void* pArg)
{
    
    m_iStateNum = iStateNum;
    //CSimonManus::FSMSTATE_DESC* pDesc = static_cast<CSimonManus::FSMSTATE_DESC*>(pArg);

    m_iBreathFrame[0] = 300;
    m_iBreathFrame[1] = 350;
    m_iBreathFrame[2] = 400;


    return S_OK;
}

HRESULT CState_SimonManus_CutScene_Meet::Start_State(void* pArg)
{
    // 모델이 달라서 여기서 해주기
    m_iAnimation_Turn = m_pMonster->Get_Model()->Find_AnimationIndex("AS_MOB_Manus_Turn_Cine", 1.f);
    m_iAnimation_Talk = m_pMonster->Get_Model()->Find_AnimationIndex("AS_MOB_Manus_Talk_Cine", 1.315f);
    m_iAnimation_Hand = m_pMonster->Get_Model()->Find_AnimationIndex("AS_MOB_Manus_Hand_Cine", 1.3159f);
    m_iAnimation_End = m_pMonster->Get_Model()->Find_AnimationIndex("AS_MOB_Manus_End_Cine", 1.315f);

    m_pMonster->Change_Animation(m_iAnimation_Turn, false, 0.f, 0);
    m_pMonster->Get_Model()->Set_SpeedPerSec(m_iAnimation_Turn, 30);

    m_isStartCutScene = false;
    m_fDelay = 0.f;

    return S_OK;
}

void CState_SimonManus_CutScene_Meet::Update(_float fTimeDelta)
{
    _int iFrame = m_pMonster->Get_Frame();
    _uint iCurAnim = m_pMonster->Get_CurrentAnimIndex();

    m_fDelay += fTimeDelta;
    if (m_fDelay >= 13.f && !m_isStartCutScene)
    {
        m_pMonster->Play_Animation();
        m_isStartCutScene = true;
    }
    else if (m_fDelay < 13.f)
    {
        m_pMonster->Stop_Animation();
    }

    _Vec3 vMove = m_pMonster->Get_Model()->Get_BoneCombindTransformationMatrix_Ptr(5)->Translation();
    _float4x4 TransMat;
    XMStoreFloat4x4(&TransMat, m_pMonster->Get_Model()->Get_Bones()[5]->Get_TransformationMatrix());
    TransMat._41 = 0.f;

    m_pMonster->Get_Model()->Get_Bones()[5]->Set_TransformationMatrix(TransMat);;

    m_pMonster->Get_Model()->Update_Bone();

    vMove = XMVector3TransformNormal(vMove, m_pMonster->Get_Transform()->Get_WorldMatrix());

    vMove.y = 0;

    if (iCurAnim != m_iAnimation_End)
    {
        vMove.x = vMove.z = 0.f;
    }
    else
    {
        vMove.x *= 0.3f;
        vMove.z *= 0.3f;
    }

    m_pMonster->Get_RigidBody()->Set_Velocity((vMove - m_vRootMoveStack) / fTimeDelta);

    m_vRootMoveStack = vMove;
   
    if (iCurAnim == m_iAnimation_Hand)
    {
        _int iFrame = m_pMonster->Get_Frame();
        if (iFrame > 255)
        {
            m_pMonster->Active_Weapon();
        }
    }

    Control_Effect(iFrame);
    Control_Dialog(iFrame);
    End_Check(fTimeDelta);
}

void CState_SimonManus_CutScene_Meet::End_State()
{
}

void CState_SimonManus_CutScene_Meet::End_Check(_float fTimeDelta)
{
    _uint iCurAnim = m_pMonster->Get_CurrentAnimIndex();

    if (m_pMonster->Get_EndAnim(m_iAnimation_Turn))
    {
        m_fDelay2 += fTimeDelta;

        if (m_fDelay2 < 0.9f)
            return;

        m_pMonster->Change_Animation(m_iAnimation_Talk, false, 0.1f, 0); 
    }
    else if (m_pMonster->Get_CurrentAnimIndex() == m_iAnimation_Talk && m_pMonster->Get_Model()->Get_CurrentFrame() > 718)
    {
        m_pMonster->Change_Animation(m_iAnimation_Hand, false, 0.2f, 65);
    }
    else if (m_pMonster->Get_CurrentAnimIndex() == m_iAnimation_Hand && m_pMonster->Get_Model()->Get_CurrentFrame() > 607)
    {
        m_pMonster->Change_Animation(m_iAnimation_End, false, 0.f, 64);
    }
    else if (m_pMonster->Get_CurrentAnimIndex() == m_iAnimation_Hand && m_pMonster->Get_Model()->Get_CurrentFrame() > 560)
    {
        m_pMonster->Get_Model()->Set_SpeedPerSec(m_iAnimation_Hand, 11);
    }
    else if (m_pMonster->Get_EndAnim(m_iAnimation_End))
    {
       // m_pMonster->End_CutScene(0);
    }
}

void CState_SimonManus_CutScene_Meet::Control_Dialog(_int iFrame)
{
    _uint iCurAnimIndex = m_pMonster->Get_CurrentAnimIndex();

    if ( !m_isShowDialog[0] && iCurAnimIndex == m_iAnimation_Turn && iFrame >= 1010)
    {
        GET_GAMEINTERFACE->Show_Script(TEXT("보라, 이것이 바로 에르고다. 실로 장엄하지 않은가!"), TEXT("none"), 22.5f);
        m_isShowDialog[0] = true;
    }
    else  if (!m_isShowDialog[1] && iCurAnimIndex == m_iAnimation_Talk && iFrame >= 245)
    {
        GET_GAMEINTERFACE->Show_Script(TEXT("불멸하는 존재, 거짓없는 세계를 이루는 열쇠지"), TEXT("none"), 20.5f);
        m_isShowDialog[1] = true;
    }
    else  if (!m_isShowDialog[2] && iCurAnimIndex == m_iAnimation_Talk && iFrame >= 570)
    {
        GET_GAMEINTERFACE->Show_Script(TEXT("이제 그 진화의 문을 열 때가 되었다."), TEXT("none"), 12.f);
        m_isShowDialog[2] = true;
    }
    else  if (!m_isShowDialog[3] && iCurAnimIndex == m_iAnimation_Hand && iFrame >= 255)
    {
        GET_GAMEINTERFACE->Show_Script(TEXT("감사하라! 이것이 신조차 삼킬 자, 나 시몬 마누스가 주는 선물이다."), TEXT("none"), 26.f);
        m_isShowDialog[3] = true;
    }
    else  if (!m_isShowDialog[4] && iCurAnimIndex == m_iAnimation_End && iFrame >= 100)
    {
        GET_GAMEINTERFACE->Show_Script(TEXT("이제 태어날 너희의 신을 경배하라!"), TEXT("none"), 18.f);
        m_isShowDialog[4] = true;
    }
}

void CState_SimonManus_CutScene_Meet::Control_Effect(_int iFrame)
{
    _uint iCurAnim = m_pMonster->Get_CurrentAnimIndex();
    if (iCurAnim == m_iAnimation_Turn)
    {
        for (_uint i = 0; i < 3; ++i)
        {
            if (!m_isBreath[i] && iFrame >= m_iBreathFrame[i])
            {
                _Vec3 vCurrentPos = m_pMonster->Calc_BoneWorldPos("Manus_facial_52");
                vCurrentPos.y += 0.05f;

                CEffect_Manager::Get_Instance()->Add_Effect_ToLayer_Rot(LEVEL_GAMEPLAY, TEXT("SimonManus_CutScene_Blow"), vCurrentPos);

                m_isBreath[i] = true;
            }
        }

        if (!m_isSpit[0] && iFrame >= m_iBreathFrame[0])
        {
            _Vec3 vCurrentPos = m_pMonster->Calc_BoneWorldPos("Manus_facial_52");

            // loop인데 아닌걸로 고치기
            CEffect_Manager::Get_Instance()->Add_Effect_ToLayer_Rot(LEVEL_GAMEPLAY, TEXT("SimonManus_CutScene_Spit"), vCurrentPos);

            m_isSpit[0] = true;
        }
    }
    else if (iCurAnim == m_iAnimation_Hand)
    {
        if (!m_isKneeDown && iFrame >= 81)
        {
            _Vec3 vCurrentPos = m_pMonster->Calc_BoneWorldPos("Bn_R_Knee");
            vCurrentPos.y -= 0.1f;

            CEffect_Manager::Get_Instance()->Add_Effect_ToLayer_Rot(LEVEL_GAMEPLAY, TEXT("SimonManus_CutScene_SmallSmoke"), vCurrentPos);

            m_isKneeDown = true;
        }
        else if (!m_isFoot[0] && iFrame >= 598)
        {
            _Vec3 vCurrentPos = m_pMonster->Calc_BoneWorldPos("Bip001-R-Toe0");

            CEffect_Manager::Get_Instance()->Add_Effect_ToLayer_Rot(LEVEL_GAMEPLAY, TEXT("SimonManus_CutScene_SmallSmoke"), vCurrentPos);

            m_isFoot[0] = true;
        }
    }
    else if (iCurAnim == m_iAnimation_End)
    {
        if (!m_isFoot[1] && iFrame >= 111)
        {
            _Vec3 vCurrentPos = m_pMonster->Calc_BoneWorldPos("Bip001-L-Toe0");

            CEffect_Manager::Get_Instance()->Add_Effect_ToLayer_Rot(LEVEL_GAMEPLAY, TEXT("SimonManus_CutScene_SmallSmoke"), vCurrentPos);

            m_isFoot[1] = true;
        }
        else if (!m_isFoot[2] && iFrame >= 160)
        {
            _Vec3 vCurrentPos = m_pMonster->Calc_BoneWorldPos("Bip001-R-Toe0");

            CEffect_Manager::Get_Instance()->Add_Effect_ToLayer_Rot(LEVEL_GAMEPLAY, TEXT("SimonManus_CutScene_SmallSmoke"), vCurrentPos);

            m_isFoot[2] = true;
        }

    }

}

CState_SimonManus_CutScene_Meet* CState_SimonManus_CutScene_Meet::Create(CFsm* pFsm, CMonster* pMonster, _uint iStateNum, void* pArg)
{
    CState_SimonManus_CutScene_Meet* pInstance = new CState_SimonManus_CutScene_Meet(pFsm, pMonster);

    if (FAILED(pInstance->Initialize(iStateNum, pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CState_SimonManus_CutScene_Meet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SimonManus_CutScene_Meet::Free()
{
    __super::Free();
}