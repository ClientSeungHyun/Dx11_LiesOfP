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

	enum class MOVETYPE
	{
		TYPE_STATIC, // �������� ����
		TYPE_MOVE, // ������
		TYPE_BAR, // �� ���·� ǥ�� (HP�� ��)
		TYPE_END
	};


	typedef struct UIPART_INFO
	{
		
		~UIPART_INFO() { Safe_Delete_Array(szText); }

		void MakeDirec() 
		{
			fDirec = { fAdjust_End.x - fAdjust_Start.x,  fAdjust_End.y - fAdjust_Start.y };
		}

		void MovePart()
		{
			if (iMoveType == _int(MOVETYPE::TYPE_BAR))
			{
				fAdjust.x = fAdjust_Start.x + (fDirec.x * fRatio * 0.5f);
				fAdjust.y = fAdjust_Start.y + (fDirec.y * fRatio * 0.5f);

				if (bBarDirecX)
					fSize.x *= fRatio;
				else
					fSize.y *= fRatio;
			}
			else
			{
				fAdjust.x = fAdjust_Start.x + (fDirec.x * fRatio);
				fAdjust.y = fAdjust_Start.y + (fDirec.y * fRatio);
			}
		}

		_char* strUIPart_Name = {};

		_int iParentPart_Index = -1; // <- 

		_float2 fSize = { 0.f,0.f };
		_float2 fPosition = { 0.f,0.f };

		// ���� ��ġ
		_float2 fAdjust = { 0.f,0.f }; // <- �߽���(Page�� ������)���� ���� �󸶳� ������ �ִ���

		// �̵��� ���� ����
		_int iMoveType = _int(MOVETYPE::TYPE_STATIC);
		_float2 fAdjust_Start = { 0.f,0.f }; // adjust�� �����̴� ���� ����
		_float2 fAdjust_End = { 0.f,0.f }; // adjust�� �����̴� ���� ����
		_float2 fDirec = { 0.f,0.f }; // Start���� End�� ���� ���� 
		_float fRatio = 0.5f;  // ���� Adjust�� Start-End ���� ��� ������ �ִ� �� ������ ǥ�� (0.f ~ 1.f);
		_int bBarDirecX = true; // Combo�� ���ߴ��� int�� �����, Ŭ�󿡼��� bool�� ���

		_int iTexture_Index = -1;

		_int iFontIndex = _int(CUIRender::UI_FONT::FONT_END);
		
		_tchar* szText = {};
		_bool bCenter = false;

		
	}UPART;

	

	typedef struct UIPAGE_INFO
	{
		_char* strUIPage_Name = {};
		_int iPage_Index = -1;

		_float2 fPosition = { g_iWinSizeX * 0.5f,g_iWinSizeY*0.5f }; // �������� ������
		vector<UPART*> vecPart;
	}UPAGE;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	vector<_wstring>& Get_PrototypeTags() { return m_PrototypeTags; }
	_wstring& Get_PrtotypeTagIndex(_uint iIndex) { return m_PrototypeTags[iIndex]; }

	void Add_ProtytypeTag(_wstring strTag) { m_PrototypeTags.emplace_back(strTag); }



public: // imgui ���
	void UITool_Edit(); 
	HRESULT UITool_Render();

private:
	void UIPage_Edit();
	void UIPart_Edit();

	void AddNewPage();

	void UIPart_Render();

	void FontTest();



private:
	CController_UITool();
	virtual ~CController_UITool() = default;

private:
	HRESULT InitializeResource();
	HRESULT SavePart();
	HRESULT LoadPart();
	HRESULT InitializeComponent();


private:
	class CGameInstance* m_pGameInstance = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	vector<_wstring>	m_PrototypeTags;

	_int m_iNowSelectNum = 0;

	_int m_iTextureNum = 0;
	_int m_iPageNum = 100;

	_char* m_ArrPageName[100];

	_char m_InputPageName[100] = "";

	_float2 m_InputPageCenter = { 0.f,0.f };




	_char m_InputText[100] = "";
	_char m_InputPartName[100] = "";

	vector<UPAGE*> m_vecPageInfo;

	CUIRender* m_pUIRender = { nullptr };

	_char* m_szPositionType[3] = { "Static", "Move", "Bar" };
	_char* m_szBarDirec[2] = { "X", "Y" };
	


public:
	virtual void Free() override;
};

END

