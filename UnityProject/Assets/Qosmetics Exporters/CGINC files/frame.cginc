// math done by Ikeiwa, put into this standalone file by RedBrumbler

float3 calculateInsideVerts(float scale, float4 vertex);
float3 frameDisplacement(float4 vertex, float obstacleScale, float4 SizeParams, float EdgeSize);

float3 frameDisplacement(float4 vertex, float obstacleScale, float4 SizeParams, float EdgeSize)
{
	float3 result;
	float3 insideVerts = calculateInsideVerts(obstacleScale, vertex);


	result = (insideVerts * (1 - ((SizeParams.w * EdgeSize) / SizeParams.xyz))) * 0.5;
	return result;
}

float3 calculateInsideVerts(float scale, float4 vertex)
{
	float3 result = ((scale - abs(vertex.xyz))*sign(vertex.xyz)) * 2;

	return result;
}