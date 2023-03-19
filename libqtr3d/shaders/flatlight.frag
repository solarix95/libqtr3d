#version 110

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

struct Fog {
    vec4  color;
    float distance;
};

uniform Fog      fog;
uniform Material material;
uniform Light    light;

// Parameters from the vertex shader
#QTR3d_SHADER_PASS
varying vec3 fragNormal;
varying vec4 fragPos;

void main() {
        // fog shortcut
        float distance = length(fragPos);
        if (fog.distance > 0.0 && distance > fog.distance)
            discard;

        // easy ambient color calculation
        vec4  inColor  = #QTR3D_FRAGMENT_COLOR
        vec3  ambient  = (light.ambient * light.color * inColor.rgb) + (material.ambient * inColor.rgb);

        // diffuse color
        vec3  norm     = normalize(fragNormal);
        vec3  lightDir = normalize(light.pos - fragPos.xyz);
        float diff     = max(dot(norm, lightDir), 0.0);
        vec3  diffuse  = diff * material.diffuse * inColor.rgb * light.color;

        vec3  finalColor = (ambient + diffuse);

        // fog color mixer
        if (fog.distance > 0.0)
            finalColor = mix(finalColor, fog.color.rgb, distance/fog.distance);

        // Final output
        gl_FragColor.rgb = finalColor;
        gl_FragColor.a   = inColor.a;
}
