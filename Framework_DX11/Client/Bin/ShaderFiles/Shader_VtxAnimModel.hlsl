
#include "Shader_Engine_Defines.hlsli"
#include "Shader_Client_InOut.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D       g_DiffuseTexture;
texture2D       g_NormalTexture;
texture2D       g_ARMTexture;

vector g_vLightDir;
vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;

vector g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.0f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vCamPosition;

/* �׸��� �޽ÿ��� ������ �ִ� ����ĵ�(o) */
matrix g_BoneMatrices[650];

VS_OUT_ANIMODEL VS_MAIN(VS_IN_ANIMODEL In)
{
    VS_OUT_ANIMODEL Out = (VS_OUT_ANIMODEL) 0;

    float fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BoneMatrices[In.vBlendIndices.w] * fWeightW;
		
    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vPosition, g_WorldMatrix);
    Out.vProjPos = Out.vPosition;

    return Out;
}

VS_OUT_NORMAL VS_MAIN_NORMAL(VS_IN_ANIMODEL In)
{
    VS_OUT_NORMAL Out = (VS_OUT_NORMAL) 0;

    float fWeightW = 1.f - (In.vBlendWeights.x + In.vBlendWeights.y + In.vBlendWeights.z);

    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndices.x] * In.vBlendWeights.x +
		g_BoneMatrices[In.vBlendIndices.y] * In.vBlendWeights.y +
		g_BoneMatrices[In.vBlendIndices.z] * In.vBlendWeights.z +
		g_BoneMatrices[In.vBlendIndices.w] * fWeightW;
		
    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
    vector vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);
	
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix)).xyz;
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;
    Out.vTangent = normalize(mul(vTangent, g_WorldMatrix)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

    return Out;
}

PS_OUT_MODEL PS_MAIN(PS_IN_ANIMODEL In)
{
    PS_OUT_MODEL Out = (PS_OUT_MODEL) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    if (0.3f > vMtrlDiffuse.a)
        discard;

    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f); // x:0~F�� Z�� ���� �� , y: view�����̽� ���� ����ȭ
    Out.vARM = g_ARMTexture.Sample(LinearSampler, In.vTexcoord);


    return Out;
}

PS_OUT_MODEL PS_MAIN_NORMAL(PS_IN_NORMAL In)
{
    PS_OUT_MODEL Out = (PS_OUT_MODEL) 0;

    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	/* ���û��� ��ȯ���� ���� �븻���͸� ���ߴ�. */
	/* ���ý����̽� => �����Ƿ��ý����̽�(x), ��ֺ��ʹ� �ϳ��� ���ý����̽��� ���������� �����ߴ�. */
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

    float3x3 WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);
    vNormal = normalize(mul(vNormal, WorldMatrix));

    if (0.3f >= vDiffuse.a)
        discard;

    Out.vDiffuse = vDiffuse;

	/* -1.f ~ 1.f -> 0.f ~ 1.f */
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.f, 0.f);
    Out.vARM = g_ARMTexture.Sample(LinearSampler, In.vTexcoord);

    return Out;
}

PS_OUT_LIGHTDEPTH PS_MAIN_LIGHTDEPTH(PS_IN_MODEL In)
{
    PS_OUT_LIGHTDEPTH Out = (PS_OUT_LIGHTDEPTH) 0;

    Out.vLightDepth = vector(In.vProjPos.w / 1000.f, 0.f, 0.f, 0.f);


    return Out;
}



technique11 DefaultTechnique
{
    pass AnimModel
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass LightDepth
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHTDEPTH();
    }

    pass AnimModel_Normal
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
    }
}