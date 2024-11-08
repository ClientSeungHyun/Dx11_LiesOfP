#include "Shader_Engine_Defines.hlsli"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ViewMatrixInv, g_ProjMatrixInv;
matrix			g_LightViewMatrix, g_LightProjMatrix;
matrix			g_StaticLightViewMatrix;
texture2D		g_Texture;

Texture2DArray	g_CascadeTextureArr;
matrix			g_CascadeViewMatrix[3], g_CascadeProjMatrix[3];

vector			g_vLightDir;
vector			g_vLightPos;
float			g_fLightRange;

vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;
texture2D		g_NormalTexture;

texture2D		g_PriorityTexture;
texture2D		g_DiffuseTexture;
texture2D		g_ARMTexture;
vector			g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
texture2D		g_ShadeTexture;
texture2D		g_DepthTexture;
texture2D		g_SpecularTexture;
texture2D		g_LightDepthTexture;
texture2D		g_BackTexture;
texture2D		g_FinalTexture;
texture2D		g_BlurXTexture;
texture2D		g_BlurYTexture;

texture2D		g_BloomTexture;
texture2D		g_CascadeShadowTexture;

vector			g_vCamPosition;


float ComputeShadow(float4 vPosition, int iCascadeIndex, float4 vNormalDesc)
{
	//    [unroll] ���� ��������??
	
    // ��ȯ ��� ���
    float4 vLightProjPos = mul(vPosition, g_CascadeViewMatrix[iCascadeIndex]);
    vLightProjPos = mul(vLightProjPos, g_CascadeProjMatrix[iCascadeIndex]);
    vLightProjPos = vLightProjPos / vLightProjPos.w;
    
	// ������ ��ġ�� �ؽ��� ��ǥ�� ��ȯ
    float2 vTextCoord;
    vTextCoord.x = vLightProjPos.x * 0.5f + 0.5f;
    vTextCoord.y = vLightProjPos.y * -0.5f + 0.5f;

    // �׸��� ���� ���� 
    float fNormalOffset = 0.0001f;
    float fDot = saturate(dot(normalize(g_vLightDir.xyz) * -1.f, vNormalDesc.xyz));
    float fBias = max((fNormalOffset * 5.0f) * (1.0f - (fDot * -1.0f)), fNormalOffset);
    
	// �� ���̿� �־����
    if (vLightProjPos.z > 1.f || vLightProjPos.z < 0.f)
    {
        return 1.f;
    }
    
    float fShadowPower = 1.f;
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            vector vLightDepth = g_CascadeTextureArr.SampleCmpLevelZero(DepthComparisonSampler, float3(vTextCoord, iCascadeIndex), vLightProjPos.z, int2(x, y));
            if (vLightProjPos.z - fBias > vLightDepth.x)
            {
                fShadowPower += 0.3f;
            }
            else
            {
                fShadowPower += 1.f;
            }
        }
    }
    return fShadowPower / 9.0f;
}

struct VS_IN
{
	float3 vPosition : POSITION;	
	float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4 vPosition : SV_POSITION;	
	float2 vTexcoord : TEXCOORD0;
};


VS_OUT VS_MAIN(/*����*/VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	/* ������ ��ġ�� ���� �� ������ȯ�Ѵ�.*/		
	/* Ŭ���̾�Ʈ���� ���������� �����ϴ� TrnasformCoord�Լ��ʹ� �ٸ��� */
	/* mul�Լ��� ��쿡�� �����ϰ� ����� ���ϱ⸸ ������ �ϰ� w�����⿬����ü�� �������� �ʴ´�. */
	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;	
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

/* Triangle : ���� ������ �� vs_main�� ����Ҷ����� ��� */
/* ������ ��δ� ����Ǹ�. ���� ������ ����. */
/* ���ϵ� ������ w�� ������ xyzw�� ������. ���� */
/* ������ ��ġ�� ����Ʈ�� ��ȯ��. (��������ǥ�� ��ȯ�Ѵ�)*/
/* �����Ͷ����� : ���������� ��������Ͽ� �ȼ��� ���������. */

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float2 vTexcoord : TEXCOORD0;
	
};

struct PS_OUT
{
	vector vColor : SV_TARGET0;
};

/* 1. �ȼ��� �������� ������ �����Ѵ�. */
PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	
	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	return Out;
}

struct PS_OUT_LIGHT
{
	vector vShade : SV_TARGET0;
	vector vSpecular : SV_TARGET1;
    vector vCascadeShadow : SV_TARGET2;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT			Out = (PS_OUT_LIGHT)0;

	//vector vARM = g_ARMTexture.Sample(LinearSampler, In.vTexcoord);
    //float fAmbienOcc = vARM.r;
    //float fRoughness = vARM.g;
    //float fMetallic = vARM.b;
	
	vector		vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
	float		fViewZ = vDepthDesc.y * 1000.f;

	vector		vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
	vector		vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);	

	// ����Ʈ ����(��������Ʈ)
	// normalize(g_vLightDir.xyz) * -1.f ��� ����ȭ -> �׳� ���ؼ� �Ѱ��ֱ�
    float fLambert = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal) * 0.5f + 0.5f);
    Out.vShade = g_vLightDiffuse * saturate(fLambert + (g_vLightAmbient * g_vMtrlAmbient));

	vector		vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));

	vector		vPosition = (vector)0;

	/* ������������ ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�. */
	/* ������ġ * ������� * ����� * ������� / w */
	vPosition.x = In.vTexcoord.x * 2.f - 1.f;
	vPosition.y = In.vTexcoord.y * -2.f + 1.f;
	vPosition.z = vDepthDesc.x;
	vPosition.w = 1.f;

	/* �佺���̽� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
	/* ������ġ * ������� * �����  */
	vPosition = vPosition * fViewZ;
	vPosition = mul(vPosition, g_ProjMatrixInv);

	/* ���� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
	vPosition = mul(vPosition, g_ViewMatrixInv);	

	vector		vLook = vPosition - g_vCamPosition;

	
	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);
		

    float fShadowPower = 1.f;
    if (fViewZ <= 15.f)
    {
        fShadowPower = ComputeShadow(vPosition, 0, vNormalDesc);
    }
    else if (fViewZ > 15.f && fViewZ <= 30.f)
    {
        fShadowPower = ComputeShadow(vPosition, 1, vNormalDesc);
    }
    else if (fViewZ > 30.f && fViewZ <= 300.f)
    {
        fShadowPower = ComputeShadow(vPosition, 2, vNormalDesc);
    }
    Out.vCascadeShadow = fShadowPower;
	
	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT			Out = (PS_OUT_LIGHT)0;

	vector		vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
	float		fViewZ = vDepthDesc.y * 1000.f;

	vector		vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
	vector		vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector		vPosition = (vector)0;

	/* ������������ ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�. */
	/* ������ġ * ������� * ����� * ������� / w */
	vPosition.x = In.vTexcoord.x * 2.f - 1.f;
	vPosition.y = In.vTexcoord.y * -2.f + 1.f;
	vPosition.z = vDepthDesc.x;
	vPosition.w = 1.f;

	/* �佺���̽� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
	/* ������ġ * ������� * �����  */
	vPosition = vPosition * fViewZ;
	vPosition = mul(vPosition, g_ProjMatrixInv);

	/* ���� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
	vPosition = mul(vPosition, g_ViewMatrixInv);

	vector		vLightDir = vPosition - g_vLightPos;

	float		fAtt = saturate((g_fLightRange - length(vLightDir)) / g_fLightRange);

	Out.vShade = g_vLightDiffuse * saturate(max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;

	vector		vReflect = reflect(normalize(vLightDir), normalize(vNormal));
	vector		vLook = vPosition - g_vCamPosition;

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f) * fAtt;

	return Out;
}

PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    if (vDiffuse.a == 0.f)
    {
        vector vPriority = g_PriorityTexture.Sample(LinearSampler, In.vTexcoord);
        Out.vColor = vPriority;
        return Out;
    }

	vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
	vector		vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);
	
    Out.vColor = (vDiffuse * vShade + vSpecular) * g_CascadeShadowTexture.Sample(LinearSampler, In.vTexcoord);

	//vector		vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
	//float		fViewZ = vDepthDesc.y * 1000.f;

	///* 1. ���� �׷����� �ȼ��� ���������� ��ġ�� ��ȯ�ϱ����ؼ� �켱 ����� ��ġȯ�Ͽ� ������ġ�� ���Ѵ�. */
	//vector		vPosition = (vector)0;

	///* ������������ ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�. */
	///* ������ġ * ������� * ����� * ������� / w */
	//vPosition.x = In.vTexcoord.x * 2.f - 1.f;
	//vPosition.y = In.vTexcoord.y * -2.f + 1.f;
	//vPosition.z = vDepthDesc.x;
	//vPosition.w = 1.f;

	///* �佺���̽� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
	///* ������ġ * ������� * �����  */
	//vPosition = vPosition * fViewZ;
	//vPosition = mul(vPosition, g_ProjMatrixInv);

	///* ���� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
	//vPosition = mul(vPosition, g_ViewMatrixInv);

	///* 2. ������� �ȼ� ��ġ���ٰ� ������������ ������� ������� ���Ͽ� ���������� �����̽��� ��ȯ�Ѵ�. */
	//vector		vOldPos = mul(vPosition, g_LightViewMatrix);
	//vOldPos = mul(vOldPos, g_LightProjMatrix);
	
	//float		fLightDepth = vOldPos.w;

	//float2		vTexcoord;
	//vTexcoord.x = (vOldPos.x / vOldPos.w) * 0.5f + 0.5f;
	//vTexcoord.y = (vOldPos.y / vOldPos.w) * -0.5f + 0.5f;	

	//float		fOldLightDepth = g_LightDepthTexture.Sample(LinearSampler, vTexcoord).r * 1000.f;

	//if (fLightDepth - 0.1f > fOldLightDepth)
	//	Out.vColor = vector(Out.vColor.rgb * 0.6f, Out.vColor.a);

	//// ������Ʈ
 //   float3 vRimLightColor = float3(0.7f, 0.f, 0.f);
	
 //   float3 vEyeToCamera = normalize(g_vCamPosition.xyz - vPosition.xyz);
 //   vector vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
 //   float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    //float fRim = 1.f - saturate(dot(vEyeToCamera, vNormal));

    //fRim = pow(fRim, 5.f);
    //vRimLightColor *= fRim;
    //Out.vColor.xyz = saturate(Out.vColor.xyz + vRimLightColor); // 0.5��� ����
	
	return Out;	
}

PS_OUT PS_MAIN_BACKBUFFER(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_BackTexture.Sample(LinearSampler, In.vTexcoord);

	return Out;
}


technique11	DefaultTechnique
{	
	pass Debug
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Light_Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OnebyOne, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OnebyOne, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

	pass Deferred
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);


		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
	}

	pass BackBuffer
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BACKBUFFER();
    }
}