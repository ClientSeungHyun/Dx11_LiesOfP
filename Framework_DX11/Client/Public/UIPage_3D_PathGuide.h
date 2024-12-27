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
���� ���
�÷��̾� ��ġ 

1. ���������� �÷��̾ �ִ� ���� �Ա����� ���� ����Ʈ ����� 
2. �÷��̾ �ִ� ������ ���� �� �÷��̾�� ���� ����� �� ã�� 
3. �÷��̾ ���� ����� ������ �Ա����� ���� 
4. ����� ����Ʈ�� ���� UI ��ü �׸��� (�ν��Ͻ� �ʿ�)

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
	


private: // <- ����̽� 
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;
};

END