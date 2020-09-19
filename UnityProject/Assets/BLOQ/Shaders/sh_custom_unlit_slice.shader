// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

Shader "BeatSaber/Slice/Unlit Glow"
{
	Properties
	{
		_Color ("Color", Color) = (1,1,1,1)
		_Tex ("Texture", 2D) = "white" {}
		_Glow ("Glow", Range (0, 1)) = 0
		_SlicePos("Slice Local Position", Vector) = (0,0,0)
		_CutPlane("Slice Local Normal", Vector) = (0,0,0)
		_TransformOffset("Transform Offset", Vector) = (0,0,0)
		_SliceEdge("Slice Glow Edge", Float) = 0.025

	}
	SubShader
	{
		Tags{ "RenderType" = "Opaque" "DisableBatching" = "true" }
		LOD 100

		Pass
		{
			Cull Off
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"
			#include "Assets/Qosmetics Exporters/CGINC files/slice.cginc"

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
				float4 vertColor : COLOR;
				float3 worldPos : TEXCOORD1;
				float3 localPos : TEXCOORD2;
				half3 worldNormal : TEXCOORD3;
				half3 localNormal : TEXCOORD4;
			};

			float4 _Color;
			float _Glow;

			sampler2D _Tex;
			float4 _Tex_ST;
			
			float3 _SlicePos;
			half3 _CutPlane;
			float3 _TransformOffset;
			float _SliceEdge;

			v2f vert(appdata v, out float4 outpos : SV_POSITION)
			{
				v2f o;
				o.uv = v.uv;
				o.vertColor = v.color;
				o.worldPos = mul(unity_ObjectToWorld, v.vertex).xyz;
				o.localPos = v.vertex.xyz;
				o.worldNormal = UnityObjectToWorldNormal(v.normal);
				o.localNormal = v.normal.xyz;
				outpos = UnityObjectToClipPos(v.vertex);

				return o;
			}

			fixed4 frag(v2f i) : SV_Target
			{
				
					if (slice(_CutPlane, _SlicePos, i.localPos, _TransformOffset, _SliceEdge)) {
						discard;
					}

				fixed4 col = _Color * tex2D(_Tex, TRANSFORM_TEX(i.uv, _Tex));
				return col * float4(1.0, 1.0, 1.0, _Glow) * i.vertColor;
			}

			ENDCG
		}
	}
}
