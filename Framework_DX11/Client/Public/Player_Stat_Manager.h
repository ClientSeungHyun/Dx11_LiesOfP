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
		_float fStat_Now = 0.f; // ���� ���Ȱ�
		_float fStat_Max = 0.f; // ���� ���� ���Ѽ� 
		_float fStat_Max_Limit = 0.f; // <- ���� ���Ѽ��� �ִ�

		_float fStat_Interval = 0.f; // <- Ÿ���� CELL�� ��� ���, ���� 1ĭ�� ��ġ 



	}STAT;

	typedef struct BUFF_INFO
	{
		_wstring strBuff_Name = TEXT("none");
		_int iTexture_Index = 0;
		_Vec4 vTexture_Color = { 0.f,0.f,0.f,0.f };
		_float fLifeTime_Now = 0.f;
		_float fLifeTime_Max = 0.f;
	}BUFF;

private:
	CPlayer_Stat_Manager(CGameInstance* pGameInstance);
	virtual ~CPlayer_Stat_Manager() = default;

public:
	// ������Ʈ 
	void Update_Stat(_float fTimeDelta);

	// ���� ���� 
	const _float& Get_NowStat_Normal(STAT_NORMAL eIndex) { return m_vecStat_Normal[_int(eIndex)]->fStat_Now; }
	const STAT& Get_StatInfo_Normal(STAT_NORMAL eIndex) { return *m_vecStat_Normal[_int(eIndex)]; }
	const _float Get_Now_Max_Ratio(STAT_NORMAL eIndex) { return m_vecStat_Normal[_int(eIndex)]->fStat_Now / m_vecStat_Normal[_int(eIndex)]->fStat_Max; }
	const _float Get_Max_Limit_Ratio(STAT_NORMAL eIndex) { return m_vecStat_Normal[_int(eIndex)]->fStat_Max / m_vecStat_Normal[_int(eIndex)]->fStat_Max_Limit; }
	const _float Get_Now_Limit_Ratio(STAT_NORMAL eIndex) { return m_vecStat_Normal[_int(eIndex)]->fStat_Now / m_vecStat_Normal[_int(eIndex)]->fStat_Max_Limit; }

	const STAT& Get_StatInfo_DEF(STAT_DEF eIndex) { return *m_vecStat_DEF[_int(eIndex)]; }

	_bool Get_Buff(BUFF_INDEX eIndex) { return _bool(m_vecBuff[_int(eIndex)]->fLifeTime_Now); }
	_float Get_Buff_Ratio(BUFF_INDEX eIndex) { return (m_vecBuff[_int(eIndex)]->fLifeTime_Now / m_vecBuff[_int(eIndex)]->fLifeTime_Max); }
	const BUFF* Get_Buff_Info(BUFF_INDEX eIndex) { return m_vecBuff[_int(eIndex)]; }
	void Start_Buff(BUFF_INDEX eIndex) { m_vecBuff[_int(eIndex)]->fLifeTime_Now = m_vecBuff[_int(eIndex)]->fLifeTime_Max; }

	// ���� ���� 
	void Add_Stat_Normal(STAT_NORMAL eIndex, _float fValue);
	void Add_StatMax_Normal(STAT_NORMAL eIndex, _float fValue);

	


private:
	HRESULT Initialize_Stat();



private:
	CGameInstance* m_pGameInstance = { nullptr };
	vector<STAT*> m_vecStat_Normal; 
	vector<STAT*> m_vecStat_DEF;
	vector<BUFF*> m_vecBuff;
	_int m_iLevel = 0;
	_int m_iErgo_NextLevel = 100;

	




public:
	static CPlayer_Stat_Manager* Create(CGameInstance* pGameInstance);
	virtual void Free() override;

};

END