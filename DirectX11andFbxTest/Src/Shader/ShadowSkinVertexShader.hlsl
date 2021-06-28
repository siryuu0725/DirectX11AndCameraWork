// �V�F�[�_�[�ɑ����{�[���s��̍ő吔
#define MAX_BONE_MATRIX 255
struct VS_IN
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
	float4 color : COLOR0;
	float2 texture_pos : TEXTURE0;
	uint4  bones : BONE_INDEX;
	float4 weights : BONE_WEIGHT;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
};

cbuffer ConstantBuffer:register(b0)
{
	float4x4	World;				//���[���h�ϊ��s��
	float4x4	View;				//�r���[�ϊ��s��
	float4x4	Projection;			//�����ˉe�ϊ��s��
	matrix      BoneWorld[MAX_BONE_MATRIX];
	float4		CameraPos;			//�J�������W
	float4x4	LightView;			//���C�g�r���[�ϊ��s��
	float4x4	LightProjection;	//���C�g�����ˉe�ϊ��s��
	float4		LightVector;		//���C�g����
	float4		LightColor;			//���C�g�J���[
	float4		MaterialAmbient;	//�A���r�G���g
	float4		MaterialDiffuse;	//�f�B�t���[�Y
	float4		MaterialSpecular;	//�X�y�L�����[
}

struct Skin
{
	float4 pos;
	float3 nor;
};

Skin SkinVert(VS_IN input)
{
	Skin output = (Skin)0;

	float4 pos = input.pos;

	// �{�[��0
	uint bone = input.bones.x;
	float weight = input.weights.x;
	matrix m = BoneWorld[bone];
	output.pos += weight * mul(pos, m);

	// �{�[��1
	bone = input.bones.y;
	weight = input.weights.y;
	m = BoneWorld[bone];
	output.pos += weight * mul(pos, m);

	// �{�[��2
	bone = input.bones.z;
	weight = input.weights.z;
	m = BoneWorld[bone];
	output.pos += weight * mul(pos, m);

	// �{�[��3
	bone = input.bones.w;
	weight = input.weights.w;
	m = BoneWorld[bone];
	output.pos += weight * mul(pos, m);

	return output;
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	// ���[�J�����W * ���[���h���W�ϊ��s��
	Skin skinned = SkinVert(input);
	output.pos = mul(skinned.pos, World);

	// ���[���h���W * �r���[���W�ϊ��s��
	output.pos = mul(output.pos, LightView);
	// �r���[���W * �v���W�F�N�V�������W�ϊ��s��
	output.pos = mul(output.pos, LightProjection);

	return output;
}