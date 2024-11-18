#pragma once

namespace Engine
{
	enum MOUSEKEYSTATE { DIMK_LBUTTON, DIMK_RBUTTON, DIMK_WHEEL, DIMK_END };
	enum MOUSEMOVESTATE { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };

	enum KEY
	{
		LEFT, RIGHT, UP, DOWN,

		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,

		F1, F2, F3, F4, F5, F6, F7, F8, F9,
		NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,

		ALT, CTRL, LSHIFT, SPACE, ENTER, ESC,
		LBUTTON, RBUTTON, TAPKEY,

		LAST,
	};

	enum KEY_STATE
	{
		NONE, // �������ʰ� �������� �ȴ���
		TAP,  // �� ��������
		HOLD, // ������ �ִ�
		AWAY, // �� �� ����

	};

	enum COMPONENT_TYPE
	{
		COLLIDER,
		FSM,
		SOUND,
		TEXTURE,
		TRANSFORM,
		VIBUFFER_CUBE,
		VIBUFFER_RECT,
		VIBUFFER_CELL,
		VIBUFFER_TERRAIN,
		SHADER,
		RIGIDBODY,
		MODEL,
		NAVIGATION,
		COMPONENT_END
	};

	// IOR �� ��
	enum TEXTURE_TYPE
	{
		TEXTURE_NONE = 0,
		DIFFUSE = 1, // BaseColor
		SPECULAR = 2, // Specular 
		AMBIENT = 3,
		EMISSIVE = 4, // Emissive
		HEIGHT = 5,
		NORMALS = 6,  // Normal
		ROUGHNESS = 7,  // ��� �������� Roughness -> ARM		Assimp������ Shiness����
		OPACITY = 8,	// Alpha -> ARH
		DISPLACEMENT = 9,
		LIGHTMAP = 10,
		REFLECTION = 11,
		BASE_COLOR = 12,
		NORMAL_CAMERA = 13,
		EMISSION_COLOR = 14,
		METALNESS = 15,
		DIFFUSE_ROUGHNESS = 16,
		AMBIENT_OCCLUSION = 17,
		UNKNOWN = 18,

		TEXTURE_END
	};

	//�̺��� ������ ��
	enum USEFULLBONE
	{
		UFB_ROOT,
		UFB_HAND_LEFT,
		UFB_HAND_RIGHT,
		UFB_FOOT_LEFT,
		UFB_FOOT_RIGHT,
		UFB_HEAD,
		UFB_WEAPON,
		UFB_BOUNDARY_UPPER,

		UFB_END
	};
}