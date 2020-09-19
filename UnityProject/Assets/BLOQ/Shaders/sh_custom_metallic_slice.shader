// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

Shader "BeatSaber/Slice/Metallic"
{
	Properties
	{
		_Color ("Color", Color) = (1,1,1,1)
		_SlicePos("Slice Local Position", Vector) = (0,0,0)
		_CutPlane("Slice Local Normal", Vector) = (0,0,0)
		_TransformOffset("Transform Offset", Vector) = (0,0,0)
		_SliceEdge("Slice Glow Edge", Float) = 0.025
		_Tex ("Reflection Texture", 2D) = "white" {}
	}
	SubShader
	{
		Tags{ "RenderType" = "Opaque" "DisableBatching" = "true" }
		LOD 100
		Cull Off

		Pass
		{
			CGPROGRAM

			#pragma vertex vert
			#pragma fragment frag
			// make fog work
			#pragma multi_compile_fog
			
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
				float4 color : COLOR;
				float3 worldPos : TEXCOORD1;
				float3 localPos : TEXCOORD2;
				half3 worldNormal : TEXCOORD3;
				half3 localNormal : TEXCOORD4;
				float3 viewDir : TEXCOORD5;
				float4 scrPos : TEXCOORD6;
				float3 normal: TEXCOORD7;
				float4 vertex : SV_POSITION;
				UNITY_FOG_COORDS(1)
			};

			float4 _Color;
			sampler2D _Tex;
			
			float3 _SlicePos;
			half3 _CutPlane;
			float3 _TransformOffset;
			float _SliceEdge;

			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;
				o.worldPos = mul(unity_ObjectToWorld, v.vertex);
				o.viewDir = normalize(UnityWorldSpaceViewDir(o.worldPos));
				o.normal = UnityObjectToWorldNormal(v.normal);
				UNITY_TRANSFER_FOG(o,o.vertex);
				o.localPos = v.vertex.xyz;
				return o;
			}
			
			fixed4 frag (v2f i) : SV_Target
			{

					if (slice(_CutPlane, _SlicePos, i.localPos, _TransformOffset, _SliceEdge)) {
						discard;
					}
				// sample the texture
				fixed4 col = _Color * (pow(tex2D(_Tex, float2(dot(i.normal.xyz,i.viewDir.xyz),dot(i.normal.xzy,i.viewDir.zyx))).r,10.0)*2.0);

				// apply fog
				UNITY_APPLY_FOG(i.fogCoord, col);
				return col * float4(1.0,1.0,1.0,0);
			}
			ENDCG
		}
	}
}
