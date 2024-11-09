#include "Shader_Engine_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix g_ViewMatrixInv, g_ProjMatrixInv;
//matrix g_CameraViewMatrix;

texture2D g_DeacalDiffuseTexture;
texture2D g_DeacalARMTexture;
texture2D g_DeacalNormalTexture;

texture2D g_DepthTexture;
texture2D g_NormalTexture;
texture2D g_DiffuseTexture;

float4x4 g_vDecalWorld;
matrix g_vDecalWorldInverse;

float3 vScale = { 10.f, 10.f, 10.f };
float4 g_fHashColor;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
VS_OUT VS_MAIN( /*����*/VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    vector vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

    vPosition = mul(vPosition, g_ViewMatrix);
    vPosition = mul(vPosition, g_ProjMatrix);

    Out.vPosition = vPosition;
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
    vector vNormal : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 vTexUV;
    
    vTexUV.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
    
    vector vDepthDesc = g_DepthTexture.Sample(PointSampler, vTexUV);
    vector vNormalDesc = g_NormalTexture.Sample(PointSampler, vTexUV);
    
    
    float fViewZ = vDepthDesc.y * 1000.f;
    
    vector vPosition = (vector) 0;
    
    /* ������������ ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�. */
   /* ������ġ * ������� * ����� * ������� / w */
    vPosition.x = vTexUV.x * 2.f - 1.f;
    vPosition.y = vTexUV.y * -2.f + 1.f;
    vPosition.z = vDepthDesc.x;
    vPosition.w = 1.f;

   /* �佺���̽� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
   /* ������ġ * ������� * �����  */
    vPosition = vPosition * fViewZ;
    vPosition = mul(vPosition, g_ProjMatrixInv);
    /* ���� ���� ȭ�鿡 �׷����� �ȼ��� ��ġ�� ���Ѵ�.*/
    vPosition = mul(vPosition, g_ViewMatrixInv);
    
    //��Į�� ���÷� �̵�
    vector vLocalPos = mul(vPosition, g_vDecalWorldInverse);
    
    // ���� ����� ��Į �������� �̵��Ͽ� ȸ��
    float3 vNormal = mul(float4(vNormalDesc.xyz, 0.f), g_vDecalWorldInverse);
    
    float3 ObjectAbsPos = abs(vLocalPos.xyz);
    clip(0.5f - ObjectAbsPos);
    
    float2 vNewTexUV = vLocalPos.xy;
    vNewTexUV -= vLocalPos.z * vNormal.xy;
    vNewTexUV *= 0.5f;
    
    //512������ ��Į �̹���
    vNewTexUV *= 2.f;
    vNewTexUV += 0.5f;
    
    //float2 vDecalTexCoord = vLocalPos.xz + 0.5f;
    vector vDecalDiffuse = g_DeacalDiffuseTexture.Sample(LinearSampler, vNewTexUV);
    vector vDecalNormal = g_DeacalNormalTexture.Sample(LinearSampler, vNewTexUV);
    
    Out.vColor = vector(vDecalDiffuse.xyz, 0.8f);
    Out.vNormal = 0.f;
    
    return Out;
}

struct PS_OUT_PICKING
{
    vector vColor : SV_TARGET0;
};

PS_OUT_PICKING PS_MAIN_PICKING(PS_IN In)
{
    PS_OUT_PICKING Out = (PS_OUT_PICKING) 0;

    return Out;
}

technique11 DefaultTechnique
{
    pass Deacl
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);
      
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Picking //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_PICKING();
    }
}

    
    //bool isDecal;

    ////��Į ���� �������� Ȯ��
    //if (vPositionDecalLocal.x >= -0.5 && vPositionDecalLocal.x <= 0.5
    //    && vPositionDecalLocal.y >= -0.5 && vPositionDecalLocal.y <= 0.5
    //    && vPositionDecalLocal.z >= -0.5 && vPositionDecalLocal.z <= 0.5)
    //{
    //    isDecal = true;
    //}
    //else
    //    isDecal = false;
    
    //if (isDecal)
    //{
    //    //���� ��ǥ�� UV�� ��ȯ
        
    //    float u = vPositionDecalLocal.x + 0.5f;
    //    float v = vPositionDecalLocal.y + 0.5f;
        
    //    Out.vColor = g_Texture.Sample(LinearSampler, float2(u, v));
    //}