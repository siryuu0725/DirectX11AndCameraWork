// �V�F�[�_�[�ɑ����{�[���s��̍ő吔
#define MAX_BONE_MATRIX 255
struct VS_IN
{
	float4 pos : POSITION0;
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
	output.pos = mul(output.pos, LightView);

	// �r���[���W * �v���W�F�N�V�������W�ϊ��s��
	output.pos = mul(output.pos, LightProjection);

	return output;
}
