// シェーダーに送れるボーン行列の最大数
#define MAX_BONE_MATRIX 255

struct PS_IN
{
	float4 pos : SV_POSITION;
};

cbuffer ConstantBuffer
{
	float4x4	World;				// ワールド変換行列
	float4x4	View;				// ビュー変換行列
	float4x4	Projection;			// 透視射影変換行列
	matrix      BoneWorld[MAX_BONE_MATRIX];
	float4		CameraPos;			// カメラ座標
	float4x4	LightView;		    // ライトビュー変換行列
	float4x4	LightProjection;	// ライト透視射影変換行列
	float4		LightVector;		// ライト方向
	float4		LightColor;			// ライトカラー
	float4		MaterialAmbient;	// アンビエント
	float4		MaterialDiffuse;	// ディフューズ
	float4		MaterialSpecular;	// スペキュラー
}

float4 main(PS_IN input) : SV_Target
{
	return float4(input.pos.z, input.pos.z, input.pos.z, 1.0f);
}
