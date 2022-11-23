#version 110
// Simple fragment shader.
// Does texturing and phong shading.

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform vec3     lightpos;   // = WorldMatrix * LightPosition

// Parameters from the vertex shader
varying vec3 fragColor;
varying vec3 fragNormal;
varying vec4 fragPos;

void main() {
        // easy ambient color calculation
        vec3  ambient = material.ambient * fragColor;

        // diffuse color
        vec3  norm     = normalize(fragNormal);
        vec3  lightDir = normalize(lightpos - fragPos.xyz);
        float diff     = max(dot(norm, lightDir), 0.0);
        vec3  diffuse  = diff * material.diffuse * fragColor;

        // Final output
        gl_FragColor.rgb = (ambient + diffuse);
}
