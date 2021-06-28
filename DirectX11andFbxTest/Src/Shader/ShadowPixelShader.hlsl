// �V�F�[�_�[�ɑ����{�[���s��̍ő吔
#define MAX_BONE_MATRIX 255

struct PS_IN
{
	float4 pos : SV_POSITION;
};

cbuffer ConstantBuffer
{
	float4x4	World;				// ���[���h�ϊ��s��
	float4x4	View;				// �r���[�ϊ��s��
	float4x4	Projection;			// �����ˉe�ϊ��s��
	matrix      BoneWorld[MAX_BONE_MATRIX];
	float4		CameraPos;			// �J�������W
	float4x4	LightView;		    // ���C�g�r���[�ϊ��s��
	float4x4	LightProjection;	// ���C�g�����ˉe�ϊ��s��
	float4		LightVector;		// ���C�g����
	float4		LightColor;			// ���C�g�J���[
	float4		MaterialAmbient;	// �A���r�G���g
	float4		MaterialDiffuse;	// �f�B�t���[�Y
	float4		MaterialSpecular;	// �X�y�L�����[
}

float4 main(PS_IN input) : SV_Target
{
	return float4(input.pos.z, input.pos.z, input.pos.z, 1.0f);
}
