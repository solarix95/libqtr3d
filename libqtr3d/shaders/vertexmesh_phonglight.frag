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
varying vec3 fragColor;
varying vec3 fragNormal;
varying vec4 fragPos;

void main() {

	// Ambient lighting
        vec3  color    = lightambientcolor + (fragColor * material.ambient);
        vec3  lightDir = normalize(lightpos - fragPos.xyz);

	// Cosine of angle between normal and vector light-vertex
        float lambertTerm = dot( fragNormal, lightDir );

	// Avoid darkening parts and drawing specular highlights wrong
	if( lambertTerm > 0.0 ) {

		// Diffuse lighting
                color += material.diffuse * fragColor * lambertTerm;

		// Specular highlights
                vec3  specDir   = normalize( reflect( lightDir, -fragNormal ) );
                float specular  = pow( max( 0.0, dot(specDir, normalize(fragPos.xyz)) ), material.shininess );
                color += material.specular * specular;
        }; // else { color = vec3(1,0,0); }
	
        gl_FragColor.rgb = vec3(min(color.x,1.0),min(color.y,1.0),min(color.z,1.0)) ;
}
