#pragma once
#include "State.h"

BEGIN(Engine)
class CFsm;
END

BEGIN(Client)

class CState_CarcassBigA_Die :
    public CState
{
private:
    typedef enum {
        AN_DIE_F = 19, AN_DIE_B = 18
    }ANIMNUM;
private:
    typedef enum {
        DIR_BEHIND, DIR_FRONT
    }DIR;

public:
    CState_CarcassBigA_Die(class CFsm* pFsm, class CMonster* pMonster);
    virtual ~CState_CarcassBigA_Die() = default;

public:
    virtual HRESULT Initialize(_uint iStateNum, void* pArg) override;
    virtual HRESULT Start_State(void* pArg = nullptr) override;
    virtual void    Update(_float fTimeDelta) override;
    virtual void    End_State() override;

private:
    class CMonster* m_pMonster = { nullptr };

    _uint               m_iRouteTrack = {};
    _uint               m_iDirCnt = {};

private:
    _bool               End_Check();

public:
    static CState_CarcassBigA_Die* Create(class CFsm* pFsm, class CMonster* pMonster, _uint iStateNum, void* pArg = nullptr);
    virtual void Free() override;

};

END