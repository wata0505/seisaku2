struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    //in uint vertexid : VERTEXID
};

cbuffer DirLight : register(b1)
{
    //w = 0:xyz = űü w > 0: xyzĘu w¸Ł 
    float4 LightDirection;
    float4 LightColor;		//F
};
