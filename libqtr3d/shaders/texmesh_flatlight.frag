#version 110
// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec2 fragTexcoords;
varying vec3 fragNormal;
varying vec4 fragPos;
varying vec3 fragLightPos;
varying vec3 fragLightAmbientk;
varying vec3 fragLightDiffusek;

// Textures
uniform sampler2D texture;

void main() {
        // easy ambient color calculation
        vec3  ambientColor = vec3(1,0.6,0);
        vec3  ambient = fragLightAmbientk * ambientColor;
        vec4  materialColor = texture2D( texture, fragTexcoords );

        // diffuse color
        vec3  norm     = normalize(fragNormal);
        vec3  lightDir = normalize(fragLightPos - fragPos.xyz);
        float diff     = max(dot(norm, lightDir), 0.0);
        vec3  diffuse  = diff * fragLightDiffusek * ambientColor;

        // Final output
        gl_FragColor.rgb = (ambient + diffuse) * materialColor.xyz;
}
