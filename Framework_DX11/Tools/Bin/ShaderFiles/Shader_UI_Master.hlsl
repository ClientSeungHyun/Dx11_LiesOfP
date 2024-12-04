
#include "Shader_Engine_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_DepthTexture;

bool g_Multiple;
float4 g_Color;
float g_Alpha_Strash;
float4 g_Render_Area; // (�׸� ��ǥ ���� ����) (left,top,right,bottom)
float2 g_Render_Angle; // (�׸� ���� ����) (Start, end) (vec(1,0) ������ 0���� �Ͽ� 0~360���� ����) (���� : CW)


// �÷� ���� 
float4 Input_Color(float4 vColor)
{
    float4 vReturn = vColor;
    
    if (vColor.r >= 0.f)
        vReturn.r = g_Color.r;
	
    if (vColor.g >= 0.f)
        vReturn.g = g_Color.g;
	
    if (vColor.b >= 0.f)
        vReturn.b = g_Color.b;
	
    if (vColor.a >= 0.f)
        vReturn.a = g_Color.a;
    
    return vReturn;
}

// �÷� ����
float4 Multi_Color(float4 vColor)
{
    float4 vReturn = vColor;
    
    vReturn.r *= g_Color.r;
    vReturn.g *= g_Color.g;
    vReturn.b *= g_Color.b;
    vReturn.a *= g_Color.a;
    
    return vReturn;
}

// ���� ���� 
float2 Clamp_Range(float2 vTex)
{
    float2 vReturn = vTex;
    vReturn.x = clamp(vTex.x, g_Render_Area.x, g_Render_Area.z);
    vReturn.y = clamp(vTex.y, g_Render_Area.y, g_Render_Area.w);
    return vReturn;
}

 // ���� ����
bool Check_Angle(float2 vTex, float3 vPos)
{
    float2 vCenter = { 0.5f, 0.5f };
    float2 vDirec = float2(vTex.x, vTex.y) - vCenter;
    float vRadian = atan2(vDirec.y, vDirec.x);
    
    if (g_Render_Angle.x <= g_Render_Angle.y)
        if ((vRadian < radians(g_Render_Angle.x)) || (vRadian > radians(g_Render_Angle.y)))
            return false;
        
    if (g_Render_Angle.x > g_Render_Angle.y)
        if ((vRadian < radians(g_Render_Angle.x)) && (vRadian > radians(g_Render_Angle.y)))
            return false;
    
    return true;
}

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	/* SV_ : ShaderValue */
	/* ���� �ؾ��� ������ ��� ���ĳ��� ���̹Ƿ� ���� dx�� �߰������� �ؾ��� �̸��� �ض�. */
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
	
};

/* 1. ������ ��ȯ������ �����Ѵ�. */
/* 2. ������ ���� ������ �����Ѵ�. */
VS_OUT VS_MAIN( /*����*/VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	/* ������ ��ġ�� ���� �� ������ȯ�Ѵ�.*/		
	/* Ŭ���̾�Ʈ���� ���������� �����ϴ� TrnasformCoord�Լ��ʹ� �ٸ��� */
	/* mul�Լ��� ��쿡�� �����ϰ� ����� ���ϱ⸸ ������ �ϰ� w�����⿬����ü�� �������� �ʴ´�. */
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

VS_OUT VS_RANGE( /*����*/VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

	/* ������ ��ġ�� ���� �� ������ȯ�Ѵ�.*/		
	/* Ŭ���̾�Ʈ���� ���������� �����ϴ� TrnasformCoord�Լ��ʹ� �ٸ��� */
	/* mul�Լ��� ��쿡�� �����ϰ� ����� ���ϱ⸸ ������ �ϰ� w�����⿬����ü�� �������� �ʴ´�. */
    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);
    
    Out.vPosition = vPosition;
    Out.vTexcoord = Clamp_Range(In.vTexcoord);

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

vector Sample(float2 vTexcoord)
{
    return g_Texture.Sample(LinearSampler, vTexcoord);
}

// 1. �⺻ ��� 
PS_OUT PS_BASIC(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = Sample(In.vTexcoord);
	
    if (Out.vColor.a < g_Alpha_Strash)
        discard;

    return Out;
}

// 2. �÷� ���� 
PS_OUT PS_COLOR_INPUT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = Sample(In.vTexcoord);
    
    if (Out.vColor.a < g_Alpha_Strash)
        discard;
    
    Out.vColor = Input_Color(Out.vColor);
	
    return Out;
}

// 3. �÷� ����
PS_OUT PS_COLOR_MULTIPLE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = Sample(In.vTexcoord);
    
    if (Out.vColor.a < g_Alpha_Strash)
        discard;

    Out.vColor = Multi_Color(Out.vColor);
   
    return Out;
}

// 5. ���� ���� 
PS_OUT PS_ANGLE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = Sample(In.vTexcoord);
	
    if (Out.vColor.a < g_Alpha_Strash)
        discard;
    
    if (Check_Angle(In.vTexcoord, In.vPosition) == false)
        discard;

    return Out;
}

// 8. �÷� ���� + ���� ���� 
PS_OUT PS_COLOR_INPUT_ANGLE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = Sample(In.vTexcoord);
    
    Out.vColor = Input_Color(Out.vColor);
    
    if (Check_Angle(In.vTexcoord, In.vPosition) == false)
        discard;
 
    return Out;
}

// 9. �÷� ���� + ���� ���� 
PS_OUT PS_COLOR_MULTIPLE_ANGLE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = Sample(In.vTexcoord);
	
    if (Out.vColor.a < g_Alpha_Strash)
        discard;

    Out.vColor = Multi_Color(Out.vColor);
    
    if (Check_Angle(In.vTexcoord, In.vPosition) == false)
        discard;

    return Out;
}

technique11 DefaultTechnique
{
	/* ������ + ������Ʈ + ssao + ��ָ��� + pbr*/

    // 0. �⺻ ���
    pass BASIC
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BASIC();
    }

    // 1. �÷� ���� 
    pass COLOR_INPUT
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR_INPUT();
    }

    // 2. �÷� ����
    pass COLOR_MULTIPLE
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR_MULTIPLE();
    }

    // 3. ���� ����  
    pass RANGE
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_RANGE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BASIC();
    }

    // 4. ���� ����
    pass ANGLE
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_ANGLE();
    }

    // 5. �÷� ���� + ���� ����  
    pass COLOR_INPUT_RANGE
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_RANGE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR_INPUT();
    }

    // 6. �÷� ���� + ���� ���� 
    pass COLOR_MULTIPLE_RANGE
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_RANGE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR_MULTIPLE();
    }

    // 7. �÷� ���� + ���� ����
    pass COLOR_INPUT_ANGLE
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR_INPUT_ANGLE();
    }

    // 8. �÷� ���� + ���� ����
    pass COLOR_MULTIPLE_ANGLE
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR_MULTIPLE_ANGLE();
    }
}