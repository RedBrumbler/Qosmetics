float distanceToPlane(float3 planePosition, float3 planeNormal, float3 pointInWorld, float3 transformOffset)
{
	float3 scale = float3(half3(
		length(unity_ObjectToWorld._m00_m10_m20),
		length(unity_ObjectToWorld._m01_m11_m21),
		length(unity_ObjectToWorld._m02_m12_m22)
		));
	planePosition -= transformOffset;
	planeNormal *= scale;
	//planePosition.y += 1;
	float3 w = -(planePosition - pointInWorld);
	return (
		planeNormal.x * w.x +
		planeNormal.y * w.y +
		planeNormal.z * w.z
		) / sqrt(
			planeNormal.x * planeNormal.x +
			planeNormal.y * planeNormal.y +
			planeNormal.z * planeNormal.z
		);
}

bool slice(float3 _CutPlane, float3 _SlicePos, float3 localPos, float3 _TransformOffset, float _SliceEdge){
	float SliceDir = length(_CutPlane);

	float planeDist = 0;

	if (SliceDir>0.5) {
		planeDist = distanceToPlane(_SlicePos, _CutPlane, localPos, _TransformOffset);
		if (planeDist < -_SliceEdge) {
			return true;
		}
		else return false;
	}
	else return false;
}
