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
	float4x4	World;				//ワールド変換行列
	float4x4	Projection;			//透視射影変換行列
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	//output.pos = input.pos;

	output.pos = mul(input.pos, World);

	// ビュー座標 * プロジェクション座標変換行列
	output.pos = mul(output.pos, Projection);

	// Texture座標指定
	output.texture_pos = input.texture_pos;

	return output;
}
