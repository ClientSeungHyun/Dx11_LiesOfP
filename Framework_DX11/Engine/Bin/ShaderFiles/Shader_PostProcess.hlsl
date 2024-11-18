#include "Shader_Engine_Defines.hlsli"
#include "Shader_Function.hlsli"

matrix g_CameraViewMatrix, g_CameraProjMatrix;
matrix g_PrevCameraViewMatrix, g_PrevCameraProjMatrix;

texture2D g_BackTexture;
texture2D g_DepthTexture;

/* DOF */
texture2D g_DofBlurTexture;
float g_fFocus; // ����

/* Motion Blur */
float g_fMotionBlurPower;

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

PS_OUT PS_MAIN_DOF(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float fDepth = g_DepthTexture.Sample(PointSampler, In.vTexcoord).y * 1000.f;
    vector vBack = g_BackTexture.Sample(LinearSampler, In.vTexcoord);
    vector vBlur = g_DofBlurTexture.Sample(LinearSampler, In.vTexcoord);
    
    vector vColor = vBack;
    
    // ���̰� Ŭ���� �� ���� ����
    if(fDepth >= g_fFocus)
    {
        float fFocusRatio = saturate(abs(fDepth - g_fFocus) / g_fFocus);
    
        fFocusRatio = saturate(fFocusRatio);
        vColor = lerp(vBack, vBlur, fFocusRatio);
    }
   
    Out.vColor = vColor;
    
    //vColor = vBack;
    //if (vDepth >= g_fFocus)
    //{
    //    float fDepthDiff = (vDepth - g_fFocus) / (1000.f - g_fFocus);
    //    Out.vColor = lerp(vBack, vBlur, saturate(10.f * fDepthDiff));
    //}
    
    return Out;

}

PS_OUT PS_MAIN_MOTION_BLUR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vDepthDesc = g_DepthTexture.Sample(PointSampler, In.vTexcoord);
    float fViewZ = vDepthDesc.y * 1000.f;
		    
    vector vCurrentlWorldPos;

    vCurrentlWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vCurrentlWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vCurrentlWorldPos.z = vDepthDesc.x;
    vCurrentlWorldPos.w = 1.0f;

    // �̰ɷ� ���Ұ���( ���� ī�޶� ��ǥ)
    vector vCurrentPos = vCurrentlWorldPos;
    
    // ���� ��ǥ��
    vCurrentlWorldPos *= fViewZ;
    vCurrentlWorldPos = mul(vCurrentlWorldPos, g_ProjMatrixInv);
    vCurrentlWorldPos = mul(vCurrentlWorldPos, g_ViewMatrixInv);
	
    // ���� ī�޶� ��ǥ��
    matrix PrveMatVP = mul(g_PrevCameraViewMatrix, g_CameraProjMatrix);

    vector vPrevPos = mul(vCurrentlWorldPos, PrveMatVP);
    vPrevPos /= vPrevPos.w;

    float2 vVelocity = ((vCurrentPos - vPrevPos) * 0.5f).xy;
    float2 vTexCoord = In.vTexcoord;

    vector vColor = vector(0.f, 0.f, 0.f, 0.f);

    for (int i = -10; i < 10; ++i)
    {
        vTexCoord += vVelocity * (0.005f + g_fMotionBlurPower) * i;
        float4 vCurrentColor = g_BackTexture.Sample(LinearClampSampler, vTexCoord);
        vColor += vCurrentColor;
    }

    Out.vColor = vColor / 20.f;
	
    return Out;
}

technique11	DefaultTechnique
{	
    // 0 
    pass DOF
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DOF();
    }

    // 1
    pass MotionBlur
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MOTION_BLUR();
    }

    
}