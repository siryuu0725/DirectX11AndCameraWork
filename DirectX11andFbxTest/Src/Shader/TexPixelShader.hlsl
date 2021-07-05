struct PS_IN
{
	float4 pos : POSITION0;
	float2 texture_pos : TEXTURE0;
};

Texture2D    Texture : register(t0); // Textureをスロット0の0番目のテクスチャレジスタに設定
SamplerState Sampler : register(s0); // Samplerをスロット0の0番目のサンプラレジスタに設定

float4 main(PS_IN input) : SV_Target
{
	// テクスチャカラーの取得
	float4 tex_color = Texture.Sample(Sampler, input.texture_pos);

	return tex_color;

}
