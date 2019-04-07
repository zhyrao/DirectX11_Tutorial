// globals
cbuffer PerFrameBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// typedefs
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

// VERTEX SHADER
PixelInputType FontVertexShader(VertexInputType input)
{
    PixelInputType output;

    // change the position to a point 
    input.position.w = 1.0f;

    // calculate the position of the vertex against the world, view, and projection matrix
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // store the texture coordinate
    output.tex = input.tex;

    return output;
}