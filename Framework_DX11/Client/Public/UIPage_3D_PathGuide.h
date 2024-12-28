#pragma once

#include "Client_Defines.h"
#include "UIPage_3D.h"
#include "GameInstance.h"


/*
[CUIPage_3D_PathGuide]

1. ���̵� ���� UI ��ü ����
2. ���̵� UI ON/OFF
3. �������� �Ǵ� Navi �ε��� ����
4. UI ��ü���� ��ġ ���� 
5. UI ��ü ������Ʈ �� ���� ����



������ ��� 
��� ���
�÷��̾� ��ġ 

1. ���������� �÷��̾���� ���� ����Ʈ ����� 
   1) ��� ������� BFS
   2) �� ���� ���� ���� �������� �Ѿ���� � ������ ��ƾ� �ϴ� �� Ȯ���Ͽ� ����Ʈ�� �߰� 
   3) �� ������ ��� ���� 
2. �÷��̾ �ִ� ������ ���� �� �÷��̾�� ���� ����� �� ã�� 
  ������ ���ڿ������� �̾����� ���� �����ϱ� ���� ���� �� ��� 
   1) ���� ����� �� 1,2�� ã�� 
   2) 1,2�� �� ���� �������� �� ����� �Ͱ� ���� ��ġ ���� 
    -> �̸� ���� ������ �������� ���� �� �� ���� �ڿ� �ִ� ������ ����Ǵ� ���� ���� 
3. �÷��̾ ���� ����� ������ �Ա����� ���� 
4. ����� ����Ʈ�� ���� UI ��ü �׸��� (�ν��Ͻ� �ʿ�)


���� ���°� �ܼ��ϰ� ������ ������ -> DFS�� �� ������ �� 



*/

BEGIN(Engine)

END

BEGIN(Client)

class CUIPage_3D_PathGuide : public CUIPage_3D
{
private:
	CUIPage_3D_PathGuide();
	virtual ~CUIPage_3D_PathGuide() = default;







private:
	vector<list<_int>> m_vecNode_Route; // <- ��� ���
	vector<_int> m_vecDestination; // <- ������ ��ǥ ���





private: // <- ����̽� 
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END