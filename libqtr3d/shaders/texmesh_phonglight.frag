#version 110
// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec2 texcoord;
varying vec3 eye;
varying vec3 light;
varying vec3 normal;

// Textures
uniform sampler2D texture;

void main() {
    // vec3 lightAmbient = vec3( 0.2, 0.2, 0.2 );
    // vec3 lightDiffuse = vec3( 0.5, 0.5, 0.5 );
    // vec3 lightSpecular = vec3( 0.3, 0.3, 0.3 );

    vec3 lightAmbient = vec3( 0.1, 0.1, 0.1 );
    vec3 lightDiffuse = vec3( 0.7, 0.7, 0.7 );
    vec3 lightSpecular = vec3( 0.3, 0.3, 0.3 );

    vec3 materialColor = texture2D( texture, texcoord ).rgb;
    vec3 materialSpecular = vec3( 1.0, 1.0, 1.0 );
    float materialShinyness = 7.0;

    // Ambient lighting
    vec3 color = lightAmbient * materialColor;

    // Cosine of angle between normal and vector light-vertex
    float lambertTerm = dot( normal, light );

    // Avoid darkening parts and drawing specular highlights wrong
    if( lambertTerm > 0.0 ) {

        // Diffuse lighting
        color += lightDiffuse * materialColor * lambertTerm;

        // Specular highlights
        vec3 specDir = normalize( reflect( light, -normal ) );
        float specular =
                pow( max( 0.0, dot(specDir, eye) ), materialShinyness );
        color += lightSpecular *
                materialSpecular *
                specular;
    }


    gl_FragColor.rgb = color;
}
