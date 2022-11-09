#version 110
// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec3 fragColor;
varying vec3 fragNormal;
varying vec4 fragPos;
varying vec3 fragLightPos;

void main() {
        // easy ambient color calculation
        vec3  ambientColor = vec3(1,0.6,0);
        float ambientStrength = 0.3;
        vec3  ambient = ambientStrength * ambientColor;

        // diffuse color
        vec3  norm     = normalize(fragNormal);
        vec3  lightDir = normalize(fragLightPos - fragPos.xyz);
        float diff     = max(dot(norm, lightDir), 0.0);
        vec3  diffuse  = diff * ambientColor;

        // Final output
        gl_FragColor.rgb = (ambient + diffuse) * fragColor;
}
