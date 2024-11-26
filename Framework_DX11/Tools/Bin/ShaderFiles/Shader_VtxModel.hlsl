#include "Shader_Engine_Defines.hlsli"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float			g_fFar;

matrix g_CascadeViewMatrix[3], g_CascadeProjMatrix[3];

texture2D		g_DiffuseTexture;
texture2D		g_NormalTexture;
texture2D		g_ARMTexture;
texture2D		g_EmessiveTexture;

float			g_fEmessiveMask;

float4			g_fHashColor;
bool			g_bSelect = false;
bool			g_isLight = false;

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;		
	float3 vTangent : TANGENT;
};

struct VS_OUT
{	
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;	
	float4 vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(/*����*/VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);	
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexcoord = In.vTexcoord;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct VS_OUT_NORMAL
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;	
};

VS_OUT_NORMAL VS_MAIN_NORMAL(VS_IN In)
{
	VS_OUT_NORMAL			Out = (VS_OUT_NORMAL)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vTexcoord = In.vTexcoord;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

	return Out;
}

struct GS_IN_CASCADE
{
    float4 vPosition : SV_POSITION;
};

GS_IN_CASCADE VS_MAIN_CASCADE(VS_IN In)
{
    GS_IN_CASCADE Out = (GS_IN_CASCADE) 0;
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    return Out;
}

struct GS_OUT_CASCADE
{
    float4 vPosition : SV_POSITION;
    uint iRTVIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(9)]
void GS_MAIN(triangle GS_IN_CASCADE In[3], inout TriangleStream<GS_OUT_CASCADE> Container)
{
    GS_OUT_CASCADE Out[3];
    
    // �� ����Ÿ�� ���� ���α�
    for (uint i = 0; i < 3; ++i)
    {
        for (uint j = 0; j < 3; ++j)
        {
            // ����� ��ġ
            float4 vViewPos = mul(In[j].vPosition, g_CascadeViewMatrix[i]);
            Out[j].vPosition = mul(vViewPos, g_CascadeProjMatrix[i]);
            Out[j].iRTVIndex = i;
            Container.Append(Out[j]);
        }
        Container.RestartStrip();
    }
}

struct PS_IN
{
	float4 vPosition : SV_POSITION;
	float4 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;	
};

struct PS_OUT
{
	vector vDiffuse : SV_TARGET0;
	vector vNormal : SV_TARGET1;
	vector vDepth : SV_TARGET2;
    vector vARM : SV_TARGET3;
    vector vEmessive : SV_TARGET4;
    vector vRimLight : SV_TARGET5;
    vector vPickDepth : SV_TARGET6;
    vector vPickObjectDepth : SV_TARGET7;
};



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	
    vector vDiffuse ;
	
	if(g_isLight == false)
    {
        vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    }
	else
        vDiffuse = (0.6f, 0.6f, 0.4f, 1.f);
		
	
	if (0.3f >= vDiffuse.a)
		discard;
	
    vector vEmissive = g_EmessiveTexture.Sample(LinearClampSampler, In.vTexcoord) * g_fEmessiveMask;
    Out.vDiffuse = vDiffuse + vEmissive;

    if (g_bSelect)
    {
        Out.vDiffuse *= 3.f;
        Out.vDiffuse.b *= 5.f;
    }
	
	/* -1.f ~ 1.f -> 0.f ~ 1.f */
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    Out.vARM = g_ARMTexture.Sample(LinearSampler, In.vTexcoord);
    Out.vEmessive = vEmissive;
    Out.vRimLight = vector(0.f, 0.f, 0.f, 0.f);
    Out.vPickDepth = vector(In.vProjPos.z / In.vProjPos.w, 0.f, 0.f, 1.f);
    Out.vPickObjectDepth = g_fHashColor;

	return Out;
}

struct PS_OUT_PICKING
{
    vector vColor : SV_TARGET0;
};

PS_OUT_PICKING PS_MAIN_PICKING(PS_IN In)
{
    PS_OUT_PICKING Out = (PS_OUT_PICKING) 0;

    Out.vColor = g_fHashColor;

    return Out;
}

struct PS_IN_NORMAL
{
	float4 vPosition : SV_POSITION;
	float3 vNormal : NORMAL;
	float2 vTexcoord : TEXCOORD0;
	float4 vProjPos : TEXCOORD1;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
};

PS_OUT PS_MAIN_NORMAL(PS_IN_NORMAL In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector			vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

	vector			vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	/* ���û��� ��ȯ���� ���� �븻���͸� ���ߴ�. */
	/* ���ý����̽� => �����Ƿ��ý����̽�(x), ��ֺ��ʹ� �ϳ��� ���ý����̽��� ���������� �����ߴ�. */
	float3			vNormal = vNormalDesc.xyz * 2.f - 1.f;

	float3x3		WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = normalize(mul(vNormal, WorldMatrix));

	if (0.3f >= vDiffuse.a)
		discard;

	
    vector vEmissive = g_EmessiveTexture.Sample(LinearClampSampler, In.vTexcoord) * g_fEmessiveMask;

    Out.vDiffuse += vDiffuse + vEmissive;

    if (g_bSelect)
    {
        Out.vDiffuse *= 3.f;
        Out.vDiffuse.b *= 5.f;
    }
	
	/* -1.f ~ 1.f -> 0.f ~ 1.f */
	Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, 0.f, 0.f);
    Out.vARM = g_ARMTexture.Sample(LinearSampler, In.vTexcoord);
    Out.vEmessive = g_EmessiveTexture.Sample(LinearClampSampler, In.vTexcoord) * g_fEmessiveMask;
    Out.vRimLight = vector(0.f, 0.f, 0.f, 0.f);
    Out.vPickDepth = vector(In.vProjPos.z / In.vProjPos.w, 0.f, 0.f, 1.f);
    Out.vPickObjectDepth = g_fHashColor;

	return Out;
}

struct PS_IN_CASCADE
{
    float4 vPosition : SV_POSITION;
    uint iRTVIndex : SV_RenderTargetArrayIndex; // ����Ÿ�� �迭 �߿� � �Ϳ� �׸��ž�?
};

float4 PS_MAIN_CASCADE(PS_IN_CASCADE In) : SV_Target0
{
    return float4(In.vPosition.z, 0.f, 0.f, 1.f);
}


technique11	DefaultTechnique
{
	pass NonAnimModel
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass NormalMapping
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}

    pass Picking //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PICKING();
    }

    pass Cascade //3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_CASCADE();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_CASCADE();
    }
}