#version 310 es

#extension GL_GOOGLE_include_directive : enable

#include "constants.h"

layout(input_attachment_index = 0, set = 0, binding = 0) uniform highp subpassInput in_color;

layout(set = 0, binding = 1) uniform sampler2D color_grading_lut_texture_sampler;

layout(location = 0) out highp vec4 out_color;

void main()
{
    highp ivec2 lut_tex_size = textureSize(color_grading_lut_texture_sampler, 0);
    highp float _COLORS      = float(lut_tex_size.y);

    highp vec4 color       = subpassLoad(in_color).rgba;

    // sample lut tex: blue_index0 & blue_index0 + 1
    highp float blue_index = color.b * _COLORS;
    highp float blue_index0 = float(int(blue_index));
    highp float dt = blue_index - blue_index0;
    
    // texture(color_grading_lut_texture_sampler, uv)
    highp vec2 uv0 = vec2((blue_index0 + color.r) / _COLORS, color.g);
    highp vec4 color0 = texture(color_grading_lut_texture_sampler, uv0);
    highp vec2 uv1 = uv0 + vec2(1.f / _COLORS, 0.f);
    highp vec4 color1 = texture(color_grading_lut_texture_sampler, uv1);

    // interpolation
    highp vec4 lut_color = color0 * (1.f - dt) + color1 * dt;

    out_color = lut_color;
}
