#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include <io.h>

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	_bool isFinished() const {
		return m_isFinished;
	}

public:
	HRESULT Initialize(LEVELID eNextLevelID);
	HRESULT Loading();
	void Draw_LoadingText();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	CGameInstance*				m_pGameInstance = { nullptr };

	LEVELID						m_eNextLevelID = { LEVEL_END };

	HANDLE						m_hThread = {};
	CRITICAL_SECTION			m_CriticalSection = {};
	_bool						m_isFinished = { false };
	_tchar						m_szLoadingText[MAX_PATH] = {};

private:
	HRESULT Ready_Resources_For_LogoLevel();
	HRESULT Ready_Resources_For_GamePlayLevel();

	HRESULT Ready_Resources_For_BossMonster();
	HRESULT Ready_Resources_For_BasicMonster();
	HRESULT Ready_Resources_For_Effect();
	HRESULT Ready_Resources_For_Weapon();

	HRESULT Ready_Resources_GoJunHo();
	HRESULT Ready_Resources_Object();

	HRESULT Ready_Resources_CutScene();

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID);
	virtual void Free() override;
};

END