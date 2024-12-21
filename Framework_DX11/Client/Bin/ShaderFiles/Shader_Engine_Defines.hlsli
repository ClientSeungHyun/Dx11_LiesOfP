
sampler PointSampler = sampler_state
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = wrap;
	AddressV = wrap;
};

sampler LinearSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

sampler LinearClampSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = clamp;
	AddressV = clamp;
};

sampler LinearBorderSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = BORDER;
    AddressV = BORDER;
};

sampler LinearYClampSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = clamp;
};

// �� ���÷�
// ���� �ؽ��� ���ø�
SamplerComparisonState DepthComparisonSampler = sampler_state
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;
    ComparisonFunc = LESS_EQUAL; // ���ø� ������� �۰ų� ���ƾ���
};

RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

RasterizerState RS_Wireframe
{
	FillMode = wireframe;
	CullMode = back;
	FrontCounterClockwise = false;
};

RasterizerState RS_Cull_CW
{
	FillMode = solid;
	CullMode = front;
	FrontCounterClockwise = false;
};

RasterizerState RS_Cull_None
{
	FillMode = solid;
	CullMode = none;
	FrontCounterClockwise = false;
};


DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_None
{
	DepthEnable = false;
	DepthWriteMask = zero;	
};

DepthStencilState DSS_NonWrite
{
    DepthEnable = true;
    DepthWriteMask = zero;
    DepthFunc = less_equal;
};


DepthStencilState DSS_Greater
{
    DepthEnable = true;
    DepthWriteMask = all;
    DepthFunc = greater;
};

DepthStencilState DSS_Always
{
    DepthEnable = true; // ���� �׽�Ʈ Ȱ��ȭ
    DepthWriteMask = all; // ���� ���ۿ� ���
    DepthFunc = always; // ������ ���� �׽�Ʈ ���
};

BlendState BS_Default
{
	BlendEnable[0] = false;	
};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = true;	

	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
};

BlendState BS_OnebyOne
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = one;
	DestBlend = one;
	BlendOp = Add;
};

