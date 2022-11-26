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
varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec4 fragPos;

// Textures
uniform sampler2D texture;

void main() {
        // Ambient lighting
        vec3  materialColor = texture2D( texture, fragTexcoord ).rgb;
        vec3  color    = (light.ambient * light.color * materialColor) + (material.ambient * materialColor);

        // Cosine of angle between normal and vector light-vertex
        vec3  lightDir      = normalize(light.pos - fragPos.xyz);
        float lambertTerm   = dot( fragNormal, lightDir );

        // Avoid darkening parts and drawing specular highlights wrong
        if( lambertTerm > 0.0 ) {

                // Diffuse lighting
                color += material.diffuse * light.color * materialColor * lambertTerm;

                // Specular highlights
                vec3  specDir   = normalize( reflect( lightDir, -fragNormal ) );
                float specular  = pow( max( 0.0, dot(specDir, normalize(fragPos.xyz)) ), material.shininess );
                color += material.specular * specular * light.color * materialColor;
        }; // else { color = vec3(1,0,0); }

        gl_FragColor.rgb = color;
}
