
// ����ȣ
struct Particle
{
    float3 vPosition;
    float2 vPSize;
    float4 vRight;
    float4 vUp;
    float4 vLook;
    float4 vTranslation;
    float2 vLifeTime;
    float4 vColor;
    float fSpeed;
    float4 vCurrenrRandomDir;
    float4 vNextRandomDir;
};

// UAV�� ������ �� �ִ� Structured Buffer ����
RWStructuredBuffer<Particle> particles : register(u0);
//RWStructuredBuffer<Particle> Initparticles : register(u1);

// �̰� �������� �Ѱ��ִ� ����
cbuffer MovementBuffer : register(b0)   // �޾ƿ� �� ����� ���� ���� �����ε�?
{
    uint    iNumInstance;
    uint	iState;
    float2  pad_1;
    
    float4  vPivot;
	float	fGravity;
    float3  pad_2;
    
    float4  vMoveDir;
    matrix  WorldMatrix;
    
    float3  vOrbitAxis;
	float	fOrbitAngle;
    
	float	fTimeInterval;
	float	fRandomRatio;
    float	fAccelSpeed;
	float	fAccelLimit;
    
	float	fTimeDelta;
    float3  pad_3;
}

//float rand(float seed);

[numthreads(256, 1, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    uint iIndex = DTid.x;
    
    if (iIndex >= iNumInstance)
        return;
    
    float2 vLifeTime = particles[iIndex].vLifeTime;
    vLifeTime.y += fTimeDelta;
    
    particles[iIndex].vLifeTime = vLifeTime;
    
    particles[iIndex].vTranslation.y += 1.f * fTimeDelta;
    particles[iIndex].vLook = float4(0.f, 1.f, 0.f, 0.f);
    
}

//float rand(float seed)
//{
//    return frac(sin(seed) * 43758.5453123);
//}
