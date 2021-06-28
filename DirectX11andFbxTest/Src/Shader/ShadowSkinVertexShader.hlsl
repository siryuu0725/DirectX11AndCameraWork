// シェーダーに送れるボーン行列の最大数
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
	float4x4	World;				//ワールド変換行列
	float4x4	View;				//ビュー変換行列
	float4x4	Projection;			//透視射影変換行列
	matrix      BoneWorld[MAX_BONE_MATRIX];
	float4		CameraPos;			//カメラ座標
	float4x4	LightView;			//ライトビュー変換行列
	float4x4	LightProjection;	//ライト透視射影変換行列
	float4		LightVector;		//ライト方向
	float4		LightColor;			//ライトカラー
	float4		MaterialAmbient;	//アンビエント
	float4		MaterialDiffuse;	//ディフューズ
	float4		MaterialSpecular;	//スペキュラー
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

	// ボーン0
	uint bone = input.bones.x;
	float weight = input.weights.x;
	matrix m = BoneWorld[bone];
	output.pos += weight * mul(pos, m);

	// ボーン1
	bone = input.bones.y;
	weight = input.weights.y;
	m = BoneWorld[bone];
	output.pos += weight * mul(pos, m);

	// ボーン2
	bone = input.bones.z;
	weight = input.weights.z;
	m = BoneWorld[bone];
	output.pos += weight * mul(pos, m);

	// ボーン3
	bone = input.bones.w;
	weight = input.weights.w;
	m = BoneWorld[bone];
	output.pos += weight * mul(pos, m);

	return output;
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	// ローカル座標 * ワールド座標変換行列
	Skin skinned = SkinVert(input);
	output.pos = mul(skinned.pos, World);

	// ワールド座標 * ビュー座標変換行列
	output.pos = mul(output.pos, LightView);
	// ビュー座標 * プロジェクション座標変換行列
	output.pos = mul(output.pos, LightProjection);

	return output;
}