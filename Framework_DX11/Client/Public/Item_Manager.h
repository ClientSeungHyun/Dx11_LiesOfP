#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "Stat_Enum.h"

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





*/

class CItem_Manager : public CBase
{
public:
	typedef struct ITEM_INFO
	{

	}ITEM;

private:
	CItem_Manager(CGameInstance* pGameInstance);
	virtual ~CItem_Manager() = default;

private:
	HRESULT Initialize_Item();

private:
	CGameInstance* m_pGameInstance = { nullptr };



public:
	static CItem_Manager* Create(CGameInstance* pGameInstance);
	virtual void Free() override;

};

END

