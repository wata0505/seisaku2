#include "Instancing.hlsli"
[maxvertexcount(4)]
void main(point GS_INPUT In[1],
	inout TriangleStream<PS_INPUT> Stream)
{
	float4 	pos = In[0].Pos;
	//float4 pos = float4(0, 5, 0, 1);
	pos.w = 1.0f;
	// 座標変換　ワールド座標系　→　ビュー座標系
//
	pos = mul(pos, View);
	
	float4 size = float4(In[0].Size);
	////	float4 pPos = mul(pos, Projection);
	float4 pPos = pos;
	// 点を面にする
	float4 posLT = pPos + float4(-size.x,  size.y, 0.0, 0.0) * pPos.w;
	float4 posLB = pPos + float4(-size.x, -size.y, 0.0, 0.0) * pPos.w;
	float4 posRT = pPos + float4(size.x,  size.y, 0.0, 0.0) * pPos.w;
	float4 posRB = pPos + float4(size.x, -size.y, 0.0, 0.0) * pPos.w;
	// 左上の点の位置(射影座標系)を計算して出力
	PS_INPUT Out;
	Out.MaterialColor = In[0].Color;
	Out.Dissolve = In[0].Dis;
	float H = 0.0;
	float W = 0.0;
	Out.Pos = mul(posLT, Projection);
	
	if (In[0].Mer >= 30 && In[0].Mer < 90) {
		W = 0.5;
	}
	if (In[0].Mer >= 60) {
		H = 0.5;
	}
	Out.Tex = float2(W, H);
	if (In[0].Mer == -1) Out.Tex = float2(0, 0);
	float3 wPos = posLT.xyz;
	//Out.wPosition = wPos;
	Out.Color = float2(0.0, 0.0);
	Stream.Append(Out);

	// 右上の点の位置(射影座標系)を計算して出力

	Out.Pos = mul(posLB, Projection);
	
	W = 0.5;
	H = 0.0;
	if (In[0].Mer >= 30 && In[0].Mer < 90) {
		W = 1.0;
	}
	if (In[0].Mer >= 60) {
		H = 0.5;
	}
	Out.Tex = float2(W, H);
	if (In[0].Mer == -1) Out.Tex = float2(1, 0);
	wPos = posLB.xyz;
	//Out.wPosition = wPos;
	Out.Color = float2(1.0, 0.0);
	Stream.Append(Out);

	// 左下の点の位置(射影座標系)を計算して出力
	Out.Pos = mul(posRT, Projection);
	
	H = 0.5;
	W = 0.0;
	if (In[0].Mer >= 30 && In[0].Mer < 90) {
		W = 0.5;
	}
	if (In[0].Mer >= 60) {
		H = 1.0;
	}
	
	Out.Tex = float2(W, H);
	if(In[0].Mer == -1) Out.Tex = float2(0, 1);
	wPos = posRT.xyz;
	//Out.wPosition = wPos;
	Out.Color = float2(0.0, 1.0);
	Stream.Append(Out);

	// 右下の点の位置(射影座標系)を計算して出力
	Out.Pos = mul(posRB, Projection);
	
	H = 0.5;
	W = 0.5;
	if (In[0].Mer >= 30 && In[0].Mer < 90) {
		W = 1.0;
	}
	if (In[0].Mer >= 60) {
		H = 1.0;
	}
	Out.Tex = float2(W, H);
	if (In[0].Mer == -1) Out.Tex = float2(1, 1);
	wPos = posRB.xyz;
	//Out.wPosition = wPos;
	Out.Color = float2(1.0, 1.0);
	Stream.Append(Out);
	Stream.RestartStrip();
}