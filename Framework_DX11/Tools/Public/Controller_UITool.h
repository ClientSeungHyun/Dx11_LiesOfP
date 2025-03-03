#pragma once

#include "Tools_Defines.h"
#include "Base.h"

#include "VIBuffer_Terrain.h"
#include "GameInstance.h"
#include "UIRender.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Tools)

class CController_UITool : public CBase
{
	DECLARE_SINGLETON(CController_UITool)

public:
	enum class MOVETYPE
	{
		TYPE_STATIC, // 움직이지 않음
		TYPE_MOVE, // 움직임
		TYPE_BAR, // 바 형태로 표시 (HP바 등)
		TYPE_END
	};

	enum class UIPAGE
	{
		PAGE_MAIN,
		PAGE_LOADING,
		PAGE_PLAY,
		PAGE_MENU,
		PAGE_INVEN,
		PAGE_EQUIP,
		PAGE_STAT,
		PAGE_COMMON,
		PAGE_OPTION,
		PAGE_ITEMINFO,
		PAGE_ORTHO,
		PAGE_TEST,
		PAGE_EFFECT,
		PAGE_TALKING,
		PAGE_POPUP,
		PAGE_INFORM,
		PAGE_SHOP,
		PAGE_CHEST,
		PAGE_TELEPOT,
		PAGE_ACHIEVMENT,
		PAGE_TUTORIAL,
		PAGE_END
	};


	typedef struct UIPART_INFO
	{

		~UIPART_INFO() { Safe_Delete_Array(szText); }

		void MakeDirec()
		{
			fDirec = { fAdjust_End.x - fAdjust_Start.x,  fAdjust_End.y - fAdjust_Start.y };
		}

		_float2 GetBarSize()
		{
			if (bBarDirecX == 1)
				return { abs(fAdjust_Start.x - fAdjust_End.x) * fRatio, fSize.y };
			else
				return { fSize.x, abs(fAdjust_Start.y - fAdjust_End.y) * fRatio };
		}

		_float2 MovePart(_float2 fParentPosition)
		{
			if (iMoveType == _int(MOVETYPE::TYPE_BAR))
			{
				fAdjust.x = fAdjust_Start.x + (fDirec.x * fRatio * 0.5f);
				fAdjust.y = fAdjust_Start.y + (fDirec.y * fRatio * 0.5f);
			}
			else if (iMoveType == _int(MOVETYPE::TYPE_MOVE))
			{
				fAdjust.x = fAdjust_Start.x + (fDirec.x * fRatio);
				fAdjust.y = fAdjust_Start.y + (fDirec.y * fRatio);
			}

			fPosition.x = fParentPosition.x + fAdjust.x;
			fPosition.y = fParentPosition.y + fAdjust.y;

			return fPosition;
		}

		_char* strUIPart_Name = {};

		_int iParentPart_Index = -1; // <- 

		_float2 fSize = { 0.f,0.f };
		_float2 fPosition = { 0.f,0.f };

		_int iGroupIndex = 0;

		// 현재 위치
		_float2 fAdjust = { 0.f,0.f }; // <- 중심점(Page의 포지션)에서 어디로 얼마나 떨어져 있는지

		// 이동을 위한 변수
		_int iMoveType = _int(MOVETYPE::TYPE_STATIC);
		_float2 fAdjust_Start = { 0.f,0.f }; // adjust가 움직이는 선의 시작
		_float2 fAdjust_End = { 0.f,0.f }; // adjust가 움직이는 선의 종료
		_float2 fDirec = { 0.f,0.f }; // Start에서 End로 가는 벡터 
		_float fRatio = 0.5f;  // 지금 Adjust가 Start-End 사이 어느 지점에 있는 지 비율로 표시 (0.f ~ 1.f);
		_int bBarDirecX = true; // Combo에 맞추느라 int로 사용함, 클라에서는 bool로 사용

		// 텍스쳐 관련
		_int iTexture_Index = -1;
		_bool bIsItem = false; // <- 아이템 아이콘 표시 여부 
		_float4 fTextureColor = { -1.f,-1.f ,-1.f ,-1.f };

		// 텍스트 관련
		_int iFontIndex = _int(CUIRender::UI_FONT::FONT_END);
		_tchar* szText = {};
		_bool bCenter = false;
		_float4 fTextColor = { 1.f,1.f,1.f,1.f };

		// 작동 제어
		_bool bUpdate = true;
		_bool bRender = true;
		_bool bTurn = false;
		_float fTurn_Degree = 0.f;

		// 쉐이킹
		_float2 fShaking_Adjust = { 0.f,0.f };
		_float2 fShaking_Direc = { 0.f,0.f };
		_float fShaking_Power = 0.f;
		_float fShaking_Interval_Now = 0.f;
		_float fShaking_Interval = 0.f;
		_float fShaking_Time = 0.f;

		// 렌더 관련 제어 변수 
		_bool bTexture_Color_Multiple = false; // <- true인 경우 텍스쳐 컬러 보정값을 원래 값에 곱하는 방식으로 진행
		_float fStrash_Alpha = 0.3f; // 파트 별로 버리는 알파 기준을 다르게 설정해야 하는 경우 사용
		_bool bText_Right = false; // 텍스트를 오른쪽 정렬로 그리는 경우 

		_Vec4 vTexture_Range = { -1.f,-1.f, -1.f, -1.f };
		_Vec2 vTexture_Angle = { 200.f,200.f };

		_bool bIsAlpha_Adjust = false; // rgb 중 가장 낮은 값을 알파로 사용 

		// 다각형 관련 변수 
		_bool bIs_TwoDPolygon = false;
		_int iTwoPolygon_Buffer_Num = 0;
		_float fRatio_TwoDPolygon[8] = { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };




	}UPART;



	typedef struct UIPAGE_INFO
	{
		_char* strUIPage_Name = {};
		_int iNowSelectPart = 0;
		_float2 fPosition = { g_iWinSizeX * 0.5f,g_iWinSizeY * 0.5f }; // 페이지의 포지션
		vector<UPART*> vecPart;
	}UPAGE;



public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	vector<_wstring>& Get_PrototypeTags() { return m_PrototypeTags; }
	_wstring& Get_PrtotypeTagIndex(_uint iIndex) { return m_PrototypeTags[iIndex]; }

	void Add_ProtytypeTag(_wstring strTag) { m_PrototypeTags.emplace_back(strTag); }

	UPART& Get_PartRenderInfo(_int iIndex)
	{
		if ((iIndex >= 0) && (iIndex < m_vecPageInfo[m_iNowSelectNum]->vecPart.size()))
			return *m_vecPageInfo[m_iNowSelectNum]->vecPart[iIndex];

		return UPART{};
	}

	UPART* Get_PartRenderInfo_Effect(_int iIndex)
	{
		if ((iIndex >= 0) && (iIndex < m_vecPageInfo[_int(UIPAGE::PAGE_EFFECT)]->vecPart.size()))
			return m_vecPageInfo[_int(UIPAGE::PAGE_EFFECT)]->vecPart[iIndex];

		return nullptr;
	}

	_int GetPartCount() { return (_int)m_vecPageInfo[m_iNowSelectNum]->vecPart.size(); }

	_int GetBackNum() { return m_iBackground; }


public: // imgui_UI메뉴 전용
	void UITool_Edit();
	HRESULT UITool_Render();

public: // 개발 편의 기능
	// SystemMessage
	void Show_StaticSystemMessage(_wstring Text); // <- Text에 작성한 내용을 최상단에 고정으로 띄운다, Off 함수 안 쓰면 계속 떠 있음, 이미 StaticMessage가 있는 상황에서 함수 다시 쓰면 내용 바뀜
	void Off_StaticSystemMessage(); // <- StaticMessage Off 명령
	void Show_SystemMessage(_wstring Text, _float fTime); // 표시 내용과 노출 시간 설정, 메시지는 한번에 10개까지 노출 가능(Static 포함 10개)


	//MouseInfo
	void Show_MouseInfo(_wstring DataNameA, _float* DataA,
		_wstring DataNameB = TEXT("none"), _float* DataB = nullptr,
		_wstring DataNameC = TEXT("none"), _float* DataC = nullptr,
		_wstring DataNameD = TEXT("none"), _float* DataD = nullptr); // 설정된 내용에 따라 커서 옆에 정보 노출


	//imgui 콘솔 
	void Show_console();


	// UI 연출 관련 
	_bool Get_EffectOn() { return m_bEffectOn; }
	void UpdateEffect(_float fTimeDelta);

	_bool Fade_Out(_wstring strTitle, _wstring strDesc, _Vec3 vColor = _Vec3{ 0.f,0.f,0.f }, _float fTime = 1.f);
	_bool Fade_In(_float fTime = 1.f);

	void Show_Script(_wstring strScript0, _wstring strScript1 = TEXT("none"), _float fTime = 1.f, _Vec3 vColor = _Vec3{ 0.f,0.f,0.f });

	_float Check_Fade() { return m_fTime_Fade_Now / m_fTime_Fade_Max; }


private:
	void UIPage_Edit();
	void UIPart_Edit();

	

	// UI 연출 관련 
private:
	_bool m_bEffectOn = false;

	_float m_fTime_Fade_Now = 0.f;
	_float m_fTime_Fade_Max = 0.f;
	_bool m_bFadeOut = false;
	_float m_fTime_Script_Now = 0.f;
	_float m_fTime_Script_Max = 0.f;


private:
	CController_UITool();
	virtual ~CController_UITool() = default;

private:
	HRESULT InitializeResource();
	HRESULT SavePage();
	HRESULT LoadPage();

	HRESULT SavePart();
	HRESULT LoadPart();
	HRESULT InitializeComponent();

	HRESULT EraseUIData();

	HRESULT MakeClientData_Page();
	HRESULT MakeClientData_Part(HANDLE handle, DWORD* dword, vector<UPART*>* pPart);


private:
	class CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	vector<_wstring>	m_PrototypeTags;

	_int m_iNowSelectNum = 0;



	_int m_iTextureNum = 0;
	_int m_iPageNum = 100;

	_char* m_ArrPageName[_int(UIPAGE::PAGE_END)];

	_char m_InputPageName[100] = "";

	_float2 m_InputPageCenter = { 0.f,0.f };

	list<string> logs;

	_char m_InputText[100] = "";
	_char m_InputPartName[100] = "";

	vector<UPAGE*> m_vecPageInfo;

	CUIRender* m_pUIRender = { nullptr };

	_char* m_szPositionType[3] = { "Static", "Move", "Bar" };
	_char* m_szBarDirec[2] = { "Y", "X" };

	vector<_wstring> m_DataTag_Page;
	vector<_wstring> m_DataTag_Part;

	_int m_iBackground = -1;

public:
	virtual void Free() override;
};

END

