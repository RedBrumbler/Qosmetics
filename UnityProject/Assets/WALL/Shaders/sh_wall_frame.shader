Shader "BeatSaber/wall Frame (by Ikeiwa)"
//SHADER BY IKEIWA, minor edits by RedBrumbler (obstacleCoreLocalScale fix and added Edge Multiplier)
{
    Properties
    {
        _Color("Color", Color) = (1,1,1,0)
        // these 2 properties are needed for the correct frame shader:
        _EdgeSize("Edge Multiplier", float) = 1
        _SizeParams("Size Params", Vector) = (0.5,0.5,0.5,0.05)

        [MaterialToggle] _CustomColors ("Custom Colors", Float) = 0
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100
        Cull Off

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            // if making your own shader, copy this next line:
            #include "Assets/Qosmetics Exporters/CGINC files/frame.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
            };

            struct v2f
            {
                float4 vertex : SV_POSITION;
            };

            // Don't forget to tell our cg program what properties we use
            float4 _SizeParams;
            float _EdgeSize;

            v2f vert (appdata v)
            {
                v2f o;

                // make sure to do this to your vertices when you make your own shader:
                v.vertex.xyz += frameDisplacement(v.vertex, 0.99, _SizeParams, _EdgeSize);
                
                o.vertex = UnityObjectToClipPos(v.vertex);
              
                return o;
            }

            fixed4 _Color;

            fixed4 frag (v2f i) : SV_Target
            {
                fixed4 col = 0.2 * _Color + 0.8 * fixed4(1,1,1,1);
                return col;
            }
            ENDCG
        }
    }
}
