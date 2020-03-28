//
// @NOTE: make sure PixelShaderProgramView.cpp is updated if these change !!!
//

float4    packedRegister0        : register(c0);
float4    packedRegister1        : register(c1);
float4    packedRegister2        : register(c2);
float4    packedRegister3        : register(c3);
float4    packedRegister4        : register(c4);
float4    textureFactor          : register(c5);
float4    textureFactor2         : register(c6);
float4    materialSpecularColor  : register(c7);
float4    userConstants[17]      : register(c8);

static const float bloomSpecularRgbScale = 0.5;
static const float bloomSpecularAlphaScale = 0.65;

#define dot3LightDirection      packedRegister0.xyz
#define materialSpecularPower   packedRegister0.w

#define dot3LightDiffuseColor   packedRegister1.rgb
#define alphaFadeOpacityEnabled packedRegister1.a

#define dot3LightSpecularColor  packedRegister2.rgb
#define alphaFadeOpacity        packedRegister2.a

#define dot3LightTangentMinusDiffuseColor packedRegister3.rgb
#define bloomEnabled            packedRegister3.a

#define dot3LightTangentMinusBackColor    packedRegister4.rgb
#define timeElapsed		packedRegister4.a
