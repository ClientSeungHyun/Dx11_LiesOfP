#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "Controller_Cutscene.h"
#include "Controller_UITool.h"
#include "GameInstance.h"
#include "CutScene.h"
#include "Camera.h"
#include "AnimModel.h"
#include <io.h>
#include<fstream>

IMPLEMENT_SINGLETON(CController_Cutscene)


CController_Cutscene::CController_Cutscene()
	:m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CController_Cutscene::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

    if (m_CutSceneList[0] == nullptr)
    {
        CGameObject* pCutScene = nullptr;
        for (_uint i = 0; i < m_iCutSceneCount; ++i)
        {
            pCutScene = m_pGameInstance->Get_CloneObject_ToLayer(LEVEL_TOOL, TEXT("Layer_CutScene"), TEXT("Prototype_GameObject_CutScene"));
            if (nullptr != pCutScene)
            {
                m_CutSceneList[i] = static_cast<CCutScene*>(pCutScene);
            }
        }
    }

    m_fTrackPosition = &m_fTrackPosition_Zero;
    m_pCamera = m_pGameInstance->Find_Camera(LEVEL_TOOL);

	return S_OK;
}

void CController_Cutscene::Update(_float fTimeDelta)
{
    Menu();

    if (m_iPreSelectedCutScene != -1)
    {
        m_pCurrentCutScene->Update(fTimeDelta);
    }

}

void CController_Cutscene::Menu()
{
    static int item_selected_idx = -1; // Here we store our selected data as an index.

    static bool item_highlight = false;
    int item_highlighted_idx = 0; // Here we store our highlighted data as an index.

    static float fMaxFrame = { 0.f };
    static _int iSelectedKeyframeNum = -1;

#pragma region �ƽ� ����Ʈ ���
    if (ImGui::CollapsingHeader("Sequence_List"))
    {
        ImGui::PushItemWidth(180);  //������ ����
        if (ImGui::BeginListBox("Name"))
        {
            for (_uint n = 0; n < m_iCutSceneCount; n++)
            {
                const bool is_selected = (item_selected_idx == n);
                if (ImGui::Selectable(m_CutSceneNameList[n], is_selected))
                    item_selected_idx = n;

                if (item_highlight && ImGui::IsItemHovered())
                    item_highlighted_idx = n;
                                           
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }
    }

#pragma endregion

    //���ο� �ƽ� ����
    if(m_iPreSelectedCutScene != item_selected_idx)
    {
        m_iPreSelectedCutScene = item_selected_idx;
        m_pCurrentCutScene = m_CutSceneList[item_selected_idx];
        fMaxFrame = m_pCurrentCutScene->Get_MaxFrame();
        m_fTrackPosition = m_pCurrentCutScene->Get_CurFrame_Ptr();
        m_bPlay = false;
        iSelectedKeyframeNum = -1;
    }

#pragma region �ƽ� ���λ��� ����
    if (ImGui::CollapsingHeader("CutScene Detail"))
    {
        //�ƽ� �̸�
        ImGui::Text("Name : "); 
        if (m_iPreSelectedCutScene != -1)
        {
            ImGui::SameLine();
            ImGui::Text(m_CutSceneNameList[item_selected_idx]);
        }
        //�ִ� ������ ����
        ImGui::PushItemWidth(150);   //������ ����
        ImGui::InputFloat("Frame Max", &fMaxFrame, 1.f);
        ImGui::PopItemWidth();  //������ ���� Ǯ��
        //������ �� ����
        ImGui::SeparatorText("Frame");
      
        if (ImGui::Button("Play"))
        {
            m_bPlay = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            m_bPlay = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("ReSet Frame"))
        {
            *m_fTrackPosition = 0.f;
            m_pCurrentCutScene->Keyframe_Actived_Reset();
            m_pCamera->Reset_Zoom();
            m_pCamera->Reset_MoveLerp();
            m_pCamera->Stop_Turn();
        }

        ImGui::SliderFloat("TimeLine", m_fTrackPosition, 0.0f, fMaxFrame);

        //���� ���
        if (m_iPreSelectedCutScene != -1)
        {
            m_pCurrentCutScene->Set_MaxFrame(fMaxFrame);
            m_pCurrentCutScene->Set_Play(m_bPlay);
        }

        //Ű������
        ImGui::SeparatorText("Key Frame Detail");
        if (ImGui::Button("Create KeyFrame") && m_pCurrentCutScene != nullptr)
        {
            m_pCurrentCutScene->Create_KeyFrame();
        }
        //Ű������ ����Ʈ
        ImGui::BeginGroup();
        ImGui::Text("[KeyFrame_List]");

        ImGui::BeginChild("KeyFrame List", ImVec2(150, 150), true); //�׸� ĭ �����

        static _int iKeyFrameCount = 0;
        if(m_pCurrentCutScene!=nullptr)
            iKeyFrameCount = m_pCurrentCutScene->Get_KeyFrameCount();

        for (_int i = 0; i < iKeyFrameCount; i++)
        {
            _char szText[MAX_PATH] = "KeyFrmae ";
            _char szNum[MAX_PATH];
            _char szTime[MAX_PATH];
            sprintf_s(szNum, "%d : ", i);
            sprintf_s(szTime, "%f", m_pCurrentCutScene->Get_Selected_KeyFrame(i)->fTrackPosition);
            strcat_s(szText, szNum);
            strcat_s(szText, szTime);

            if (ImGui::Selectable(szText, iSelectedKeyframeNum == i))
            {
                iSelectedKeyframeNum = i;
            }
        }
        
        if (m_iPreSelected_KeyFrame != iSelectedKeyframeNum) //���� �� ���
        {
            m_iPreSelected_KeyFrame = iSelectedKeyframeNum;
            if(iSelectedKeyframeNum != -1)
            {
                CUTSCENE_KEYFRAME_DESC* pDesc = m_pCurrentCutScene->Get_Selected_KeyFrame(iSelectedKeyframeNum);
                pCutScene_Desc = pDesc;
                *m_fTrackPosition = pDesc->fTrackPosition;
            }
            else
            {
                *m_fTrackPosition = m_fTrackPosition_Zero;
                pCutScene_Desc = nullptr;
            }
        }

        ImGui::EndChild();
        ImGui::EndGroup();

        ImGui::SameLine();

        //Ű������ �� ��� ���
        ImGui::BeginGroup();
        ImGui::Text("[Actor_List]");
       
        ImGui::BeginChild("Actor List", ImVec2(110, 90), true);

        static int selected = -1;
        for (int n = 0; n < CCutScene::TYPE_END; n++)
        {
            char buf[32];
            sprintf_s(buf, m_ActorTypeNameList[n]);
            if (ImGui::Selectable(buf, selected == n))
                selected = n;
        }
        ImGui::EndChild(); 
        //���� ��ư
        if (ImGui::Button("Delete Selected KeyFrame"))
        {
            m_pCurrentCutScene->Delete_Selected_Keyframe(iSelectedKeyframeNum);
            iSelectedKeyframeNum = -1;
        }
        //TrackPosition ���� üũ�ڽ�
        static bool bChangeTrackPos = false;
        ImGui::Checkbox("Change TrackPosition", &bChangeTrackPos);
        if (bChangeTrackPos)
        {
            if (iSelectedKeyframeNum == -1)
                bChangeTrackPos = false;
            else
            {
                m_pCurrentCutScene->Get_Selected_KeyFrame(iSelectedKeyframeNum)->fTrackPosition = *m_fTrackPosition;
                m_pCurrentCutScene->Sort_KeyFrame_TrackPosition();
            }
        }
        ImGui::EndGroup();

        //��Һ� �޴� â

        if(m_pCurrentCutScene != nullptr && iSelectedKeyframeNum != -1)
        {
            switch (selected)
            {
            case CCutScene::CAMERA:
                Camera_Memu();
                break;
            case CCutScene::UI:
                UI_Memu();
                break;
            case CCutScene::SHADER:
                Shader_Memu();
                break;
            case CCutScene::GAMEOBJECT:
                GamgeObject_Memu();
                break;
            case CCutScene::SOUND:
                Sound_Memu();
                break;
            default:
                break;
            }
        }
   
    }
#pragma endregion

#pragma region ����, �ҷ�����
    ImGui::SeparatorText("Save/Load");
    if(ImGui::Button("Save"))
    {
        Save();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        Load();
    }
    if (ImGui::Button("Load Old"))
    {
        Load();
    }
#pragma endregion
}

void CController_Cutscene::Camera_Memu()
{
    static bool bShowCurCamState = { false };
    ImGui::Checkbox("Show CurCamState", &bShowCurCamState);
    if (bShowCurCamState)
        Show_CurCamState();

    //ī�޶� �̵�
    ImGui::Checkbox("Teleport", &pCutScene_Desc->Camera_Desc.bTeleport);
    if (pCutScene_Desc->Camera_Desc.bTeleport)
    {
        if (ImGui::Button("Save Current CamMatrix"))
        {
            pCutScene_Desc->Camera_Desc.mCameraWorlMatrix = m_pGameInstance->Find_Camera(LEVEL_TOOL)->Get_Transform()->Get_WorldMatrix();
        }
    }
    //ī�޶� ���� �̵�
    ImGui::Checkbox("MoveLerp", &pCutScene_Desc->Camera_Desc.bLerpMove);
    if (pCutScene_Desc->Camera_Desc.bLerpMove)
    {
        ImGui::DragFloat3("TargetPos", (_float*)&pCutScene_Desc->Camera_Desc.vTargetPos, 0.1f);
        ImGui::DragFloat("Move Speed", &pCutScene_Desc->Camera_Desc.fMoveSpeed, 0.01f, 0.f);
    }
    //ī�޶� ȸ��
    ImGui::Checkbox("Turn", &pCutScene_Desc->Camera_Desc.bTurn);
    if (pCutScene_Desc->Camera_Desc.bTurn)
    {
        ImGui::DragFloat3("PitchYawRoll", (_float*)&pCutScene_Desc->Camera_Desc.vPitchTawRoll, 0.1f,0.f,360.f);
        ImGui::DragFloat("Turn Speed", &pCutScene_Desc->Camera_Desc.fTurn_Speed, 0.1f,0.f,10.f);
    }
    //����
    ImGui::Checkbox("ZoomnIn", &pCutScene_Desc->Camera_Desc.bZoomIn);
    if (pCutScene_Desc->Camera_Desc.bZoomIn)
    {
        ImGui::DragFloat("Fovy(Degree)", &pCutScene_Desc->Camera_Desc.fFovy, 0.01f, 0.01f, 2.f);
        ImGui::DragFloat("Duration", &pCutScene_Desc->Camera_Desc.fZoomDuration, 0.05f, 0.f, 10.f);
    }
    //�ܾƿ�
    ImGui::Checkbox("ZoomnOut", &pCutScene_Desc->Camera_Desc.bZoomOut);
    if (pCutScene_Desc->Camera_Desc.bZoomOut)
    {
        ImGui::DragFloat("Duration", &pCutScene_Desc->Camera_Desc.fZoomDuration, 0.1f, 0.f, 10.f);
    }
    //����ŷ
    ImGui::Checkbox("Shake", &pCutScene_Desc->Camera_Desc.bShake);
    if (pCutScene_Desc->Camera_Desc.bShake)
    {
        ImGui::DragFloat("ShakeDuration", &pCutScene_Desc->Camera_Desc.fShakeDuration, 0.1f, 0.f);
        ImGui::DragFloat("ShakePower", &pCutScene_Desc->Camera_Desc.fShakePower, 0.1f, 0.f);
    }
}

void CController_Cutscene::UI_Memu()
{
    //FadeOut ���
    ImGui::Checkbox("FadeOut", &pCutScene_Desc->UI_DESC.bFadeOut);
    ImGui::SameLine();
    //FadeIn ���
    ImGui::Checkbox("FadeIn", &pCutScene_Desc->UI_DESC.bFadeIn);
    //Fade �� ����
    static ImVec4 color = ImVec4(0.f,0.f,0.f,1.f);
    ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_NoOptions;
    ImGui::ColorEdit3("MyColor##1", (float*)&pCutScene_Desc->UI_DESC.fColor, misc_flags);
    //Fade �ӵ�
    ImGui::DragFloat("Fade Time", &pCutScene_Desc->UI_DESC.fTime,0.1f, 0.f);
}

void CController_Cutscene::Shader_Memu()
{
    //Dof��� ����
    ImGui::Checkbox("Use Dof", &pCutScene_Desc->ShaderDesc.bUseDof); ImGui::SameLine();
    ImGui::Checkbox("Inverse Dof", &pCutScene_Desc->ShaderDesc.bUseDof_Inverse);
    //Dof��ġ ���� ����
    ImGui::Checkbox("Increase", &pCutScene_Desc->ShaderDesc.bDof_Increase); ImGui::SameLine();
    ImGui::Checkbox("Decrease", &pCutScene_Desc->ShaderDesc.bDof_Decrease);
    //Dof ��� ��ġ
    ImGui::DragFloat("DOF", &pCutScene_Desc->ShaderDesc.fDof, 0.02f, 0.f);
    ImGui::DragFloat("Speed", &pCutScene_Desc->ShaderDesc.fSpeed, 0.02f, 0.f, 10.f, "%4f");
}

void CController_Cutscene::GamgeObject_Memu()
{
    //������Ʈ ��� ����
    ImGui::Checkbox("Player", &pCutScene_Desc->Obj_Desc.bUseObj[0]); ImGui::SameLine();
    ImGui::Checkbox("Boss1", &pCutScene_Desc->Obj_Desc.bUseObj[1]); ImGui::SameLine();
    ImGui::Checkbox("Boss2", &pCutScene_Desc->Obj_Desc.bUseObj[2]);


    if(pCutScene_Desc->Obj_Desc.bUseObj[0])
    {
        ImGui::Text("Player");
        //���� enum ����
        ImGui::InputInt("Player State Num", (int*)&pCutScene_Desc->Obj_Desc.iStateNum[0]);
        ImGui::InputFloat("Player Anim Speed Ratio ", &pCutScene_Desc->Obj_Desc.fAnimSpeedRatio[0]);
    }

    if (pCutScene_Desc->Obj_Desc.bUseObj[1])
    {
        ImGui::Text("Boss1");
        //���� enum ����
        ImGui::InputInt("Boss1 State Num", (int*)&pCutScene_Desc->Obj_Desc.iStateNum[1]);
        ImGui::InputFloat("Boss1 Anim Speed Ratio ", &pCutScene_Desc->Obj_Desc.fAnimSpeedRatio[1]);
    }

    if (pCutScene_Desc->Obj_Desc.bUseObj[2])
    {
        ImGui::Text("Boss2");
        //���� enum ����
        ImGui::InputInt("Boss2 State Num", (int*)&pCutScene_Desc->Obj_Desc.iStateNum[2]);
        ImGui::InputFloat("Boss2 Speed Ratio ", &pCutScene_Desc->Obj_Desc.fAnimSpeedRatio[2]);
    }
    
}

void CController_Cutscene::Sound_Memu()
{
    //BGM ���߱�
    ImGui::Checkbox("Stop BGM", &pCutScene_Desc->Sound_Desc.bStopBGM);
    //BGM �ٲٱ�
    ImGui::Checkbox("Change BGM", &pCutScene_Desc->Sound_Desc.bChangeBGM);
}

void CController_Cutscene::Show_CurCamState()
{
    ImGui::Begin("CurrCam_State", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar
        | ImGuiWindowFlags_AlwaysVerticalScrollbar);

    ImGui::Text("Ori Fov : %f", XMConvertToRadians(60.f));
    ImGui::DragFloat("FoV", m_pCamera->Get_FoV_Ptr(), 0.01f, 0.01f, 2.f);
    
    ////////////////////////////
    _Vec4 CurrentQuat = XMQuaternionRotationMatrix(m_pCamera->Get_Transform()->Get_WorldMatrix());

    // ���ʹϾ� -> ���
    XMMATRIX rotMatrix = XMMatrixRotationQuaternion(CurrentQuat);

    // ��� -> Euler �� (Yaw, Pitch, Roll)
    float pitch, yaw, roll;
    pitch = std::atan2(rotMatrix.r[1].m128_f32[2], rotMatrix.r[2].m128_f32[2]);
    yaw = std::asin(-rotMatrix.r[0].m128_f32[2]);
    roll = std::atan2(rotMatrix.r[0].m128_f32[1], rotMatrix.r[0].m128_f32[0]);

    //Euler�� Degree��
    ImGui::Text("Rot : %f, %f, %f", XMConvertToDegrees( pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll));
    
    _Vec3 vCurPos = m_pCamera->Get_Transform()->Get_State(CTransform::STATE_POSITION);
    ImGui::Text("Pos: %f, %f, %f", vCurPos.x, vCurPos.y, vCurPos.z);

    static _bool bTurnCamera = { false };
    //ȸ�� �����ϱ�
    ImGui::Checkbox("Turn Camera", &bTurnCamera);

    if (bTurnCamera)
    {
        static _float vNewTurn[3] = {};
        ImGui::DragFloat3("Camera Test Turn: ", vNewTurn);
        m_pCamera->Get_Transform()->Turn_RollPitchYaw_Lerp(vNewTurn[0], vNewTurn[1], vNewTurn[2], 1.f, 1.f );
    }

    ImGui::End();
}

void CController_Cutscene::Save()
{
    const char cFile[128] = "../Bin/DataFiles/CutScene_Data.dat";
    ofstream fout(cFile, ios::out | ios::binary);

    //	fout.open();
    if (!fout.is_open())    // ���� �����ٸ�
    {
        MSG_BOX(TEXT("���� ���⸦ ����"));
        return;
    }

    //��ü �ƽ� �� ����
    fout.write(reinterpret_cast<const char*>(&m_iCutSceneCount), sizeof(_uint));

    for (_uint i = 0; i < m_iCutSceneCount; ++i)
    {
        //�ƽ� �ε���
        fout.write(reinterpret_cast<const char*>(&i), sizeof(_uint));

        //�ƽ��� ��ü ������ (�ð�)
        _float fMaxFrame = m_CutSceneList[i]->Get_MaxFrame();
        fout.write(reinterpret_cast<const char*>(&fMaxFrame), sizeof(_float));

        //�ƽ��� KeyFrame����(�̺�Ʈ ����)
        _int iKeyFrameCount = m_CutSceneList[i]->Get_KeyFrameCount();
        fout.write(reinterpret_cast<const char*>(&iKeyFrameCount), sizeof(_int));

        for (_int j = 0; j < iKeyFrameCount; ++j)
        {
            fout.write(reinterpret_cast<const char*>(m_CutSceneList[i]->Get_Selected_KeyFrame(j)), sizeof(CUTSCENE_KEYFRAME_DESC));
        }
    }

    fout.close();
    MSG_BOX(TEXT("���� ���⸦ ����"));
}

void CController_Cutscene::Load()
{
    const char cFile[128] = "../Bin/DataFiles/CutScene_Data.dat";
    ifstream fin(cFile, ios::in | ios::binary);

    //	fin.open("../Bin/Map_Data.txt");
    if (!fin.is_open())    // ���� �����ٸ�
    {
        MSG_BOX(TEXT("���� �б⸦ �����߾��.."));
        return;
    }

    _uint CutSceneCount = { 0 };
    fin.read(reinterpret_cast<char*>(&CutSceneCount), sizeof(_uint));

    for(_uint i = 0 ; i < CutSceneCount; ++i)
    {
        //�ƽ� �ε���
        _uint iCutScene_Index;
        fin.read(reinterpret_cast<char*>(&iCutScene_Index), sizeof(_uint));

        //�ƽ��� ��ü ������ (�ð�)
        _float fMaxFrame;
        fin.read(reinterpret_cast<char*>(&fMaxFrame), sizeof(_float));
        m_CutSceneList[i]->Set_MaxFrame(fMaxFrame);

        //�ƽ��� KeyFrame����(�̺�Ʈ ����)
        _int iKeyFrameCount;
        fin.read(reinterpret_cast<char*>(&iKeyFrameCount), sizeof(_int));

        for (int j = 0; j < iKeyFrameCount; ++j)
        {
            CUTSCENE_KEYFRAME_DESC pDesc = {};
            fin.read(reinterpret_cast<char*>( &pDesc), sizeof(CUTSCENE_KEYFRAME_DESC));
            m_CutSceneList[i]->Load_KeyFrame(pDesc);
        }
    }

    fin.close();
    MSG_BOX(TEXT("���� �б⸦ �����߽��ϴ�.."));
}

void CController_Cutscene::Load_Old()
{
    const char cFile[128] = "../Bin/DataFiles/CutScene_Data.dat";
    ifstream fin(cFile, ios::in | ios::binary);

    //	fin.open("../Bin/Map_Data.txt");
    if (!fin.is_open())    // ���� �����ٸ�
    {
        MSG_BOX(TEXT("���� �б⸦ �����߾��.."));
        return;
    }

    _uint CutSceneCount = { 0 };
    fin.read(reinterpret_cast<char*>(&CutSceneCount), sizeof(_uint));

    for (_uint i = 0; i < CutSceneCount; ++i)
    {
        //�ƽ� �ε���
        _uint iCutScene_Index;
        fin.read(reinterpret_cast<char*>(&iCutScene_Index), sizeof(_uint));

        //�ƽ��� ��ü ������ (�ð�)
        _float fMaxFrame;
        fin.read(reinterpret_cast<char*>(&fMaxFrame), sizeof(_float));
        m_CutSceneList[i]->Set_MaxFrame(fMaxFrame);

        //�ƽ��� KeyFrame����(�̺�Ʈ ����)
        _int iKeyFrameCount;
        fin.read(reinterpret_cast<char*>(&iKeyFrameCount), sizeof(_int));

        for (int j = 0; j < iKeyFrameCount; ++j)
        {
            CUTSCENE_KEYFRAME_DESC pDesc = {};
            fin.read(reinterpret_cast<char*>(&pDesc), sizeof(CUTSCENE_KEYFRAME_DESC));
            m_CutSceneList[i]->Load_KeyFrame(pDesc);
        }
    }

    fin.close();
    MSG_BOX(TEXT("���� �б⸦ �����߽��ϴ�.."));
}


void CController_Cutscene::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
