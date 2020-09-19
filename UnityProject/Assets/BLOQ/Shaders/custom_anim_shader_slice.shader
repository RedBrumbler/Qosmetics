// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

Shader "BeatSaber/Slice/Unlit Glow Cutout Dithered Animation"
{
	Properties
	{
		_Color("Color", Color) = (1,1,1,1)
		_MainTex("Texture", 2D) = "white" {}
		_Bloom("Glow", Range(0, 1)) = 0
		_DitherMaskScale("Dither Mask Scale", Float) = 40
		_DitherMask("Dither Mask", 2D) = "white" {}
		_Alpha("Alpha", Float) = 1
		_Cutout("Cutout", Range(0, 1)) = 0.5

		_SlicePos("Slice Local Position", Vector) = (0,0,0)
		_CutPlane("Slice Local Normal", Vector) = (0,0,0)
		_TransformOffset("Transform Offset", Vector) = (0,0,0)
		_SliceEdge("Slice Glow Edge", Float) = 0.025

	}
		SubShader
	{
		Tags{ "RenderType" = "Opaque" }
		LOD 100
		Cull Off

		Pass
	{
		CGPROGRAM
#pragma vertex vert
#pragma fragment frag

#include "UnityCG.cginc"
#include "Slice.cginc"

		struct appdata
	{
		float4 vertex : POSITION;
		float4 color : COLOR;
		float2 uv : TEXCOORD0;
		float3 normal : NORMAL;
		UNITY_VERTEX_INPUT_INSTANCE_ID
	};

	struct v2f
	{
		float2 uv : TEXCOORD0;
		float4 color : COLOR;
		float3 worldPos : TEXCOORD1;
		float3 localPos : TEXCOORD2;
		half3 worldNormal : TEXCOORD3;
		half3 localNormal : TEXCOORD4;
		float4 scrPos : TEXCOORD5;
		float4 vertex: POSITION;
	};

	float4 _Color;
	float _Bloom;
	sampler2D _DitherMask;
	float _DitherMaskScale;
	float _Alpha;
	float _Cutout;

	sampler2D _MainTex;
	float4 _MainTex_ST;

	float3 _SlicePos;
	half3 _CutPlane;
	float3 _TransformOffset;
	float _SliceEdge;

	v2f vert(appdata_full v)
	{
		v2f o;
		o.vertex = UnityObjectToClipPos(v.vertex);
		o.uv = v.texcoord;
		o.color = v.color;
		o.scrPos = ComputeScreenPos(v.vertex);
		o.localPos = v.vertex.xyz;

		return o;
	}

	fixed4 frag(v2f i) : SV_Target
	{
		// slice math
			if (slice(_CutPlane, _SlicePos, i.localPos, _TransformOffset, _SliceEdge)) {
				discard;
			}
		

		fixed4 col = _Color * tex2D(_MainTex, TRANSFORM_TEX(i.uv, _MainTex));

		if (col.a < _Cutout) discard;

		float4 ase_screenPos = float4(i.scrPos.xyz , i.scrPos.w + 0.00000000001);
		float4 ase_screenPosNorm = ase_screenPos / ase_screenPos.w;

		if (tex2D(_DitherMask, ase_screenPosNorm.xy* _ScreenParams.xy * _DitherMaskScale).r >= _Alpha * i.color.a) discard;

		col *= float4(i.color.rgb,0.0);
		col.a = _Bloom;

		return col;
	}
		ENDCG
	}
	}
}
