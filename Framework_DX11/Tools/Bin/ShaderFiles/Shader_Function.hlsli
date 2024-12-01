matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix g_ViewMatrixInv, g_ProjMatrixInv;

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

// ������ - �Ի�� �ݻ簢�� �̿��� ���� ���(Specular)
float3 FresnelSchlick(float LdotH, float3 fSpecularColor)
{
    return fSpecularColor + (1.f - fSpecularColor) * pow(1.f - LdotH, 5.f);
}

// ��ĥ�⿡ ���� �ݻ� ���� ��� ������ �ݻ纤�ͷ� ���
float DistributionGGX(float3 vNormal, float3 vHalfVector, float fRoughness)
{
    float a = fRoughness * fRoughness;
    float a2 = a * a;
    float NdotH = max(dot(vNormal, vHalfVector), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.141592 * denom * denom;

    return num / denom;
}


// ������ ����
float GeometrySchlickGGX(float NdotV, float fRoughness)
{
    float r = (fRoughness + 1.0);
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(float3 N, float3 V, float3 L, float fRoughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, fRoughness);
    float ggx1 = GeometrySchlickGGX(NdotL, fRoughness);

    return ggx1 * ggx2;
}

float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    
    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (3.141592 * denom * denom);
}

float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);

}

float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}
