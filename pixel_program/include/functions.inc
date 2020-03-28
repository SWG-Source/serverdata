// ======================================================================
// functions.inc
// HLSL pixel shader functions
// ======================================================================

#include "../../shared_program/functions.inc"

// ======================================================================

float calculateFakeAnisotropicSpecularLighting(float dot3SpecularNoPower)
{
	float tighten = 1.25f;
	float powerScale = 0.15f;
	return max(0, pow(1 - abs(1 - (dot3SpecularNoPower * tighten)), materialSpecularPower * powerScale) - powerScale);
}

// ======================================================================

float3 calculateHemisphericLighting(float3 direction, float3 normal, float3 vertexDiffuse)
{
	float  dotProduct = dot(direction, normal);
	
	float3 light = vertexDiffuse + dot3LightTangentMinusDiffuseColor + dot3LightDiffuseColor + (-max(0.0, dotProduct) * (dot3LightTangentMinusDiffuseColor));
	
	light += (min(0.0, dotProduct) * (dot3LightTangentMinusBackColor));

	return saturate(light);
}

float3 calculateHemisphericLightingVertexColor(float3 direction, float3 normal, float3 vertexDiffuse, float3 vertexColor)
{
	float  dotProduct = dot(direction, normal);
	
	float3 light = 
		  dot3LightTangentMinusDiffuseColor 
		+ dot3LightDiffuseColor 
		+ -max(0.0, dotProduct) * (dot3LightTangentMinusDiffuseColor)
		+  min(0.0, dotProduct) * (dot3LightTangentMinusBackColor)
		;
	
	light = light*vertexColor + vertexDiffuse;

	return saturate(light);
}

float3 calculateHemisphericLightingAlpha(float3 direction, float3 normal, float3 vertexDiffuse, float alpha)
{
	float  dotProduct = dot(direction, normal);
	float3 allMainLight = saturate(lerp(dotProduct, direction.z, alpha));
	
	float3 light = vertexDiffuse + dot3LightTangentMinusDiffuseColor + dot3LightDiffuseColor + (-max(0.0, allMainLight) * (dot3LightTangentMinusDiffuseColor));
	
	light += (min(0.0, dotProduct) * (dot3LightTangentMinusBackColor));

	return saturate(light);
}

