#version 110

struct Fog {
    vec4  color;
    float distance;
};
uniform Fog fog;

// Parameters from the vertex shader
varying vec4 fragColor;
varying vec4 fragPos;

void main() {

    vec4 inColor = fragColor;

    // Alpha-Transparency

    /*
    if(inColor.a < 0.1)
        discard;

    if (fog.distance > 0.0) {
        float distance = length(fragPos);
        if (distance > fog.distance)
            discard;
        gl_FragColor = mix(inColor, fog.color, distance/fog.distance);
        return;
    }
    */

    gl_FragColor = inColor;
}
