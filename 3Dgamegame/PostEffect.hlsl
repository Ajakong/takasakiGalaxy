Texture2D<float4> rt : register(t0);
Texture2D<float4> blurRT : register(t1);
Texture2D<float4> depthRT : register(t2);
Texture2D<float4> normalRT : register(t3);
SamplerState smp : register(s0);

struct PS_INPUT
{
    float4 pos : SVm_posITION;
    float4 dif : COLOR0;
    float4 spec : COLOR1;
    float2 uv : TEXCOORD0;
};

cbuffer UserData : register(b4)
{
    float dissolveHeight;
    float bbMinY;
    float bbMaxY;
    float dummy;
    float2 clickedUV;
}

struct PSOutput
{
    float4 col : SV_Target0;
    float4 outline : SV_Target1;
};


uint XorShift(uint val)
{
    val ^= (val << 13);
    val ^= (val >> 17);
    val ^= (val << 15);
    return val;
}

static const uint UINT_MAX = 0xffffffffu;

float Random(float val)
{
    float ref = float(XorShift(asuint(val))) / float(UINT_MAX);
    return ref;
}

float RandomFromUV(float2 uv)
{
    float rnd = Random(dot(uv, float2(3.14, 2.78)));
    rnd = Random(rnd + uv.x + uv.y);
    float rnd2 = Random(rnd);
    return rnd2;
}

PSOutput main(PS_INPUT input)
{
    uint2 wh;
    uint num;
    normalRT.GetDimensions(0, wh.x, wh.y, num); // �e�N�X�`���̉𑜓x���擾����
    float2 diffUV = 1.0 / float2(wh.x, wh.y); // �ƂȂ�̃s�N�Z����UV�����v�Z
    float3 norm = normalRT.Sample(smp, input.uv).rgb; // �^��
    
    // ����(4�ߖT)�̖@�������擾����
    float3 normL = normalRT.Sample(smp, input.uv + float2(-diffUV.x, 0)).rgb; // ��
    float3 normR = normalRT.Sample(smp, input.uv + float2(+diffUV.x, 0)).rgb; // �E
    float3 normU = normalRT.Sample(smp, input.uv + float2(0, -diffUV.y)).rgb; // ��
    float3 normD = normalRT.Sample(smp, input.uv + float2(0, +diffUV.y)).rgb; // ��
    
    float3 edgeN = saturate(abs(norm * 4 - normL - normR - normU - normD));
    float outlineN = 1 - saturate(edgeN.r + edgeN.g + edgeN.b);
    
    float targetDepth = depthRT.Sample(smp, clickedUV).r;
    
    float dep = depthRT.Sample(smp, input.uv).r;
    float depL = depthRT.Sample(smp, input.uv + float2(-diffUV.x, 0)).r; // ��
    float depR = depthRT.Sample(smp, input.uv + float2(+diffUV.x, 0)).r; // �E
    float depU = depthRT.Sample(smp, input.uv + float2(0, -diffUV.y)).r; // ��
    float depD = depthRT.Sample(smp, input.uv + float2(0, +diffUV.y)).r; // ��
    
    float outlineD = saturate(abs(dep * 4 - depL - depR - depU - depD));;
    outlineD = 1.0 - outlineD;
//    return float4(outlineD, outlineD, outlineD, 1.0);
    
    float diffDep = distance(dep, targetDepth);
    diffDep = pow(diffDep, 0.3);
    
    // ��ʊE�[�x
    float4 col = lerp(rt.Sample(smp, input.uv), blurRT.Sample(smp, input.uv), 0);
    
    float3 outline = float3(1, outlineN * outlineD, outlineN * outlineD);
    
    PSOutput output;
    output.col = float4(col.rgb * 0.75, col.a);
    output.outline = float4(edgeN, 1.0);
    return output;
    
    
    // ��C���ߖ@
    //return lerp(col, float4(0.5, 0.5, 0.5, 1), pow(dep, 500));
    //return pow(depthRT.Sample(smp, input.uv).rrrr, 100);
}