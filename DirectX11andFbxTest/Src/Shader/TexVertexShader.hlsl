struct VS_IN
{
	float4 pos : POSITION0;
	float2 texture_pos : TEXTURE0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 texture_pos : TEXTURE0;
};

cbuffer TexConstantBuffer
{
	float4x4	World;				//���[���h�ϊ��s��
	float4x4	Projection;			//�����ˉe�ϊ��s��
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	//output.pos = input.pos;

	output.pos = mul(input.pos, World);

	// �r���[���W * �v���W�F�N�V�������W�ϊ��s��
	output.pos = mul(output.pos, Projection);

	// Texture���W�w��
	output.texture_pos = input.texture_pos;

	return output;
}
