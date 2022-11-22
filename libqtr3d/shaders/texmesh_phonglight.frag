#version 110
// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec4 fragPos;
varying vec3 fragLightPos;
varying vec3 fragLightAmbientk;
varying vec3 fragLightDiffusek;

// Textures
uniform sampler2D texture;

void main() {
        vec3 lightSpecular = vec3( 0.3, 0.3, 0.3 );

        vec3 materialSpecular = vec3( 1.0, 1.0, 1.0 );
        float materialShinyness = 5.0;
        // vec3 materialSpecular = vec3( 0.0, 0.0, 0.0 );
        // float materialShinyness = 0.0;

        // Ambient lighting
        vec3  materialColor = texture2D( texture, fragTexcoord ).rgb;
        vec3  color    = fragLightAmbientk * materialColor;
        vec3  lightDir = normalize(fragLightPos - fragPos.xyz);

        // Cosine of angle between normal and vector light-vertex
        float lambertTerm = dot( fragNormal, lightDir );

        // Avoid darkening parts and drawing specular highlights wrong
        if( lambertTerm > 0.0 ) {

                // Diffuse lighting
                color += fragLightDiffusek * materialColor * lambertTerm;

                // Specular highlights
                vec3 specDir = normalize( reflect( lightDir, -fragNormal ) );
                float specular =
                        pow( max( 0.0, dot(specDir, normalize(fragPos.xyz)) ), materialShinyness );
                color += lightSpecular *
                        materialSpecular *
                        specular;
        }; // else { color = vec3(1,0,0); }

        gl_FragColor.rgb = color;
}
