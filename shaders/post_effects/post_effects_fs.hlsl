
#include "gamma_correction.hlsli"
#include "tone_mapping.hlsli"


Texture2D hdr_buffer : register(t0);
SamplerState smplr : register(s0);

cbuffer camera_cb : register(b0)
{
	float3 cam_pos;
	float cam_gamma;
	
	float3 cam_dir;
	float cam_exposure;
};


float4	frag(float4 sv_pos : SV_POSITION, float2 uv : UV) : SV_TARGET
{
	float4 hdr_color = hdr_buffer.Sample(smplr, uv);
	float exposure = hdr_color.w;
	
	float3 tm_color = tone_mapping(hdr_color.xyz, cam_exposure * exposure);
	float3 gamma_color = gamma_correction(tm_color.xyz, cam_gamma);
	float4 color = float4(gamma_color.xyz, 1.0f);
	
	return (color);
}
