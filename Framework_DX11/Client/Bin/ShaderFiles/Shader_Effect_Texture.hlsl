#include "Shader_Engine_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector g_vColor = vector(0.f, 0.f, 0.f, 0.f);
vector g_vMaskColor = vector(0.f, 0.f, 0.f, 0.f);

float2 g_vTexcoord = float2(0.5f, 0.5f);
float g_fRadian = 0.f;
float g_fTime = 0.f;

int g_iNumCol;
int g_iNumRow;
int g_iCol;
int g_iRow;

bool g_isChange = false;

texture2D g_DiffuseTexture;
texture2D g_NormalTexture;
texture2D g_MaskTexture_1;
texture2D g_MaskTexture_2;
texture2D g_OpacityTexture;

float2  g_vTexDivide;
int     g_iTexIndex;
float   g_fRatio;

cbuffer Effect_Desc
{
    float4 vColor_Add = float4(0.f, 0.f, 0.f, 0.f);         // ���� ����
    float4 vColor_Discard = float4(0.f, 0.f, 0.f, 0.f);     // �ڸ� ����
    float4 vColor_Mul = float4(1.f, 1.f, 1.f, 1.f);         // ���� ����
    
    //float2 vNumSprite = float2(1.f, 1.f);       // ��������Ʈ ��ü ����
    //float2 vSpriteIndex = float2(0.f, 0.f);     // ��������Ʈ ���� �ε���
    
    bool isMoveDistortion = false;              // ����� �����ϰ���?            
    float fDistortionSpeed = 1.f;               // �ӵ� �󸶳� �����ϰ���?
};

cbuffer Effect_Strength
{
    float fBloomIntensity;      // Bloom ����
    float fDistortionStrength;  // �ְ� ����
    float fDissolveAmount;      // Disslove ���� ��
};

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

VS_OUT VS_MAIN( /*����*/VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

struct PS_EFFECT_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNonDownSample : SV_TARGET1;
    vector vDownSample : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 start = (float2) 0;
    float2 over = (float2) 0;

    start.x = (1 / g_vTexDivide.x) * g_iTexIndex;
    start.y = (1 / g_vTexDivide.y) * (int) (g_iTexIndex / g_vTexDivide.x);
	
    over.x = start.x + (1 / g_vTexDivide.x);
    over.y = start.y + (1 / g_vTexDivide.y);
	
    float2 vTexcoord = start + (over - start) * In.vTexcoord;

    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, vTexcoord);
    
    return Out;
}

PS_EFFECT_OUT PS_ATTACK_NORMAL_LIGHT_MAIN(PS_IN In)
{
    PS_EFFECT_OUT Out = (PS_EFFECT_OUT) 0;
    
    vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    vColor.r = 1.f - (1 - g_vColor.r) * (1 - vColor.a);
    vColor.g = 1.f - (1 - g_vColor.g) * (1 - vColor.a);
    vColor.b = 1.f - (1 - g_vColor.b) * (1 - vColor.a);

    Out.vDiffuse = vColor;
    Out.vNonDownSample = (vector) 0.f;
    Out.vDownSample = vColor;
    
    return Out;
}

technique11 DefaultTechnique
{
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Attack_Normal_Light
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ATTACK_NORMAL_LIGHT_MAIN();
    }
}
