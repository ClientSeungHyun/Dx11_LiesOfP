#include "stdafx.h"
#include "Controller_AnimationTool.h"
#include "GameInstance.h"

//++
#include "AnimModel.h"
#include "Animation.h"

IMPLEMENT_SINGLETON(CController_AnimationTool)

CController_AnimationTool::CController_AnimationTool()
	:m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CController_AnimationTool::Initialize()
{
	//�ִϸ��̼� �� �ǱԾ�(��ġ�� �̷��κ� �����)
	
	CAnimModel::ANIMMODEL_DESC Desc{};
	Desc.vPosition = { 1.f,5.f,1.f };
	Desc.vScale = { 1.f,1.f,1.f };
	Desc.vRotation = { 0.f,0.f,0.f };
	Desc.pUpdateCtr = &m_bObjRenderCtr;

	strcpy_s(Desc.szModelTag, "Prototype_AnimModel_Test");
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_TOOL, TEXT("Layer_AnimationTool_Test"), TEXT("Prototype_GameObject_Anim"), &Desc)))
		return E_FAIL;

	return S_OK;
}

void CController_AnimationTool::SetUp_AnimTool()
{
	m_bObjRenderCtr = true;
	CModel* pModel = dynamic_cast<CModel*>(m_pGameInstance->Find_Component(LEVEL_TOOL, TEXT("Layer_AnimationTool_Test"), TEXT("Com_Model")));
	if (m_pCopyModelCom == nullptr)
	{
		m_pCopyModelCom = pModel;

	}
	else if (m_pCopyModelCom != pModel)
	{
		m_pCopyModelCom = pModel;

		m_pCopyAnimVec = nullptr;
		m_pCopyBoneVec = nullptr;
	}

	if (m_pCopyModelCom != nullptr)
	{
		if (m_pCopyAnimVec == nullptr)
		{
			m_pCopyAnimVec = &m_pCopyModelCom->Get_Animations();

			m_AnimSpeedPS = (*m_pCopyAnimVec)[m_iSelected_Index_Anim]->Get_SpeedPerSec();
		}

		if (m_pCopyBoneVec == nullptr)
		{
			m_pCopyBoneVec = &m_pCopyModelCom->Get_Bones();
		}
	}
	


	if (ImGui::BeginTabBar("AnimTool"))
	{
		if (ImGui::BeginTabItem("Anim"))
		{
			ListUp_Anim();
			SetUp_Controller_Anim();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Bone"))
		{
			ListUp_Bone();
			SetUp_Controller_Bone();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Vertex"))
		{
			ImGui::EndTabItem();
		}
		ImGui::EndTabItem();
	}
}

void CController_AnimationTool::ListUp_Anim()
{
	//���� ���� ����
	m_iCurSelected_Index_Anim = m_iSelected_Index_Anim;

	ImGui::PushItemWidth(300); // ũ������
	if (ImGui::BeginListBox("Animation List"))
	{
		if (m_pCopyModelCom != nullptr)
		{
			bool is_selected{ false };
			for (int i = 0; i < m_pCopyAnimVec->size(); i++)
			{
				if (m_iSelected_Index_Anim == i)
				{
					is_selected = true;
				}

				if (ImGui::Selectable((*m_pCopyAnimVec)[i]->Get_Name(), is_selected))
				{
					m_iSelected_Index_Anim = i;
				}


				if (!is_selected)
					ImGui::SetItemDefaultFocus();
				is_selected = false;
				// �ݺ������� ����Ʈ�ڽ��� ���õ� ��ü ã��
			}
		}
		
		ImGui::EndListBox();
	}
	//���� ������ �� �� �ִϸ��̼� ��ȯ
	if (m_pCopyModelCom != nullptr)
	{
		if (m_iSelected_Index_Anim != m_iCurSelected_Index_Anim)
		{
			m_pCopyModelCom->SetUp_Animation(m_iSelected_Index_Anim, true);
			m_AnimSpeedPS = (*m_pCopyAnimVec)[m_iSelected_Index_Anim]->Get_SpeedPerSec();
		}
	}

	ImGui::PopItemWidth();

}

void CController_AnimationTool::SetUp_Controller_Anim()
{
	//�ӵ� ����
	ImGui::InputDouble("SpeedPerSec", &m_AnimSpeedPS);

	if (ImGui::Button("Change_SpeedPerSec"))
	{
		if (m_pCopyModelCom != nullptr)
		{
			(*m_pCopyAnimVec)[m_iSelected_Index_Anim]->Set_SpeedPerSec(m_AnimSpeedPS);
		}
	}

	//����Ʈ ����
	if (ImGui::Button("Add_EventKeyFrame"))
	{
		if (m_pCopyModelCom != nullptr)
		{
			//�̺�Ʈ ��� ���� ĳ����Ÿ�� �޴� �ڽ� �����, �ش� �ڽ��� ���� Ű������ �ް� �����, �� �ִϸ��̼��� ��� ���� ��Ű��, Ű������ ������ �ǵ��� �� �Ŀ�
			//EVENT_KEYFRAME Desc;
			//Desc.TrackPosition = ;
			//(*m_pCopyAnimVec)[m_iSelected_Index_Anim]->Add_EventKeyFrame();
		}
	}
	//�ݶ��̴� ����


}

void CController_AnimationTool::ListUp_Bone()
{
	//���� ���� ����
	m_iCurSelected_Index_Bone = m_iSelected_Index_Bone;

	ImGui::PushItemWidth(300); // ũ������
	if (ImGui::BeginListBox("Bone List"))
	{
		if (m_pCopyModelCom != nullptr)
		{
			bool is_selected{ false };
			for (int i = 0; i < m_pCopyBoneVec->size(); i++)
			{
				if (m_iSelected_Index_Bone == i)
				{
					is_selected = true;
				}

				if (ImGui::Selectable((*m_pCopyBoneVec)[i]->Get_Name(), is_selected))
				{
					m_iSelected_Index_Bone = i;
				}

				if (!is_selected)
					ImGui::SetItemDefaultFocus();
				is_selected = false;
				// �ݺ������� ����Ʈ�ڽ��� ���õ� ��ü ã��
			}
		}

		ImGui::EndListBox();
	}
	ImGui::PopItemWidth();

	//�� �̳�
	ImGui::PushItemWidth(100); // ũ������
	if (ImGui::BeginListBox("Bone Type"))
	{
		if (ImGui::Selectable("ROOT", (m_iBoneTypeIndex == 0)))
		{
			m_iBoneTypeIndex = 0;
		}
		else
			ImGui::SetItemDefaultFocus();
		if (ImGui::Selectable("HAND", (m_iBoneTypeIndex == 1)))
		{
			m_iBoneTypeIndex = 1;
		}
		else
			ImGui::SetItemDefaultFocus();
		if (ImGui::Selectable("FOOT", (m_iBoneTypeIndex == 2)))
		{
			m_iBoneTypeIndex = 2;
		}
		else
			ImGui::SetItemDefaultFocus();
		if (ImGui::Selectable("HEAD", (m_iBoneTypeIndex == 3)))
		{
			m_iBoneTypeIndex = 3;
		}
		else
			ImGui::SetItemDefaultFocus();
		if (ImGui::Selectable("WEAPON", (m_iBoneTypeIndex == 4)))
		{
			m_iBoneTypeIndex = 4;
		}
		else
			ImGui::SetItemDefaultFocus();

		ImGui::EndListBox();
	}


	//���� ������ �� �� �ִϸ��̼� ��ȯ
	if (m_pCopyModelCom != nullptr)
	{
		if (m_iSelected_Index_Bone != m_iCurSelected_Index_Bone)
		{
			//���� �ε���, �̸� ǥ��, ���� ���� ����
			
		}
	}

	ImGui::PopItemWidth();
	
}

void CController_AnimationTool::SetUp_Controller_Bone()
{
	_int i  = m_pCopyModelCom->Get_BoneIndex(((*m_pCopyBoneVec)[m_iSelected_Index_Bone])->Get_Name()); // << ���ؿ� �� �̸����� ���� �ε���;


	if (ImGui::Button("Save_BoneIndex"))
	{
		if (m_pCopyModelCom != nullptr)
		{
			//���� Ư�� �����̳ʿ� ����.
			m_pCopyModelCom->Set_UFBIndices(m_iBoneTypeIndex, m_iSelected_Index_Bone);

		}
	}
	ImGui::Text("\n");
	_vector vPos{};
	if (m_pCopyModelCom != nullptr)
	{
		vPos = ((*m_pCopyBoneVec)[m_iSelected_Index_Bone])->Get_TransformationMatrix().r[CTransform::STATE_POSITION];
	}

	ImGui::Text("Pos_X   %d", XMVectorGetX(vPos));
	ImGui::Text("Pos_Y   %d", XMVectorGetY(vPos));
	ImGui::Text("Pos_Z   %d", XMVectorGetZ(vPos));


	ImGui::PushItemWidth(300); // ũ������
	if (ImGui::BeginListBox("USEFULLBONE Vec"))
	{
		if (m_pCopyModelCom != nullptr)
		{
			bool is_selected{ false };
			ImGui::Text("ROOT :: %d", m_pCopyModelCom->Get_UFBIndices(0));
			ImGui::Text("HAND :: %d", m_pCopyModelCom->Get_UFBIndices(1));
			ImGui::Text("FOOT :: %d", m_pCopyModelCom->Get_UFBIndices(2));
			ImGui::Text("HEAD :: %d", m_pCopyModelCom->Get_UFBIndices(3));
			ImGui::Text("WEAPON :: %d", m_pCopyModelCom->Get_UFBIndices(4));
		}

		ImGui::EndListBox();
	}
	ImGui::PopItemWidth();

}

void CController_AnimationTool::Ready_PickModel()
{
}

void CController_AnimationTool::SetUp_Controller_Vertex()
{
}

void CController_AnimationTool::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
