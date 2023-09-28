struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    //in uint vertexid : VERTEXID
};

cbuffer DirLight : register(b1)
{
    //w = 0:xyz = •ûŒü w > 0: xyzˆÊ’u wŒ¸Š‹——£ 
    float4 LightDirection;
    float4 LightColor;		//F
};
