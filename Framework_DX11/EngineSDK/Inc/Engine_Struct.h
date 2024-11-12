#pragma once

namespace Engine
{
	typedef struct
	{
		HWND			hWnd;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
		bool			isWindowsed;
	}ENGINE_DESC;

	typedef struct
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_SPOT, TYPE_GODRAY, TYPE_END };

		TYPE		eType;
		XMFLOAT4	vDirection;
		XMFLOAT4	vPosition;
		float		fRange;

		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;
	}LIGHT_DESC;

	typedef struct
	{
		class CTexture* pMaterialTextures[18]{ nullptr };
	}MESH_MATERIAL;

	typedef struct
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vTranslation;
		_double			TrackPosition;
	}KEYFRAME;

	//�̺��� �̺�Ʈ Ű ������
	typedef struct
	{
		enum EVENT_TYPE
		{
			ET_ONCE,
			ET_REPET,
			ET_END
		};
		enum BODY_TYPE_EV
		{
			BT_UPPER,
			BT_EITHER,
			BT_LOWER
		};

		EVENT_TYPE		eEvent_type{ ET_END };
		BODY_TYPE_EV	eEvent_Body_Type{ BT_EITHER };
		_char			szEventName[MAX_PATH];	//Ű������ �̸�(�Ⱦ�����)
		_double			Start_TrackPosition{};	//������ ��ġ
		_double			End_TrackPosition{};	//�ߴ��� ��ġ
		_double			LiveRange{};			//?
		_double			Repet_Duration{};		//�̺�Ʈ�� ����� �ð�(�ߴ��� - ������)

		_uint			iBoneIndex{};			//����Ʈ�� ���� Ȥ�� ������ų ���� �ε���

		_uint			iEffectNum{};			//������ ����Ʈ�� ��ȣ?

	}EVENT_KEYFRAME;

	// ����ȣ ��ƼŬ
	typedef struct
	{
		_float3		vPosition;
		_float2		vPSize;
		_float4		vRight;
		_float4		vUp;
		_float4		vLook;
		_float4		vTranslation;
		_float2		vLifeTime;
		_float4		vColor;
		_float		fSpeed;
		_float4		vCurrenrRandomDir;
		_float4		vNextRandomDir;
	}PARTICLE;

	typedef struct
	{
		_bool			bActiveEffect{ false };

		_uint			iBoneIndex{};			//����Ʈ�� ���� Ȥ�� ������ų ���� �ε���

		_uint			iEffectNum{};			//������ ����Ʈ�� ��ȣ?

		EVENT_KEYFRAME::EVENT_TYPE		eEvent_type{ EVENT_KEYFRAME::ET_END};

		//_double			LiveRange{};			//�̺�Ʈ�� �ð�(�ߴ��� - ������)
		//_double			Repet_Duration{};		//�̺�Ʈ�� ����� �ð�(�ߴ��� - ������)

	}OUTPUT_EVKEY;

	typedef struct
	{
		_uint			iMeshNum;
		_uint			VtxNum;
		_uint			UseWay;//�̳����� �޵��� ������ ���ڷ� ������ ����غ���
	}UFVTX;

	typedef struct
	{
		vector<string>			m_ModelFilePaths;
	}FilePathStruct;

	typedef struct
	{
		vector<FilePathStruct>		pStruct;
	}FilePathStructStack;

	typedef struct
	{
		vector<FilePathStructStack>		pLump;
	}StructStacklump;


	typedef struct
	{
		_uint	iNextAnimIndex;
		_uint	iStartFrame;
		_float	fChangeTime = 0.f;
		_float	fChangeDuration = 0.f;
	}CHANGEANIMATION_DESC;

#pragma region ��ó�� ����
	typedef struct
	{
		_int isOnSSAO;
		_float fRadius;
		_float fBias;
		_float fAmount;
	}SSAO_DESC;
#pragma endregion

	//������Ʈ ���� �� �ҷ����⿡ ���
	typedef struct
	{
		_tchar		szModelTag[MAX_PATH];
		_tchar		szTextureTag_Diffuse[MAX_PATH];
		_tchar		szTextureTag_Normal[MAX_PATH];
		_tchar		szTextureTag_ARM[MAX_PATH];
		_uint		iID;
		_int		iCurrentCellNum;
		_float3		vPosition;
		_float3		vScale;
		_float3		vRotation;
		_bool		isInstance = { false };
		_bool		bShadow = { false };
		_bool		bDecal = { false };
		_bool		bNormal = { false };
		_bool		bARM = { false };
		_bool		bUseWorldColor = { false };

	}OBJECT_DEFAULT_DESC;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;	

		static const unsigned int	iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPOS;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;

		static const unsigned int	iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPOINT;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int	iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];	
	}VTXPOSTEX;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;

		static const unsigned int	iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXCUBETEX;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int	iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXNORTEX;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;		
		XMFLOAT3		vTangent;		

		static const unsigned int	iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXMESH;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndices; /* �� ������ � ��(�ִ� 4�� ���� �ε���)�� ���¸� ���󰡾��ϴ°�? */
		XMFLOAT4		vBlendWeights; /* ������ ������ �װ��� ���� �� �������� ���� �� �ۼ�Ʈ(0.f ~ 1.f)�� ������ �ִ���? */

		static const unsigned int	iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXANIMMESH;


	typedef struct ENGINE_DLL
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;			
		XMFLOAT2		vLifeTime;

		static const unsigned int	iNumElements = 7;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
		
	}VTXRECTINSTANCE;

	typedef struct ENGINE_DLL
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;
		XMFLOAT2		vLifeTime;
		XMFLOAT4		vColor;

		static const unsigned int	iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXPOINTINSTANCE;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3 vCurPos;
		XMFLOAT3 vPrePos;

		_float2 vLifeTime;

		static const unsigned int	iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXTRAIL_ONEPOINT_INSTANCE;

	typedef struct ENGINE_DLL
	{
		XMFLOAT3 vFirstTopPos;
		XMFLOAT3 vFirstBottomPos;

		XMFLOAT3 vSecondTopPos;
		XMFLOAT3 vSecondBottomPos;

		XMFLOAT3 vThirdTopPos;
		XMFLOAT3 vThirdBottomPos;

		XMFLOAT3 vForthTopPos;
		XMFLOAT3 vForthBottomPos;

		_float2 vLifeTime;
		_uint iIndex;

		static const unsigned int	iNumElements = 11;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXTRAIL_TWOPOINT_INSTANCE;

	typedef struct ENGINE_DLL
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;

		static const unsigned int	iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXMODELINSTANCE;

}