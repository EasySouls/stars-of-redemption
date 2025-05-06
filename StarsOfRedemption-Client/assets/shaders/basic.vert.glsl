#version 460 core

precision mediump float;

struct Vertex {
	vec2 position;
	vec3 color;
};

vec2 triangle_positions[3] = vec2[](
	vec2(0.5, -0.5),
	vec2(0.5, 0.5),
	vec2(-0.5, 0.5)
);

vec3 triangle_colors[3] = vec3[](
	vec3(1.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 1.0)
);

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec3 in_color;

layout(location = 0) out vec3 out_color;

void main() {
	gl_Position = vec4(triangle_positions[gl_VertexIndex], 0.0, 1.0);
	
	out_color = triangle_colors[gl_VertexIndex];
}