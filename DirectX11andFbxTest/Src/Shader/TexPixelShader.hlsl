#define MAX_BONE_MATRIX 255
struct PS_IN
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

Texture2D    Texture : register(t0[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
Texture2D    ShaowTexture : register(t1[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�

float4 Diffuse(float ndl)
{
	/*
		Diffuse = Kd * Id
			Kd�F�}�e���A������DiffuseColor
			Id�F���C�g����Ambient�J���[

			Kd = �}�e���A��DiffuseColor * ���Ɩ@���̓���(�]��)
			���̐F * ���Ɩ@���̗]�� * ���˗�
			���}�e���A���ɔ��˗�������Ȃ炻����|����
	*/
	//return DiffuseLightColor * ndl;

	// �n�[�t�����o�[�g�ɂ��Diffuse�Z�o
	/*
		���̌v�Z�̓s�N�Z���V�F�[�_�ōs�킸�ɁA�o�[�e�b�N�X�V�F�[�_�ōs���Ă��ǂ�
		VertexShader.hlsl�Ƀn�[�t�����o�[�g�p�̌v�Z�������ăR�����g���Ă܂��B
	*/
	float half_ndl = ndl * 0.5 + 0.5;
	float squre_ndl = half_ndl * half_ndl;

	// ���̐F * ���Ɩ@���̗]��
	return  LightColor * squre_ndl * 0.5f;
}

float4 Phong(float4 world_pos, float normal_dot_light, float4 vertex_normal)
{
	/*
		Phong = Is * Ks;
			Is�F������Specular�J���[
			Ks�F�}�e���A������Specular�J���[

			Ks = �}�e���A����Specular�J���[ * pow((R�EV), ��)
				R�F���˃x�N�g��
				V�F�����x�N�g��
				���F����x
	*/
	float3 Normal = (float3)normalize(vertex_normal);
	float3 LightDir = (float3)normalize(LightVector);
	float3 ViewDir = (float3)normalize(world_pos - CameraPos);

	// �����̓��ς�-1�`1�܂ł͈̔͂��g�p����̂�saturate�͎g��Ȃ�
	float3 NL = dot(Normal, -LightDir);

	float3 Reflect = normalize((2 * NL * Normal) + LightDir);
	float specular = pow(saturate(dot(Reflect, -ViewDir)), 10);

	return MaterialSpecular * specular * LightColor;
}

float4 main(PS_IN input) : SV_Target
{
	// �e�N�X�`���J���[�̎擾
	float4 tex_color = Texture.Sample(Sampler, input.texture_pos);

	// Diffuse�v�Z
	tex_color += Diffuse(input.color);

	// Specular�v�Z
	tex_color += Phong(input.pos, input.color, input.vertex_normal);

	float2 shadow_uv = input.light_view_pos.xy / input.light_view_pos.w;

	//�͈͂����킹��
	shadow_uv.x = shadow_uv.x * 0.5f + 0.5f;
	shadow_uv.y = shadow_uv.y * -0.5f + 0.5f;

	if (shadow_uv.x >= 0.0f && shadow_uv.x <= 1.0f &&
		shadow_uv.y >= 0.0f && shadow_uv.y <= 1.0f)
	{
		//Z�l�擾
		float z_in_light_view = input.light_view_pos.z / input.light_view_pos.w;

		//�\�t�g�V���h�E���s�p��1�s�N�Z���T�C�Y
		float offset_u = 1.0f / 1260.0f;
		float offset_v = 1.0f / 760.0f;

		float z_in_shadomap0 = ShaowTexture.Sample(Sampler, shadow_uv);
		float z_in_shadomap1 = ShaowTexture.Sample(Sampler, shadow_uv + float2(offset_u, offset_v));
		float z_in_shadomap2 = ShaowTexture.Sample(Sampler, shadow_uv + float2(offset_u, 0.0f));
		float z_in_shadomap3 = ShaowTexture.Sample(Sampler, shadow_uv + float2(0.0f, offset_v));

		float bias = 0.0005f;

		float shadow_rate = 0.0f;

		if (z_in_light_view > z_in_shadomap0 + bias)
		{
			shadow_rate += 1.0f;
		}

		if (z_in_light_view > z_in_shadomap1 + bias)
		{
			shadow_rate += 1.0f;
		}

		if (z_in_light_view > z_in_shadomap2 + bias)
		{
			shadow_rate += 1.0f;
		}

		if (z_in_light_view > z_in_shadomap3 + bias)
		{
			shadow_rate += 1.0f;
		}

		
		shadow_rate /= 4.0f;

		//�`�悷��e�̔Z��(����)
		float3 shadow_color = tex_color.xyz * 0.7f;

		//���`��Ԃŉe�̔Z���𒲐�����
		tex_color.xyz = lerp(tex_color.xyz, shadow_color, shadow_rate);
	}
	// �A���r�G���g�J���[ + �f�B�t���[�Y�J���[ + �e�N�X�`���J���[
	return tex_color;

}
