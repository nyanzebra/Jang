#version 420

uniform mat4 model;
uniform vec3 camera_position;

uniform sampler2D textures[16]; 
uniform float shininess;
uniform vec3 specular;

in light {

} light;

in data {
	vec3 position;
	vec2 coordinate;
	uint texture;
	vec4 color;
	vec3 normal;
} in_data;

out final_color;

void main() {
	vec3 normal = normalize(transpose(inverse(mat3(model))) * in_data.normal);
	vec3 position = vec3(model * vec4(in_data.position, 1));
	vec4 color = texture(textures[in_data.texture], in_data.coordinate);
	vec3 light = normalize()
}
