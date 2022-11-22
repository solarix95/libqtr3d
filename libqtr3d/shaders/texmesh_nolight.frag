#version 130
out vec4 FragColor;

// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec2 fragTexcoords;

// Textures
uniform sampler2D texture;

void main() {

    // vec3 materialColor = texture2D( texture, texcoord ).rgb;
    // gl_FragColor.rgb = materialColor;

    vec4 materialColor = texture2D( texture, fragTexcoords );

    // Alpha-Transparency
    if(materialColor.a < 0.1)
            discard;

    FragColor = materialColor;
}
