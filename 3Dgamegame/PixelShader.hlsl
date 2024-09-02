SamplerState smp : register(s0);
Texture2D<float4> tex : register(t0);
Texture2D<float4> normal : register(t1);
Texture2D<float4> dissolveTex : register(t3);
Texture2D<float4> sph : register(t4);
Texture2D<float4> rough : register(t5);
Texture2D<float4> metal : register(t6);
Texture2D<float4> toon : register(t7);



cbuffer UserData : register(b4)
{
    float dissolveHeight;
    float bbMinY; // �o�E���f�B���O�{�b�N�X�̍ŏ�Y
    float bbMaxY; // �o�E���f�B���O�{�b�N�X�̍ő�Y
    float dummy;
};



struct PS_INPUT
{
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1; // �X�y�L�����J���[
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoords0 : TEXCOORD0; // �e�N�X�`�����W
    float4 svPosition : SVm_posITION; // ���W( �v���W�F�N�V������� )
    float4 Position : POSITION0; // ���W( ���[���h��� )
    float4 P : POSITION1; // ���W( �v���W�F�N�V������� )
    float4 VPosition : POSITION2; // ���W( ���[���h���*�r���[ )
    
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;
    float4 depht : SV_Target1;
    float4 normal : SV_Target2;
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



PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    PS_OUTPUT demoout;
    demoout.color = tex.Sample(smp, input.TexCoords0);
    return demoout;
    float3 nmap = normal.Sample(smp, input.TexCoords0);
    nmap = (nmap * 2.0 - 1.0);
    
    float3 N = normalize(nmap.x * input.Tangent - nmap.y * input.Binormal + nmap.z * input.Normal);
    N = (N);
    
    
    // float4��float�Ŏ󂯎��Ɛ擪�����󂯎��
    float dissolveNoise = dissolveTex.Sample(smp, input.TexCoords0).r;
    float range = bbMaxY - bbMinY;
    
    float dissolveRate = saturate(dissolveHeight / range);
    dissolveNoise *= 0.1;
    
    float YRate = (input.Position.y - bbMinY) / range;
    
    if (dissolveHeight + dissolveNoise < YRate)
    {
        discard;
    }
    
    float shineRate = 1.0f - saturate(dissolveHeight + dissolveNoise - YRate);
    float3 shineColor = float3(0.5, 1.0, 0.5) * pow(shineRate, 20.0);
    
    
    float4 texCol = tex.Sample(smp, input.TexCoords0);
    
    
    // ����
    float3 ligth = normalize(float3(1, -1, 1));
    // ����
    float3 eye = float3(0, 40, -300.0f); //�J�����̈ʒu���Q�l�ɂ���
    // �����x�N�g��
    float3 V = normalize(input.Position.xyz - eye);
    
    // ���˃x�N�g��
    float3 R = reflect(ligth, N);
    
    float3 V2 = normalize(input.VPosition.xyz - eye);
    
    
    // �������ۂ������̕\��
    float3 refView = reflect(V2, N);
    refView = normalize(refView);
    
    float3 refVecCol = float3((refView.xy + 1.0) / 2.0f, 0);
    
    float2 refViewUV = ((N.xy + 1.0) / 2.0);
    //float4 refCol = sph.Sample(smp, refViewUV);
    
    
    float rgh = rough.Sample(smp, input.TexCoords0).r;
    float mtl = metal.Sample(smp, input.TexCoords0).r;
    uint2 sphWH;
    uint mipSPH;
    sph.GetDimensions(0, sphWH.x, sphWH.y, mipSPH);
    float2 diffSPH = float2(2.0 / (float) sphWH.x, 2.0 / (float) sphWH.y);
    float4 refCol = sph.Sample(smp, refViewUV);
    float4 avgSPH = (refCol + sph.Sample(smp, refViewUV + diffSPH * float2(1, 0)) +
    sph.Sample(smp, refViewUV + diffSPH * float2(-1, 0)) +
    sph.Sample(smp, refViewUV + diffSPH * float2(0, 1)) +
    sph.Sample(smp, refViewUV + diffSPH * float2(0, -1))) / 5.0f;
    
    refCol = lerp(refCol, avgSPH, rgh);
    // �X�y�L�����[
    float spec = pow(saturate(dot(R, -V)), 20.0);
    // ����������̋���擾
    float ambient = 0.25f;
    float brightness = max(saturate(dot(N, -ligth)), ambient);
    
    float3 toonColor = toon.Sample(smp, float2(0.5, 1 - brightness));
    
    // �������C�g
    float rim = 1.0 - saturate(dot(-V, input.Normal));
    rim = pow(rim, 50);
    float3 rimColor = float3(0, 0, 0); //float3(1.0, 0.8, 0.8) * rim;
    
    PS_OUTPUT outdata;
    //spec = lerp(spec, spec * 0.5,rgh);
    
    float3 col = lerp(texCol.rgb, refCol.rgb, saturate(mtl));
    
    // ���܂ł̒ʏ�`�敔��
    outdata.color = float4(col * toonColor + spec + shineColor + rimColor, texCol.a);
    
    // �[�x�l
    float dep = input.P.z / input.P.w; // w�Ŋ����Ă����Ȃ���0�`1�ɂȂ�Ȃ�
    outdata.depht = float4(dep, dep, dep, 1.0);
    outdata.normal = float4((N + 1) * 0.5, 1.0);
    
    //outdata.color = float4(refVecCol, 1.0);
    return outdata;
}