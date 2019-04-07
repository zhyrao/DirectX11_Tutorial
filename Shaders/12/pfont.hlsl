// globals
Texture2D shaderTexture;
SamplerState SamplerType;

cbuffer PixelBuffer
{
    float4 pixelColor;
};

// typedefs
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


// PIXEL SHADER
float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
    float4 color;

    // sample the texture pixel at this location
    color = shaderTexture.Sample(SamplerType, input.tex);

    // if the color is black on the texture then treat this pixle as transparent
    if (color.r == 0)
    {
        color.a = 0.0f;
    }
    else    // if the color is other than black on the texture then this is a pixle
    // in the font so draw it using the font pixel color
    {
        color.a = 1.0f;
        color = color * pixelColor;
    }

    return color;
}