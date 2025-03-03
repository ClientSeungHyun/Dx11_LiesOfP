#pragma once
#include "State.h"

BEGIN(Engine)
class CFsm;
END

BEGIN(Client)

class CState_Player_Scissor_Fable2 : public CState
{
public:
    CState_Player_Scissor_Fable2(class CFsm* pFsm, class CPlayer* pPlayer);
    virtual ~CState_Player_Scissor_Fable2() = default;

public:
    virtual HRESULT Initialize(_uint iStateNum, void* pArg) override;
    virtual HRESULT Start_State(void* pArg = nullptr) override;
    virtual void    Update(_float fTimeDelta) override;
    virtual void    End_State() override;

private:
    class CPlayer* m_pPlayer = { nullptr };

    _uint               m_iAnimation_ScissorCombos3 = { };

    _int                m_iComboIndex = {};

    _int                m_iChangeFrame = {};
    _int                m_iSeperateFrame = {};
    _int                m_iCombineFrame = {};

    _bool               m_isChangeAnim = {};
    _bool               m_isInputLButton = {};
    _bool               m_isInputRButton = {};
    _float              m_fRButtonTime = {};


    _double* m_pTrackPos = { nullptr };

private:
    _bool               End_Check();

private:
    _int                m_iColliderStartFrame_Left[2] = {};
    _int                m_iColliderEndFrame_Left[2] = {};
    _int                m_iColliderStartFrame_Right[2] = {};
    _int                m_iColliderEndFrame_Right[2] = {};
    _int                m_iColliderStartFrame = {};
    _int                m_iColliderEndFrame = {};

    void                Control_Collider();

private:
    _int                m_iEffectSeperateStartFrame = {};
    _int                m_iEffectSeperateEndFrame = {};
    _int                m_iEffectCombineStartFrame = {};
    _int                m_iEffectCombineEndFrame = {};
    _bool               m_isActiveSeperateEffect = {};
    _bool               m_isActiveCombineEffect = {};
    _bool               m_isActiveFableEffect[2] = {};

    void                Control_Effect(_int iFrame);

    _int                m_iSoundFrame = {};
    _int                m_iSoundFrameArray[3] = {};
    _bool               m_isSoundPlay = {};
    _bool               m_isSoundPlayArray[3] = {false,};

public:
    static CState_Player_Scissor_Fable2* Create(class CFsm* pFsm, class CPlayer* pPlayer, _uint iStateNum, void* pArg = nullptr);
    virtual void Free() override;

};

END