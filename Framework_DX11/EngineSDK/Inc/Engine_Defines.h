#pragma once

#pragma warning (disable : 4251)
#pragma warning(disable : 5208) //����ü ��� ����
#pragma warning(disable : 26495) //����ü ��� ����
#pragma warning (disable : 6031) // ReadFile ��ȯ�� ���� ��� ���� 

#include <d3d11.h>
#include <time.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Effects11\d3dx11effect.h"
#include "DirectXTK\DDSTextureLoader.h"
#include "DirectXTK\WICTextureLoader.h"
#include "DirectXTK\ScreenGrab.h"

#include "DirectXTK\PrimitiveBatch.h"
#include "DirectXTK\VertexTypes.h"
#include "DirectXTK\Effects.h"

#include "DirectXTK\SpriteBatch.h"
#include "DirectXTK\SpriteFont.h"

#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <DirectXTK/SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

/* d3dx11�� ���ԵǾ��־����� �ʹ� ���̴�. */
/* ���� d3dx11���ԵǾ��־��� �پ��� ����� �����ϴ� ��� + ���̺귯�� ������ ���������� �߰��Ѵ�. */
//#include <d3dx11.h>

#include <map>
#include <unordered_set>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <unordered_map>

using namespace std;

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"


namespace Engine
{
	const _wstring		g_strTransformTag = TEXT("Com_Transform");
	const _uint			g_iMaxMeshBones = 660;
}

using namespace Engine;

#include "PxPhysics.h"
#include "PxPhysicsAPI.h"
#include "gpu/PxGpu.h"
#include "gpu/PxPhysicsGpu.h"

using namespace physx;

//#include <cuda.h>
//#include <cuda_runtime.h>
//#include <device_launch_parameters.h>

#define TEXTURE_TYPE_MAX 18

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

namespace Engine {
	class CModel;  // CModel�� ���� ������ ����
}

namespace physX
{
	enum GEOMETRY_TYPE { PX_CAPSULE, PX_SPHERE, PX_BOX, PX_MODEL, PX_END };

	struct ENGINE_DLL Physx_Geometry_Desc abstract
	{
	protected:
		Physx_Geometry_Desc(GEOMETRY_TYPE eType)
		{
			m_eGeometryType = eType;
		}

		GEOMETRY_TYPE m_eGeometryType = { PX_END };

	public:
		GEOMETRY_TYPE Get_GeometryType() { return m_eGeometryType; };
	};

	// ĸ�� �ɼ�
	struct ENGINE_DLL GeometryCapsule final : public Physx_Geometry_Desc
	{
		GeometryCapsule()
			: Physx_Geometry_Desc(PX_CAPSULE) {}
		_float fRadius = 0.f;
		_float fHeight = 0.f;
	};

	// �� �ɼ�
	struct ENGINE_DLL GeometrySphere final : public Physx_Geometry_Desc
	{
		GeometrySphere() : Physx_Geometry_Desc(PX_SPHERE) {}
		_float fRadius = 0.f;
	};

	// �ڽ� �ɼ�
	struct ENGINE_DLL GeometryBox final : public Physx_Geometry_Desc
	{
		GeometryBox()
			: Physx_Geometry_Desc(PX_BOX) {}
		_Vec3 vSize = { 0.f,0.f,0.f };
	};
	
	struct ENGINE_DLL GeometryTriangleMesh final : public Physx_Geometry_Desc
	{
		GeometryTriangleMesh()
			: Physx_Geometry_Desc(PX_MODEL), pModel(nullptr) {}
		class CModel* pModel = { nullptr };
	};
}