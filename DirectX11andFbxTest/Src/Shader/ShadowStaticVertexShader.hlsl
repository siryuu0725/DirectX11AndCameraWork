// シェーダーに送れるボーン行列の最大数
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
	output.pos = mul(output.pos, LightView);

	// ビュー座標 * プロジェクション座標変換行列
	output.pos = mul(output.pos, LightProjection);

	return output;
}
