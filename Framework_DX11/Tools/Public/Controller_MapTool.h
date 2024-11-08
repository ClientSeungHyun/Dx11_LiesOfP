#pragma once

#include "Tools_Defines.h"
#include "Base.h"

#include "VIBuffer_Terrain.h"
#include "GameInstance.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Tools)

class CController_MapTool : public CBase
{
	DECLARE_SINGLETON(CController_MapTool)

private:
	CController_MapTool();
	virtual ~CController_MapTool() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Control_Player();
	void Create_Map();
	void Pick_Object();
	void PickPos();
	void Save_Load();

private:
	void Select_Map_Model();
	void Show_List(_uint iFolder);
	void SaveMap();
	void LoadMap();
	void SaveNav();
	void LoadNav();
	void Find_PickObject();
	void Map_Menu();
	void Nav_Menu();
	void Light_Menu();
	void Decal_Menu();
	void Cell_Create_Menu();
	void Cell_Select_Menu();
	void Cell_Select_Point_Menu();
	void Light_Create();
	void Light_Modify();
	void Decal_Create();

	_bool LoadTextureFromMemory(const void* data, size_t data_size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
		_bool LoadTextureFromFile(const char* file_name, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
private:
	class CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	class CNavigationController* m_pNavigationController = { nullptr };

	vector<const char*>	m_FileNames;	//ListBox�� ��� �׸�� �̸�
	vector<const char*>	m_Decal_Folder_Names;	//ListBox�� ��� �׸�� �̸�
	vector<const char*>	m_Decal_File_Names;	//ListBox�� ��� �׸�� �̸�

	_int m_iListCount = { 0 };			//ListBox�׸� ����
	_int m_iListSelectNum = { 0 };		//ListBox�׸��� ������ �ε���
	_int m_iPrePickedCellIndex = { -1 };	//��ŷ���� �� cell index	
	_int m_iSelectedLightIndex = { -1 };	//��ŷ���� �� ���� index	
	_int m_iPreSelectedLightIndex = { -1 };	//��ŷ���� ���� ���� index	
	_int m_iPreSeletImage = { -1 };

	_uint m_iPickObject_ID = { 0 };
	_uint m_iPre_Picked_ID={ 0 };

	class CGameObject* m_pSelectObject = { nullptr };		//���� ������ ��ü
	class CGameObject* m_pPreSelectObject = { nullptr };	//���� �����ߴ� ��ü

	static _float3 m_vScale;	//������Ʈ ũ��
	static _float3 m_vPosition; //������Ʈ ��ǥ
	static _float3 m_vRotation; //������Ʈ ��ǥ

	_float3 m_vPickPos = {}; //��ŷ�� ��ǥ

	using GIZMODESC = struct tagGizmoDesc
	{
		ImGuizmo::MODE CurrentGizmoMode = ImGuizmo::WORLD;
		ImGuizmo::OPERATION CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		bool bUseSnap = false;
		bool boundSizing = false;
		bool boundSizingSnap = false;
		float snap[3] = { 0.1f, 0.1f, 0.1f };
		float bounds[6] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		float boundsSnap[3] = { 0.1f, 0.1f, 0.1f };

	};

	GIZMODESC m_tGizmoDesc;

	ID3D11ShaderResourceView* m_my_texture = NULL;

public:
	virtual void Free() override;
};

END

