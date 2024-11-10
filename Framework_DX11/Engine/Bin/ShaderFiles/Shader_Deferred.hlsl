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

texture2D		g_DecalDiffuseTexture;
texture2D		g_DecalNormalTexture;

vector			g_vCamPosition;

// ĳ��ĳ�̵�� �׸��� ���
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

// ���� ��ġ ���
float4 Compute_WorldPos(float2 vTexcoord, float fDepth, float fViewZ)
{
    float4 vWorldPositoin = 0;
	/* ������������ ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�. */
	/* ������ġ * ������� * ����� * ������� / w */
    vWorldPositoin.x = vTexcoord.x * 2.f - 1.f;
    vWorldPositoin.y = vTexcoord.y * -2.f + 1.f;
    vWorldPositoin.z = fDepth;
    vWorldPositoin.w = 1.f;

	/* �佺���̽� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
	/* ������ġ * ������� * �����  */
    vWorldPositoin = vWorldPositoin * fViewZ;
    vWorldPositoin = mul(vWorldPositoin, g_ProjMatrixInv);

	/* ���� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
    vWorldPositoin = mul(vWorldPositoin, g_ViewMatrixInv);
	
    return vWorldPositoin;
}

// �ٻ縦 ���� �ݻ絵�� �����
float3 FresnelSchlick(float LdotH, float3 fSpecularColor)
{
    return fSpecularColor + (1.0 - fSpecularColor) * pow(1.f - LdotH, 5.f);
}

    // Normal Distribution Function (GGX/Trowbridge-Reitz)
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.141592 * denom * denom;

    return num / denom;
}

    // Geometry Function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
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

struct PS_OUT_LIGHT_POINT
{
    vector vShade : SV_TARGET0;
    vector vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT			Out = (PS_OUT_LIGHT)0;

    vector		vARM = g_ARMTexture.Sample(LinearSampler, In.vTexcoord);
    float		fAmbietnOcc = 1.f;
    float		fRoughness = vARM.g;
    float		fMetallic = vARM.b;
	
	vector		vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
	vector		vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
	
    float		fViewZ = vDepthDesc.y * 1000.f;
    float3		vNormal = float3(vNormalDesc.xyz * 2.f - 1.f);
	
    vector		vPosition = Compute_WorldPos(In.vTexcoord, vDepthDesc.x, fViewZ);
	
    float3		vViewDir = normalize(g_vCamPosition.xyz - vPosition.xyz);
    float3		vLightDir = normalize(g_vLightDir.xyz);
    float3		vReflect = reflect(normalize(g_vLightDir.xyz) * -1.f, normalize(vNormal));
	
	// �ݼӰ� ��ݼ��� �����ϱ� ���� �⺻ �ݻ絵 (��ݼ��� ��� F0 = 0.04)
    float3 F0 = lerp(float3(0.3f, 0.3f, 0.3f), float3(0.9f, 0.9f, 0.9f), fMetallic);
	
	// ���� ���� ���
    float3 H = normalize(vViewDir + vLightDir);

    // BRDF �ݻ籤 ���
    float3 F = FresnelSchlick(max(dot(H, vViewDir), 0.0), F0);
    float NDF = DistributionGGX(vNormal, H, fRoughness);
    float G = GeometrySmith(vNormal, vViewDir, vLightDir, fRoughness);

	// ����ŧ�� ��� (Cook-Torrance BRDF)
    float3 numerator = NDF * G * F;
    float3 denominator = 4.0 * max(dot(vNormal, vViewDir), 0.0) * max(dot(vNormal, vLightDir), 0.0) + 0.001;
    float3 specular = numerator / denominator;
	
	// Ȯ�걤 ��� (Lambertian Diffuse)
    float3 kS = F; // ����ŧ�� ���
    float3 kD = 1.0 - kS; // ��ǻ�� ���
    kD *= 1.0 - fMetallic; // �ݼӼ� ���������� Ȯ�� ������ ����
    float3 diffuse = kD * g_vLightDiffuse.rgb * max(dot(vNormal, vLightDir), 0.0);
	
	// ���� ���� ���
    float3 ambient = g_vLightAmbient.rgb + fAmbietnOcc;
    float3 finalColor = ambient + (diffuse + specular) * g_vLightDiffuse.rgb;

    // ���� ���
    Out.vShade = float4(finalColor, 1.0);
    Out.vSpecular = float4(specular, 1.0f);

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

PS_OUT_LIGHT_POINT PS_MAIN_LIGHT_POINT(PS_IN In)
{
    PS_OUT_LIGHT_POINT Out = (PS_OUT_LIGHT_POINT) 0;

	vector		vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
	float		fViewZ = vDepthDesc.y * 1000.f;

	vector		vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
	vector		vNormal = float4(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vPosition = Compute_WorldPos(In.vTexcoord, vDepthDesc.x, fViewZ);

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
    
    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vDiffuse.a == 0.f)
    {
        vector vPriority = g_PriorityTexture.Sample(LinearSampler, In.vTexcoord);
        Out.vColor = vPriority;
        return Out;
    }


    vector vDecal = g_DecalDiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vDiffuse = vector(lerp(vDiffuse, vDecal, vDecal.a)); // ���� ���� ���� ȥ��
	
	vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexcoord);
	vector		vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexcoord);
	
    Out.vColor = (vDiffuse * vShade + vSpecular) * 1.f; //g_CascadeShadowTexture.Sample(LinearSampler, In.vTexcoord);

	vector		vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
	float		fViewZ = vDepthDesc.y * 1000.f;

	/* 1. ���� �׷����� �ȼ��� ���������� ��ġ�� ��ȯ�ϱ����ؼ� �켱 ����� ��ġȯ�Ͽ� ������ġ�� ���Ѵ�. */
	vector		vPosition = Compute_WorldPos(In.vTexcoord, vDepthDesc.x, fViewZ);

	return Out;	
}


	
	// ������Ʈ
    //float3 vRimLightColor = float3(0.7f, 0.f, 0.f);
	
    //float3 vEyeToCamera = normalize(g_vCamPosition.xyz - vPosition.xyz);
    //vector vNormalDesc = g_NormalTexture.Sample(PointSampler, In.vTexcoord);
    //float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    //float fRim = 1.f - saturate(dot(vEyeToCamera, vNormal));

    //fRim = pow(fRim, 5.f);
    //vRimLightColor *= fRim;
    //Out.vColor.xyz = saturate(Out.vColor.xyz + vRimLightColor);
	
    
 //   float fLambert = saturate(dot(normalize(g_vLightDir) * -1.f, vNormal) * 0.5f + 0.5f);
 //   Out.vShade = g_vLightDiffuse * saturate(fLambert + (g_vLightAmbient * g_vMtrlAmbient));

	//vector		vLook = vPosition - g_vCamPosition;

	
	//Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);

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