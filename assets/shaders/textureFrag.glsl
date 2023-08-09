#version 330 core

in vec2 vertexUV;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec4 fragment_position_light_space;

uniform vec3 ambient_material;
uniform vec3 diffuse_material;
uniform vec3 specular_material;
uniform float shininess_material = 32.0f;

uniform vec3 light_color;
uniform vec3 light_position;

uniform sampler2D textureSampler;
uniform sampler2D shadow_map;

uniform vec3 view_position;

out vec4 FragColor;


vec3 ambient_color(vec3 light_color_arg) {
    return ambient_material * light_color_arg;
}

vec3 diffuse_color(vec3 light_color_arg, vec3 light_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);

    return diffuse_material * light_color_arg * max(dot(normalize(fragment_normal), light_direction), 0.0f);
}

vec3 specular_color(vec3 light_color_arg, vec3 light_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_light_direction = reflect(-light_direction, normalize(fragment_normal));

    return specular_material * light_color_arg * pow(max(dot(reflect_light_direction, view_direction), 0.0f), shininess_material);
}

float shadow_scalar() {
    vec3 normalized_device_coordinates = fragment_position_light_space.xyz / fragment_position_light_space.w;
    
    normalized_device_coordinates = normalized_device_coordinates * 0.5 + 0.5;
    
    float closest_depth = texture(shadow_map, normalized_device_coordinates.xy).r;
    
    float current_depth = normalized_device_coordinates.z;

    float bias = 0.03;
    
    return ((current_depth - bias) < closest_depth) ? 1.0 : 0.0;
}


void main()
{
    float scalar = shadow_scalar();

    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    ambient = ambient_color(light_color);
    diffuse = scalar * diffuse_color(light_color, light_position);
    specular = scalar * specular_color(light_color, light_position);

	vec3 textureColor = texture(textureSampler, vertexUV).rgb;

    vec3 color = (specular + diffuse + ambient) * textureColor;

	FragColor = vec4(color, 1.0f);
}
