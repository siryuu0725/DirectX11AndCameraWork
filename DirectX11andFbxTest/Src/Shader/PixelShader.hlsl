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
	float4x4	World;				//ワールド変換行列
	float4x4	View;				//ビュー変換行列
	float4x4	Projection;			//透視射影変換行列
	matrix      BoneWorld[MAX_BONE_MATRIX];
	float4		CameraPos;			//カメラ座標
	float4x4	LightView;		    //ライトビュー変換行列
	float4x4	LightProjection;	//ライト透視射影変換行列
	float4		LightVector;		//ライト方向
	float4		LightColor;			//ライトカラー
	float4		MaterialAmbient;	//アンビエント
	float4		MaterialDiffuse;	//ディフューズ
	float4		MaterialSpecular;	//スペキュラー
}

Texture2D    Texture : register(t0[0]);      // Textureをスロット0の0番目のテクスチャレジスタに設定
Texture2D    ShaowTexture : register(t1[0]); // ShadowTextureをスロット1の0番目のテクスチャレジスタに設定
SamplerState Sampler : register(s0[0]);      // Samplerをスロット0の0番目のサンプラレジスタに設定

float4 Diffuse(float ndl)
{
	// ハーフランバートによるDiffuse算出
	float half_ndl = ndl * 0.5 + 0.5;
	float squre_ndl = half_ndl * half_ndl;

	// 光の色 * 光と法線の余弦
	return  LightColor * squre_ndl * 0.5f;
}

float4 Phong(float4 world_pos, float normal_dot_light, float4 vertex_normal)
{
	float3 Normal = (float3)normalize(vertex_normal);
	float3 LightDir = (float3)normalize(LightVector);
	float3 ViewDir = (float3)normalize(world_pos - CameraPos);

	// ここの内積は-1～1までの範囲を使用するのでsaturateは使わない
	float3 NL = dot(Normal, -LightDir);

	float3 Reflect = normalize((2 * NL * Normal) + LightDir);
	float specular = pow(saturate(dot(Reflect, -ViewDir)), 10);

	return specular * LightColor;
}

float4 main(PS_IN input) : SV_Target
{
	// テクスチャカラーの取得
	float4 tex_color = Texture.Sample(Sampler, input.texture_pos);

	// Diffuse計算
	tex_color += Diffuse(input.color);

	// Specular計算
	tex_color += Phong(input.pos, input.color, input.vertex_normal);

	float2 shadow_uv = input.light_view_pos.xy / input.light_view_pos.w;

	//範囲を合わせる
	shadow_uv.x = shadow_uv.x * 0.5f + 0.5f;
	shadow_uv.y = shadow_uv.y * -0.5f + 0.5f;

	if (shadow_uv.x >= 0.0f && shadow_uv.x <= 1.0f &&
		shadow_uv.y >= 0.0f && shadow_uv.y <= 1.0f)
	{
		//Z値取得
		float z_in_light_view = input.light_view_pos.z / input.light_view_pos.w;

		//ソフトシャドウ実行用の1ピクセルサイズ
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

		//平均値
		shadow_rate /= 4.0f;

		//描画する影の濃さ(割合)
		float3 shadow_color = tex_color.xyz * 0.7f;

		//線形補間で影の濃さを調整する
		tex_color.xyz = lerp(tex_color.xyz, shadow_color, shadow_rate);
	}

	return tex_color;

}
