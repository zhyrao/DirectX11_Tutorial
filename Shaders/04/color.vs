// Filename: color.vs

// GLOBALS
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// TYPEDEFS
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


// VERTEX SHADER
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;

	// change the position vector to be 4 units for proper matrix calculations
    input.position.w = 1.0f;

	// calculate the position of the vertex against the world, view, projection matrix
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// store the input color the pixel shader to use
    output.color = input.color;

    return output;
}