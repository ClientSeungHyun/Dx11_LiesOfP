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
	HRESULT Initialize();
	HRESULT Control_Player();
	void Create_Map();
	void Select_Obj();
	void EditTransform();
	void ShowPickPos();
	void Save_Load();
private:
	void Select_Map_Model();
	void Show_List(_uint iFolder);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

	vector<const char*>	m_FileNames;	//ListBox�� ��� �׸�� �̸�

	_int m_iListCount = { 0 };			//ListBox�׸� ����
	_int m_iListSelectNum = { 0 };		//ListBox�׸��� ������ �ε���

	class CGameObject* m_pSelectObject = { nullptr };		//���� ������ ��ü
	class CGameObject* m_pPreSelectObject = { nullptr };	//���� �����ߴ� ��ü

	static _float3 m_vScale;	//������Ʈ ũ��
	static _float3 m_vPosition; //������Ʈ ��ǥ
	static _float3 m_vRotation; //������Ʈ ��ǥ

private:
	void SaveMap();
	void LoadMap();

public:
	virtual void Free() override;
};

END

