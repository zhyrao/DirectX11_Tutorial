// light vertex shader

// globals
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;  // 16bytes
};

// typedefs
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

// VERTEX SHADER
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;

    // change the position vector to be 4units
    input.position.w = 1.f;

    // calculate the position of the vertex against the 
    // world, view, projection matrix
    output.position = mul(input.position, worldMatrix);
    worldPosition = output.position;

    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // store texture coordinate
    output.tex = input.tex;

    // normal trans to world space and normalize it
    output.normal = mul(input.normal, (float3x3) worldMatrix);

    // normalize
    output.normal = normalize(output.normal);

    // view direction in world space
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
    output.viewDirection = normalize(output.viewDirection);

    return output;
}