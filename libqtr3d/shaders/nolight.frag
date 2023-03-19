#version 110

struct Fog {
    vec4  color;
    float distance;
};
uniform Fog fog;

// Parameters from the vertex shader
#QTR3d_SHADER_PASS
varying vec4 fragPos;

void main() {

    vec4 inColor = #QTR3D_FRAGMENT_COLOR

    // Alpha-Transparency
    if(inColor.a < 0.1)
        discard;

    if (fog.distance > 0.0) {
        float distance = length(fragPos);
        if (distance > fog.distance)
            discard;
        gl_FragColor = mix(inColor, fog.color, distance/fog.distance);
        return;
    }

    gl_FragColor = inColor;
}
