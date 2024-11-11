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
[Player_Stat_Manager]
1. ���� 
 1) ���� ���� ���� �ִ�ġ ���� �� ���� 
 2) ������ ���� ���� ��ġ�� ���� 
 3) �ܺο��� �ʿ信 ���� �����Ͽ� ���� ������ �̿� �� ����

2. �߰� ��� ���� 
 1) ����, ����, NPC�� ������ ���� �ʿ��� �� ���� 
 2) ���� �ʿ��� ��� ������ Stat_Manager�� �и��Ͽ� ���� 

3. ���� ��� ���� 
 1) ���� ���� ������ �߰�, ������ �����ϰ� ������ �� �ֵ��� �ڷ� ���� ¥�� 
 2) ����, ������ �����ϰ� �ϵ� ������ ��Ģ,���� ����� �ʵ��� �ϰ��� ���� �ʿ� 
 3) ������ �ʱ� ������ ���� ���� �Ľ��Ͽ� ���� 

*/

class CPlayer_Stat_Manager : public CBase
{
public:
	typedef struct STAT_INFO
	{
		

		_wstring strStat_Name = TEXT("none");
		STAT_TYPE eType = STAT_TYPE::STAT_TYPE_END; // ������ ���� 
		_float fStat_Min_Limit = 0.f; // <- ���� ���Ѽ��� �ּҰ�
		_float fStat_Min = 0.f; // ���� ���� ���Ѽ� 
		_float fStat_Now = 0.f; // ���� ���Ȱ�
		_float fStat_Max = 0.f; // ���� ���� ���Ѽ� 
		_float fStat_Max_Limit = 0.f; // <- ���� ���Ѽ��� �ִ�

		_float fStat_Interval = 0.f; // <- Ÿ���� CELL�� ��� ���, ���� 1ĭ�� ��ġ 



	}STAT;

private:
	CPlayer_Stat_Manager();
	virtual ~CPlayer_Stat_Manager() = default;

public:
	// ���� ���� 
	const _float& Get_Stat(STAT_INDEX eIndex) { return m_vecStat[_int(eIndex)]->fStat_Now; }
	
	//const _float& Get_Stat(STAT_INDEX eIndex) { return m_vecStat[_int(eIndex)]->fStat_Now; }

	// ���� ���� 
	//void Add_Stat(STAT_INDEX eIndex, _float fValue);

private:
	HRESULT Initialize_Stat();



private:
	vector<STAT*> m_vecStat; 


public:
	static CPlayer_Stat_Manager* Create();
	virtual void Free() override;

};

END