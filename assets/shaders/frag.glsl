#version 330 core

in vec3 fragment_position;
in vec3 fragment_normal;
in vec4 fragment_position_light_space;

uniform vec3 light_color;
uniform vec3 light_position;

uniform vec3 object_color;

uniform float shading_ambient_strength = 0.1f;
uniform float shading_diffuse_strength = 0.6f;
uniform float shading_specular_strength = 0.9f;

uniform sampler2D shadow_map;

uniform vec3 view_position;

out vec4 result;


vec3 ambient_color(vec3 light_color_arg) {
    return shading_ambient_strength * light_color_arg;
}

vec3 diffuse_color(vec3 light_color_arg, vec3 light_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    
    return shading_diffuse_strength * light_color_arg * max(dot(normalize(fragment_normal), light_direction), 0.0f);
}

vec3 specular_color(vec3 light_color_arg, vec3 light_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_light_direction = reflect(-light_direction, normalize(fragment_normal));
    
    return shading_specular_strength * light_color_arg * pow(max(dot(reflect_light_direction, view_direction), 0.0f),32);
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
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
   	vec3 specular = vec3(0.0f);

    float scalar = shadow_scalar();

    ambient = ambient_color(light_color);
    diffuse = scalar * diffuse_color(light_color, light_position);
    specular = scalar * specular_color(light_color, light_position);
    
    vec3 color = (specular + diffuse + ambient) * object_color;
    
    result = vec4(color, 1.0f);
}
