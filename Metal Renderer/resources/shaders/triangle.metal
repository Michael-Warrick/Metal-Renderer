//
//  triangle.metal
//  Metal Renderer
//
//  Created by Michael Warrick on 18/10/2024.
//

#include <metal_stdlib>
using namespace metal;

struct VertexPayload
{
    float4 position [[position]];
    half3 color;
};

struct VertexData
{
    device float3* positions [[id(0)]];
    device float3* colors [[id(1)]];
};

struct FrameData
{
    float angle;
};

VertexPayload vertex vertexMain( device const VertexData* vertexData [[buffer(0)]], constant FrameData* frameData [[buffer(1)]], uint vertexId [[vertex_id]] )
{
    float a = frameData->angle;
    
    float3x3 rotationMatrix = float3x3(
                                       cos(a), -sin(a), 0.0,
                                       sin(a), cos(a),  0.0,
                                       0.0,    0.0,    1.0
                                       );
    
    VertexPayload vertexOut;
    vertexOut.position = float4( rotationMatrix * vertexData->positions[ vertexId ], 1.0 );
    vertexOut.color = half3(vertexData->colors[ vertexId ]);
    
    return vertexOut;
}

half4 fragment fragmentMain( VertexPayload fragIn [[stage_in]] )
{
    return half4( fragIn.color, 1.0 );
}
