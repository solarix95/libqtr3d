#version 110
// Simple fragment shader.
// Does texturing and phong shading.

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3  pos;       // = WorldMatrix * LightPosition
    vec3  color;
    vec3  ambient;
};

uniform Material material;
uniform Light    light;

// Parameters from the vertex shader
varying vec3 fragColor;
varying vec3 fragNormal;
varying vec4 fragPos;

void main() {

	// Ambient lighting
        vec3  color    = (light.ambient * light.color * fragColor) + (material.ambient * fragColor);
        vec3  lightDir = normalize(light.pos - fragPos.xyz);

	// Cosine of angle between normal and vector light-vertex
        float lambertTerm = dot( fragNormal, lightDir );

	// Avoid darkening parts and drawing specular highlights wrong
	if( lambertTerm > 0.0 ) {

		// Diffuse lighting
                color += material.diffuse * light.color * fragColor * lambertTerm;

		// Specular highlights
                vec3  specDir   = normalize( reflect( lightDir, -fragNormal ) );
                float specular  = pow( max( 0.0, dot(specDir, normalize(fragPos.xyz)) ), material.shininess );
                color += material.specular * specular * light.color * fragColor;
        }; // else { color = vec3(1,0,0); }
	
        gl_FragColor.rgb = vec3(min(color.x,1.0),min(color.y,1.0),min(color.z,1.0)) ;
}
