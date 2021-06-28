#define MAX_BONE_MATRIX 255
struct VS_IN
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
	float4 color : COLOR0;
	float2 texture_pos : TEXTURE0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texture_pos : TEXTURE0;
	float4 vertex_normal : NORMAL0;
	float4 light_view_pos: LIGHT_VIEW;
};

cbuffer ConstantBuffer
{
	float4x4	World;				//���[���h�ϊ��s��
	float4x4	View;				//�r���[�ϊ��s��
	float4x4	Projection;			//�����ˉe�ϊ��s��
	matrix      BoneWorld[MAX_BONE_MATRIX];
	float4		CameraPos;			//�J�������W
	float4x4	LightView;		    //���C�g�r���[�ϊ��s��
	float4x4	LightProjection;	//���C�g�����ˉe�ϊ��s��
	float4		LightVector;		//���C�g����
	float4		LightColor;			//���C�g�J���[
	float4		MaterialAmbient;	//�A���r�G���g
	float4		MaterialDiffuse;	//�f�B�t���[�Y
	float4		MaterialSpecular;	//�X�y�L�����[
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;


	output.pos = mul(input.pos, World);

	// ���[���h���W * �r���[���W�ϊ��s��
	output.pos = mul(output.pos, View);
	// �r���[���W * �v���W�F�N�V�������W�ϊ��s��
	output.pos = mul(output.pos, Projection);

	// ���_�J���[
	output.color = input.color;

	// Texture���W�w��
	output.texture_pos = input.texture_pos;

	// �ړ����v�Z�ɔ��f�����Ȃ�
	input.nor.w = 0.0;
	// �@���x�N�g��
	output.vertex_normal = mul(input.nor, World).xyzw;

	output.vertex_normal = normalize(output.vertex_normal);
	// saturate => �����Ŏw�肵���l��0�`1�Ԃł͈̔͂Ɏ��߂�
	// dot => ���όv�Z
	output.color *= saturate(dot(output.vertex_normal, normalize(-LightVector)));

	output.light_view_pos = mul(input.pos, World);

	// ���[���h���W * �r���[���W�ϊ��s��
	output.light_view_pos = mul(output.light_view_pos, LightView);
	// �r���[���W * �v���W�F�N�V�������W�ϊ��s��
	output.light_view_pos = mul(output.light_view_pos, LightProjection);

	return output;
}
