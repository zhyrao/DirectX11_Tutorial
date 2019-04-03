// light pixel shader

// globals
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};


// typedefs
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
    float3 viewDirection : TEXCOORD1;
};


// PIXEL SHADER
float4 LightPixelShader(PixelInputType input) : SV_Target
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;

    // sample pixel color from the texture using the sampler at this texture coordinate location
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    // set the default output color to the ambient light value for all pixels
    color = ambientColor;

    // init specular color
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // invert the light direction for calculate
    lightDir = -lightDirection;

    // calculate intensity of light on this pixel
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0)
    {
        color += (diffuseColor * lightIntensity);
        color = saturate(color);

        // calculate the reflection vector
        reflection = normalize(2 * lightIntensity * input.normal - lightDir);

        // determine the amount of specular light based on the reflection vector, view direction, specular power
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower) * specularColor;
    }

    // multiply the texture pixel and the final diffuse color 
    // to get the final pixel color result
    color = color * textureColor;

    // add specular component last to the output color
    color = saturate(color + specular);

    return color;
}