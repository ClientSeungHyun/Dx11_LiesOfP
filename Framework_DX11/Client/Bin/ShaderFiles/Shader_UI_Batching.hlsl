
#include "Shader_Engine_Defines.hlsli"

// �ؽ��� ���� ���� ������ �ݵ�� ������ �� 
#define MAX_TEXTURES_UI 653
#define MAX_TEXTURES_ITEM 334

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_Texture;
texture2D g_DepthTexture;
float4 g_Color;

Texture2D g_Textures_UI[MAX_TEXTURES_UI] : register(t0); // �ؽ�ó �迭 (MAX_TEXTURES�� �ִ� �ؽ�ó ����)
Texture2D g_Textures_Item[MAX_TEXTURES_ITEM] : register(t0); // �ؽ�ó �迭 (MAX_TEXTURES�� �ִ� �ؽ�ó ����)
SamplerState samplerState : register(s0); // ���÷� ����

struct VertexInput
{
    float4 position : POSITION;
    float2 texCoords : TEXCOORD;
    int textureID : TEXID; // �ؽ�ó ID (�� ����/�ν��Ͻ��� �Ҵ�� �ؽ�ó ��ȣ)
};

float4 PS_Main(VertexInput input) : SV_Target
{
    // �ؽ�ó ID�� ����� �ش� �ؽ�ó ���ø�
    float4 color = textures[input.textureID].Sample(samplerState, input.texCoords);
    return color;
}



