#version 110

// Parameters from the vertex shader
#QTR3d_SHADER_PASS

void main() {

    vec4 inColor = #QTR3D_FRAGMENT_COLOR

    // Alpha-Transparency
    if(inColor.a < 0.1)
        discard;

    gl_FragColor = inColor;
}
