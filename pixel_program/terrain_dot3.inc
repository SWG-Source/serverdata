//--------------------------------------------------------------------------------------
//-- construct layers of terrain, including emissive & specular mapping
//--------------------------------------------------------------------------------------


struct ComputeConstants
{
	float2 tc;
	float3 lightDir;
	float4 dot3LightColor;
	float4 vertexLight;
	float3 eyeVector_t;
};

struct ComputeConstantsSpec
{
	ComputeConstants cc;
	float4 specColor;
	float3 halfAngle_t;
};

#define CCS_MACRO \
	ComputeConstantsSpec ccs; \
	ccs.cc.tc = tc; \
	ccs.cc.lightDir = normalize(lightDirection); \
	ccs.cc.dot3LightColor = dot3LightColor; \
	ccs.cc.vertexLight = vertexLight; \
	ccs.cc.eyeVector_t = normalize(eyeVector_t); \
	ccs.specColor = float4(dot3LightSpecularColor, 0.0) * materialSpecularColor; \
	ccs.halfAngle_t = normalize(halfAngle_t);

#define CC_MACRO \
	ComputeConstants cc; \
	cc.tc = tc; \
	cc.lightDir = normalize(lightDirection); \
	cc.dot3LightColor = dot3LightColor; \
	cc.vertexLight = vertexLight; \
	cc.eyeVector_t = normalize(eyeVector_t);
	
//--------------------------------------------------------------------------------------

float4 computeLayerColor2(
			float3 diffuseMap,
			float3 normalMap, 
			ComputeConstants cc)
{
	float3 light = calculateHemisphericLightingVertexColor(cc.lightDir, normalMap, cc.vertexLight, cc.dot3LightColor);

	float4 layer = float4(diffuseMap * light, 0.0);
			
	return layer;
}

//--------------------------------------------------------------------------------------

float4 computeLayerColor(
			sampler samplerDiffuse, 
			sampler samplerNormal, 
			ComputeConstants cc)
{
	// parallax mapping
	//  T = T + (Vxy * h)
	float heightMap = tex2D(samplerNormal, cc.tc).a;
	
	float h = (heightMap - 0.5f) * 0.02;
	float2 offset = cc.eyeVector_t.xy * h;
	float2 newTc = cc.tc + offset;

	float3 diffuseMap = tex2D(samplerDiffuse, newTc);
	float3 normalMap  = signAndBias(tex2D(samplerNormal, newTc));

	return computeLayerColor2(diffuseMap, normalMap, cc);
	
}

//--------------------------------------------------------------------------------------


float4 computeLayerColorEmissive2(
			float4 diffuseMap,
			float3 normalMap, 
			float emissiveScale,
			ComputeConstants cc)
{
	float emis = diffuseMap.a * emissiveScale;
	float3 light = calculateHemisphericLightingVertexColor(cc.lightDir, normalMap, cc.vertexLight, cc.dot3LightColor);	
	float4 layer = float4(max(diffuseMap * emis, diffuseMap * light), emis * 0.5);
		
	return layer;
}

//--------------------------------------------------------------------------------------

float4 computeLayerColorEmissive(
			sampler samplerDiffuse, 
			sampler samplerNormal, 
			float emissiveScale,
			ComputeConstants cc)
{
	// parallax mapping
	//  T = T + (Vxy * h)
	float heightMap = tex2D(samplerNormal, cc.tc).a;
	
	float h = (heightMap - 0.5f) * 0.02;
	float2 offset = cc.eyeVector_t.xy * h;
	float2 newTc = cc.tc + offset;

	float4 diffuseMap = tex2D(samplerDiffuse, newTc);
	float3 normalMap  = signAndBias(tex2D(samplerNormal, newTc));

	return computeLayerColorEmissive2(diffuseMap, normalMap, emissiveScale, cc);
	
}

//--------------------------------------------------------------------------------------

float4 computeLayerColorEmissiveSpecular2(
			float4 diffuseMap,
			float3 normalMap, 
			float4 aux, 
			float emissiveScale,
			ComputeConstantsSpec ccs)
{
	float emis = diffuseMap.a * emissiveScale;
	float3 light = calculateHemisphericLightingVertexColor(ccs.cc.lightDir, normalMap, ccs.cc.vertexLight, ccs.cc.dot3LightColor);
	float4 layer = float4(max(diffuseMap * emis, diffuseMap * light), emis * 0.5);
		
	// specular
	{
		float specularBloom = materialSpecularColor.a;
		float specularMap = aux.r;
		
		float3 spec = specularMap;
		if (any(spec))
		{
			float localHdN = saturate(dot(ccs.halfAngle_t, normalMap));
			float localSpecPowered = pow(localHdN, materialSpecularPower);
			
			float3 localSpec = spec * ((localSpecPowered * ccs.specColor));
				
			float bloom = intensity(localSpec) * bloomSpecularRgbScale * specularBloom;
			layer += float4(localSpec, bloom);

		}
	}
		
	return layer;
}

//--------------------------------------------------------------------------------------

float4 computeLayerColorEmissiveSpecular(
			sampler samplerDiffuse, 
			sampler samplerNormal, 
			sampler samplerAux, 
			float emissiveScale, 
			ComputeConstantsSpec ccs)
{

	// parallax mapping
	//  T = T + (Vxy * h)
	float heightMap = tex2D(samplerNormal, ccs.cc.tc).a;
	
	float h = (heightMap - 0.5f) * 0.02;
	float2 offset = ccs.cc.eyeVector_t.xy * h;
	float2 newTc = ccs.cc.tc + offset;
	
	float4 diffuseMap = tex2D(samplerDiffuse, newTc);
	float3 normalMap  = normalize(signAndBias(tex2D(samplerNormal, newTc)));
	float4 aux = tex2D(samplerAux, newTc);

	float4 color = computeLayerColorEmissiveSpecular2(diffuseMap, normalMap, aux, emissiveScale, ccs);
	
	return color;
	
}

//--------------------------------------------------------------------------------------
