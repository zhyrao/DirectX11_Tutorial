// light pixel shader

// globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};


// typedefs
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
};


// PIXEL SHADER
float4 LightPixelShader(PixelInputType input) : SV_Target
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;

    // sample pixel color from the texture using the sampler at this texture coordinate location
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    // set the default output color to the ambient light value for all pixels
    color = ambientColor;

    // invert the light direction for calculate
    lightDir = -lightDirection;

    // calculate intensity of light on this pixel
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0)
    {
        color += (diffuseColor * lightIntensity);
    }

    // determine the final amount of diffuse color based on 
    // the diffuse color combined with light intensity
    color = saturate(color);

    // multiply the texture pixel and the final diffuse color 
    // to get the final pixel color result
    color = color * textureColor;

    return color;
}