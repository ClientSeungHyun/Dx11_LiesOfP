#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "Interface_Enums.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

/*
[Item_Manager]
1. ���� 
 0) �������� �⺻ ���� ������ ���� 
 1) �κ��丮 ����
 2) ���� ������ ����
 3) ������ ȹ��, ��� ����

 �� �߰� ���� �ʿ��� ���� 
 �Ʒ��� Ȯ���� �ƴϸ� ���� �ʿ� 
 4) NPC ���� ������ ����
 5) ���� �� ��� ������ ����
 6) ����Ʈ, �̺�Ʈ�� ȹ���ϴ� �������� ���� ����

2. �ν��Ͻ� 
 1) ������ ���� ���� 
 2) �κ� ���� 
 3) ���� ������ ���� 


3. ��� ���� 
 1) �׽�Ʈ �������� ���÷� �ٲ� �� �ִ� ���� �ý���
 2) �ټ��� ������ ������ ������ �����ϵ��� ����

4. ��Ģ
 1) �ű� ������ ���� -> ���� ���� 
 2) ������ �̵� -> ���� ���� �� ���� ������ nullptr ó�� (Delete ���� ���� ����)
 3) �ܺ� ���� -> �ݵ�� ���۷����� 



*/

class CItem_Manager : public CBase
{
public:
	typedef struct ITEM_INFO
	{

	}ITEM;

	typedef struct SLOT_INFO
	{
		SLOT_INFO(_int iSlotNum)
		{
			vecValidType.resize(_int(ITEM_TYPE::TYPE_END));
			vecItemInfo.resize(iSlotNum);
		}

		~SLOT_INFO()
		{
			for (auto& iter : vecItemInfo)
				Safe_Delete(iter);

			vecItemInfo.clear();
			vecValidType.clear();
		}

		vector<ITEM*> vecItemInfo; 
		vector<_bool> vecValidType;
	}SLOT;


public:
	// ����, ����
	ITEM_RESULT AddItem_Inven(ITEM_INDEX eIndex, _int iCount = 1); // <- �κ��� �������� �߰��Ѵ� 
	ITEM_RESULT EquipItem_Inven(ITEM_INDEX eIndex); // <- �κ��� �ִ� �������� ����Ѵ� 




private:
	CItem_Manager(CGameInstance* pGameInstance);
	virtual ~CItem_Manager() = default;

private:
	HRESULT Initialize_Item();

private:
	CGameInstance* m_pGameInstance = { nullptr };

	/*
	�ʿ��� ��
	[Item Array]
	0. ������ �⺻ ����
	
	[Slot Array - Slot]
	0. �κ��丮 
	 1) 

	1. ����
	
	*/

	vector<ITEM*> m_vecItem_BasicSpec; // ���� �� �����ϴ� ��� �������� �⺻ ����
	
	vector<SLOT*> m_vecSlot_Inven; // �κ��丮 ���� ���� 

	vector<_int> m_vecEquip_ItemIndex; // ���� � ��� ���� ���� �� Ȯ�� 






public:
	static CItem_Manager* Create(CGameInstance* pGameInstance);
	virtual void Free() override;

};

END

