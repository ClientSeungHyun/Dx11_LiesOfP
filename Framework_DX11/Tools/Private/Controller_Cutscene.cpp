#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "Controller_Cutscene.h"
#include "GameInstance.h"
#include "CutScene.h"
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
        for (int i = 0; i < 8; ++i)
        {
            pCutScene = m_pGameInstance->Get_CloneObject_ToLayer(LEVEL_TOOL, TEXT("Layer_CutScene"), TEXT("Prototype_GameObject_CutScene"));
            if (nullptr != pCutScene)
            {
                m_CutSceneList[i] = static_cast<CCutScene*>(pCutScene);
            }
        }
    }

    m_fCurrentFrame = new float(0.0f);
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

    static float fMaxFrame = { 50.f };

#pragma region �ƽ� ����Ʈ ���
    if (ImGui::CollapsingHeader("Sequence_List"))
    {
        ImGui::PushItemWidth(180);  //������ ����
        if (ImGui::BeginListBox("Name"))
        {
            for (int n = 0; n < 8; n++)
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
        m_fCurrentFrame = m_pCurrentCutScene->Get_CurFrame_Ptr();
        m_bPlay = false;
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
        ImGui::SliderFloat("TimeLine", m_fCurrentFrame, 0.0f, fMaxFrame);
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
            *m_fCurrentFrame = 0.f;
        }
        //��� �ֱ�
        if (ImGui::TreeNode("Add Actors"))
        {
            if (ImGui::Button("Add Camera"))
            {

            }
            if (ImGui::Button("Add UI"))
            {

            }
            if (ImGui::Button("Add Shader"))
            {

            }
            if (ImGui::Button("Add GameObject"))
            {

            }
            if (ImGui::Button("Add Sound"))
            {

            }
            
            ImGui::TreePop();
        }



        //���� ���
        if (m_iPreSelectedCutScene != -1)
        {
            m_pCurrentCutScene->Set_MaxFrame(fMaxFrame);
            m_pCurrentCutScene->Set_Play(m_bPlay);
        }
    }
#pragma endregion
}


void CController_Cutscene::Free()
{
	__super::Free();

   
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
