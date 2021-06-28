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

VS_OUT main(VS_IN input)
{
	VS_OUT output;


	output.pos = mul(input.pos, World);

	// ワールド座標 * ビュー座標変換行列
	output.pos = mul(output.pos, View);
	// ビュー座標 * プロジェクション座標変換行列
	output.pos = mul(output.pos, Projection);

	// 頂点カラー
	output.color = input.color;

	// Texture座標指定
	output.texture_pos = input.texture_pos;

	// 移動が計算に反映させない
	input.nor.w = 0.0;
	// 法線ベクトル
	output.vertex_normal = mul(input.nor, World).xyzw;

	output.vertex_normal = normalize(output.vertex_normal);
	// saturate => 引数で指定した値を0～1間での範囲に収める
	// dot => 内積計算
	output.color *= saturate(dot(output.vertex_normal, normalize(-LightVector)));

	output.light_view_pos = mul(input.pos, World);

	// ワールド座標 * ビュー座標変換行列
	output.light_view_pos = mul(output.light_view_pos, LightView);
	// ビュー座標 * プロジェクション座標変換行列
	output.light_view_pos = mul(output.light_view_pos, LightProjection);

	return output;
}
