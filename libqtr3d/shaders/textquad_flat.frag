#version 130
out vec4 FragColor;

// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec2 texcoord;

// Textures
uniform sampler2D texture;

void main() {

    // vec3 materialColor = texture2D( texture, texcoord ).rgb;
    // gl_FragColor.rgb = materialColor;

    vec4 materialColor = texture2D( texture, texcoord );

    // Alpha-Transparency
    if(materialColor.a < 0.1)
            discard;

    FragColor = materialColor;
}
