#version 110
// Simple fragment shader.
// Does texturing and phong shading.

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    // Phong only
    float shininess;  // Phong only
};

struct Light {
    vec3  pos;        // = WorldMatrix * LightPosition
    vec3  color;
    vec3  ambient;
};

uniform Material material;
uniform Light    light;


// Parameters from the vertex shader
varying vec4 fragColor;
varying vec3 fragNormal;
varying vec4 fragPos;

void main() {
        // easy ambient color calculation
        vec3  ambient  = (light.ambient * light.color * fragColor.rgb) + (material.ambient * fragColor.rgb);

        // diffuse color
        vec3  norm     = normalize(fragNormal);
        vec3  lightDir = normalize(light.pos - fragPos.xyz);
        float diff     = max(dot(norm, lightDir), 0.0);
        vec3  diffuse  = diff * material.diffuse * fragColor.rgb * light.color;

        // Final output
        gl_FragColor.rgb = (ambient + diffuse);
        gl_FragColor.a   = fragColor.a;
}
