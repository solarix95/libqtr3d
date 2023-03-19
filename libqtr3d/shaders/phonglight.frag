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

        vec4  inColor  = #QTR3D_FRAGMENT_COLOR

	// Ambient lighting
        vec3  color    = (light.ambient * light.color * inColor.rgb) + (material.ambient * inColor.rgb);
        vec3  lightDir = normalize(light.pos - fragPos.xyz);

	// Cosine of angle between normal and vector light-vertex
        float lambertTerm = dot( fragNormal, lightDir );

	// Avoid darkening parts and drawing specular highlights wrong
	if( lambertTerm > 0.0 ) {

		// Diffuse lighting
                color += material.diffuse * light.color * inColor.rgb * lambertTerm;

		// Specular highlights
                vec3  specDir   = normalize( reflect( lightDir, -fragNormal ) );
                float specular  = pow( max( 0.0, dot(specDir, normalize(fragPos.xyz)) ), material.shininess );
                color += material.specular * specular * light.color * inColor.rgb;
        }; // else { color = vec3(1,0,0); }
	
        vec3 finalColor = vec3(min(color.x,1.0),min(color.y,1.0),min(color.z,inColor.a));

        // fog color mixer
        if (fog.distance > 0.0)
            finalColor = mix(finalColor, fog.color.rgb, distance/fog.distance);

        gl_FragColor.rgb = finalColor;
}
