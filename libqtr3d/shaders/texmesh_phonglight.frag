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
uniform vec3     lightpos;          // = WorldMatrix * LightPosition
uniform vec3     lightambientcolor; // lightcolor * lightambientk

// Parameters from the vertex shader
varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec4 fragPos;

// Textures
uniform sampler2D texture;

void main() {
        vec3 lightSpecular = vec3( 0.3, 0.3, 0.3 );

        vec3 materialSpecular = vec3( 1.0, 1.0, 1.0 );

        // Ambient lighting
        vec3  materialColor = texture2D( texture, fragTexcoord ).rgb;
        vec3  color         = lightambientcolor + (materialColor * material.ambient);

        // Cosine of angle between normal and vector light-vertex
        vec3  lightDir      = normalize(lightpos - fragPos.xyz);
        float lambertTerm = dot( fragNormal, lightDir );

        // Avoid darkening parts and drawing specular highlights wrong
        if( lambertTerm > 0.0 ) {

                // Diffuse lighting
                color += material.diffuse * materialColor * lambertTerm;

                // Specular highlights
                vec3 specDir = normalize( reflect( lightDir, -fragNormal ) );
                float specular =
                        pow( max( 0.0, dot(specDir, normalize(fragPos.xyz)) ), material.shininess );
                color += lightSpecular *
                        materialSpecular *
                        specular;
        }; // else { color = vec3(1,0,0); }

        gl_FragColor.rgb = color;
}
