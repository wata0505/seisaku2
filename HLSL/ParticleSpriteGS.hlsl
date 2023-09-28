
#include "ParticleSprite.hlsli"

[maxvertexcount(4)]
void main(point GS_INPUT In[1],
	inout TriangleStream<PS_INPUT> Stream)
{
	float4 	pos = In[0].Pos;
	//float4 pos = float4(0, 5, 0, 1);
	pos.w = 1.0f;
	// 座標変換　ワールド座標系　→　ビュー座標系
//	float4 pos = mul(float4(In[0].Pos), View);
	float3 Dir = float3(0,1,0);
	if (In[0].Type == SLASH){ //|| In[0].Type == EXPANSION) {
		Dir = In[0].Dir;
	}
	float sclea = 1;
	if (In[0].Type == DIFFUSION) { //|| In[0].Type == EXPANSION) {
		sclea = sin(In[0].Mer);
		Dir.x = sin(In[0].Mer) * sclea;
		Dir.y = cos(In[0].Mer) * sclea;
	}
	float3 N = float3(0, 1, 0);
	pos = mul(pos, View);
	//
	float c = 0;
	float s = 0;
	sincos(0, s, c);
	float d = dot(Dir,N);
	float cross = (Dir.y * N.x) - (Dir.x * N.y);
	d = acos(d);
	
	float4 size = float4(In[0].Size);
	float4 pPos = pos;
	sincos(d,s,c);
	//float size = ParticleSize.y;
	float4 poslt = float4(-size.x, size.w, 0.0, 0.0);
	float4 poslb = float4(-size.x, -size.y, 0.0, 0.0);
	float4 posrt = float4(size.x, size.w, 0.0, 0.0);
	float4 posrb = float4(size.x, -size.y, 0.0, 0.0);
	// 点を面にする
	float4 posLT = pPos + float4(Dir.y * poslt.x + -Dir.x * poslt.y, Dir.x * poslt.x + Dir.y * poslt.y, 0.0, 0.0) * pPos.w;
	float4 posLB = pPos + float4(Dir.y * poslb.x + -Dir.x * poslb.y, Dir.x * poslb.x + Dir.y * poslb.y, 0.0, 0.0) * pPos.w;
	float4 posRT = pPos + float4(Dir.y * posrt.x + -Dir.x * posrt.y, Dir.x * posrt.x + Dir.y * posrt.y, 0.0, 0.0) * pPos.w;
	float4 posRB = pPos + float4(Dir.y * posrb.x + -Dir.x * posrb.y, Dir.x * posrb.x + Dir.y * posrb.y, 0.0, 0.0) * pPos.w;


	// 左上の点の位置(射影座標系)を計算して出力
	PS_INPUT Out;
	float H = 0.0;
	float W = 0.0;
	Out.Pos = mul(posLT, Projection);
	
	Out.Tex = float2(0.0, 0.0);
	
	float3 wPos = posLT.xyz;
	Out.wPosition = wPos;
	Out.Color = float2(0.0, 0.0);
	
	Out.uvTimer = In[0].Mer;
	if (In[0].Type == EXPANSION) Out.uvTimer = 1;
	Stream.Append(Out);

	// 右上の点の位置(射影座標系)を計算して出力

	Out.Pos = mul(posLB, Projection);
	
		Out.Tex = float2(1.0, 0.0);
	
	wPos = posLB.xyz;
	Out.wPosition = wPos;
	Out.Color = float2(1.0, 0.0);
	//Out.uvTimer = In[0].Mer;
	Stream.Append(Out);

	// 左下の点の位置(射影座標系)を計算して出力
	Out.Pos = mul(posRT, Projection);
	
	
	Out.Tex = float2(0.0, 1.0);
	
	wPos = posRT.xyz;
	Out.wPosition = wPos;
	Out.Color = float2(0.0, 1.0);
	//Out.uvTimer = In[0].Mer;
	Stream.Append(Out);

	// 右下の点の位置(射影座標系)を計算して出力
	Out.Pos = mul(posRB, Projection);
	
	
	Out.Tex = float2(1.0, 1.0);
	
	wPos = posRB.xyz;
	Out.wPosition = wPos;
	Out.Color = float2(1.0, 1.0);
	//Out.uvTimer = In[0].Mer;
	Stream.Append(Out);
	Stream.RestartStrip();
}