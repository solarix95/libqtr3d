#version 110
// Simple fragment shader.
// Does texturing and phong shading.

// Parameters from the vertex shader
varying vec3 color;
varying vec3 normal;
varying vec4 fragpos;
varying vec3 lightPos;

void main() {
        // easy ambient color calculation
        vec3  ambientColor = vec3(1,0.6,0);
        float ambientStrength = 0.3;
        vec3  ambient = ambientStrength * ambientColor;

        // diffuse color

        vec3 norm     = normalize(normal);
        vec3 lightDir = normalize(lightPos - vec3(fragpos.x, fragpos.y, fragpos.z));
        float diff    = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * ambientColor;

        vec3 result = (ambient + diffuse) * color;
        gl_FragColor.rgb = vec3( result.x, result.y, result.z );   // = color; // vec3( 0.3, 0.9, 0.9 );
}
