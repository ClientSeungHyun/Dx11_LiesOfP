#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

#include "UIPage_Defines.h"
#include "Interface_Enums.h"

// [UIRender_Batching]
// 
// 1. UI ���� ������ �̿�ȭ�ǰ� ���������� ���� �ذ� -> �Ͽ�ȭ 
// 2. ��ġ �������� ��ο� �� �ּ�ȭ (��ǥ 1ȸ) -> ����ȭ 
// 
// ��� 
// 1. ��� UI ��ü�� UIRender_Batching�� ���� ������ ���� 
// 2. UI ������ ���յ� ������ ������ �Է� ������� ó���ϰ� ���� (queue)
// 3. ��Ī ������� �ϳ��� ���ۿ� ��� �ѹ��� ó�� 
//  (��ũ�� ��ü ���� �� �� �� ���� �� �� ����)
// 
// ��Ģ 
// 1. ���� ������ UI ��ġ, ���� ���� � ���� �߰� �������� �ʴ´� (�ݵ�� ������ ����)
//  -> Move,Bar�� ���� ��굵 �� �ؼ� �ѱ� ��
// 2. UIRender_Batching���� ����ü�� �׷��� �� ���׸� ���� �ְ� ���Ŀ��� ������ ��Ī �� ������ �Ѵ� 
//  -> �̸� ���� ���� ������ ����������(���۷��� -> ���� ����) �������� �� ������ �Ѵ� 
//  -> �̿� �Բ� ���� ������� ��ȭ�ǰ� �ִ� UI ���� ����� �����Ͽ� �ڵ带 �ܼ�ȭ �Ѵ� 
//  -> Ư�� ���� ����, ���/�����̻� �ȳ�/�κ��丮 �� �ݺ���/���������� �׷��� �ϴ� ��ü�� ȿ�������� �����Ѵ� 
// 
// 
// 
// 
// 
// 
// 
// 
// 







BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)


class CUIRender_Batching final : public CUIObject
{
public:
	typedef struct UITEXTURE_INFO
	{
		_tchar* strTexturePath = {};
		_tchar* strTextureTag = {};
		CTexture* Texture = { nullptr };
	}UTEXTURE;

	typedef struct UIRENDER_INFO
	{
		// UI ��ü�� ���� 
		_bool bIsItem = false;

		// �׸� ���� 
		_Vec2 vPosition = { 0.f,0.f };
		_Vec2 vSize = { 0.f,0.f };

		_int iTexture = -1;
		_bool bIsMultiple = false;
		_Vec4 vColor_Texture = { 0.f,0.f,0.f,0.f };
		_float fAlpha_Strash = 0.3f;

		TEXT_TYPE eText_Type = TEXT_TYPE::TEXT_END;
		_int iFont = 14;
		_wstring strText = {};
		_Vec4 vColor_Text = { 0.f,0.f,0.f,0.f };

		_float fTurn = 0.f;

		// ��ũ�� ����
		SCROLL_AREA eArea = SCROLL_AREA::SCROLL_END;

	}URENDER;

	enum class UI_SHADER
	{
		SHADER_NORMAL,
		SHADER_CHANGE_COLOR,
		SHADER_MULTIPLE_COLOR,
		SHADER_END
	};



private:
	CUIRender_Batching(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIRender_Batching(const CUIRender_Batching& Prototype);
	virtual ~CUIRender_Batching() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;



public:
	void Input_Render_Info(UIRENDER_INFO* pInfo)
	{
		if (pInfo == nullptr)
			return;

		m_queueRender.push(pInfo);
	}


	HRESULT Make_Texture(_int iTextureIndex);
	HRESULT Make_Texture_Item(_int iTextureIndex);

	HRESULT Render_Part(CUIPage::UPART& pPart, CUIPage& pPage, _bool bTopMove);

	void Set_Scroll_Area(SCROLL_AREA eArea, _Vec2 vPos, _Vec2 vSize);
	void Select_Scroll_Area(SCROLL_AREA eArea) 
	{ 
		m_eNow_Area = eArea;
		m_pContext->RSSetViewports(1, m_vecViewPort[_int(m_eNow_Area)]);
	}

	void Set_Scroll_Y_Offset(SCROLL_AREA eArea, _float fOffset) { m_vecfScrollY_Offset_Max[_int(eArea)] = fOffset; }

public:
	class CShader* m_pShaderCom = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom = { nullptr };



	class CVIBuffer_Rect* m_pVIBufferCom_FullScreen = { nullptr };
	class CVIBuffer_Rect* m_pVIBufferCom_Scroll = { nullptr };
private:
	HRESULT Ready_Components();

	void Ready_Font();
	HRESULT Ready_Texture_UIPart();
	HRESULT Ready_Texture_ItemIcon();

	queue<UIRENDER_INFO*> m_queueRender; // <- �̹� �����ӿ� �׸� ����, 

	vector<UTEXTURE*> m_vecTextureInfo_UIPart;
	vector<UTEXTURE*> m_vecTextureInfo_ItemIcon;
	vector<_tchar*> m_vecFont_tchar;

	vector<class CShader*> m_vecShader_UI;
	vector<D3D11_VIEWPORT*> m_vecViewPort;

	SCROLL_AREA m_eNow_Area = SCROLL_AREA::SCROLL_NONE;
	vector<_float> m_vecfScrollY_Offset_Max;



	// �Ʒ� ������ ���� �ܰ� 

	ID3D11ShaderResourceView* m_Array_UI_Texture[MAX_TEXTURES_UI]; // �ؽ�ó�� SRV �迭
	ID3D11ShaderResourceView* m_Array_Item_Texture[MAX_TEXTURES_ITEM]; // �ؽ�ó�� SRV �迭
	ID3D11SamplerState* m_pSamplerState;

public:
	static CUIRender_Batching* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END