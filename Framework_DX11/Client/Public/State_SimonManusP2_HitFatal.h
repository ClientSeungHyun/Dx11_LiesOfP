#pragma once
#include "State.h"

BEGIN(Engine)
class CFsm;
END

BEGIN(Client)

class CState_SimonManusP2_HitFatal : public CState
{
private:
    typedef enum {
        AN_FATAL_START = 3, AN_FATAL_LOOP = 2, AN_FATAL_END = 1
    }ANIMNUM;

public:
    CState_SimonManusP2_HitFatal(class CFsm* pFsm, class CMonster* pMonster);
    virtual ~CState_SimonManusP2_HitFatal() = default;

public:
    virtual HRESULT Initialize(_uint iStateNum, void* pArg) override;
    virtual HRESULT Start_State(void* pArg = nullptr) override;
    virtual void    Update(_float fTimeDelta) override;
    virtual void    End_State() override;

private:
    class CMonster* m_pMonster = { nullptr };

    _float              m_fHitFatalTime = { 0.f };
    _float              m_fHitFatalDuration = { 3.f };

    _uint               m_iAnimCnt = {};

    _uint               m_iDirCnt = {};

private:
    _bool               End_Check();

    _bool* m_pFatalAttacked = { nullptr };
public:
    static CState_SimonManusP2_HitFatal* Create(class CFsm* pFsm, class CMonster* pMonster, _uint iStateNum, void* pArg = nullptr);
    virtual void Free() override;

};

END