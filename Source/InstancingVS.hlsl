#include "Instancing.hlsli"

GS_INPUT main(VS_INPUT In, uint instanceId : SV_INSTANCEID)
{
	GS_INPUT Out1;
	Out1.Pos = World[instanceId];
	Out1.Color = MaterialColor[instanceId];
	Out1.Size= SpriteSize[instanceId];

	Out1.Dis= Dissolve[instanceId];


	
	Out1.Mer = Timer[instanceId].x;
	
	return Out1;	//	return In;
}