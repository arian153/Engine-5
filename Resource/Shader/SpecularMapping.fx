Texture2D shader_texture[ 3 ];
SamplerState sample_type;

cbuffer MatrixBuffer
{
    matrix mvp;
    matrix world;
};

cbuffer CameraBuffer
{
    float3 camera_position;
};

cbuffer ColorBuffer
{
    float4 color;
};

cbuffer LightBuffer
{
    float4 ambient_color;
    float4 diffuse_color;
    float4 specular_color;
    float specular_power;
    float3 light_direction;
};

//defs
struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 view_direction : TEXCOORD1;
};

//Vertex Shader
PixelInputType VertexShaderEntry(VertexInputType input)
{
    input.position.w = 1.0f;

    PixelInputType output;
    output.position = mul(input.position, mvp);
    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3)world);
    output.normal = normalize(output.normal);

    output.tangent = mul(input.tangent, (float3x3)world);
    output.tangent = normalize(output.tangent);

    output.binormal = mul(input.binormal, (float3x3)world);
    output.binormal = normalize(output.binormal);

    float4 world_position = mul(input.position, world);

    output.view_direction = camera_position.xyz - world_position.xyz;
    output.view_direction = normalize(output.view_direction);

    return output;
}

// Pixel Shader
float4 PixelShaderEntry(PixelInputType input) : SV_TARGET
{
    float4 texture_color = color * shader_texture[ 0 ].Sample(sample_type, input.uv);
    float4 normal_map = shader_texture[ 1 ].Sample(sample_type, input.uv);
    normal_map = (normal_map * 2.0f) - 1.0f;
    float3 normal_vector = (normal_map.x * input.tangent) + (normal_map.y * input.binormal) + input.normal;
    normal_vector = normalize(normal_vector);
    float3 light_dir = -light_direction;
    float light_intensity = saturate(dot(normal_vector, light_dir));
    float4 light_color = saturate(ambient_color + (diffuse_color * light_intensity));
    light_color = light_color * texture_color;

    if (light_intensity > 0.0f)
    {
        float4 specular_intensity = shader_texture[ 2 ].Sample(sample_type, input.uv);
        float3 reflection = normalize(2.0f * light_intensity * normal_vector - light_dir);
        float4 specular = pow(saturate(dot(reflection, input.view_direction)), specular_power);
        specular = specular_color * specular * specular_intensity;
        light_color = saturate(light_color + specular);
    }

    return light_color;
}
