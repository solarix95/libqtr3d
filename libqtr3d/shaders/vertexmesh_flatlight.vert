#version 150
// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
attribute vec4 vertex;
attribute vec3 vnormal;
attribute vec3 vcolor;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;  // = WorldMatrix * ModelView
uniform vec3 lightpos;   // = WorldMatrix * LightPosition
uniform vec3 lightambientk;
uniform vec3 lightdiffusek;

// Parameters passed to the fragment shader.
varying vec3 fragColor;
varying vec3 fragNormal;
varying vec4 fragPos;
varying vec3 fragLightPos;
varying vec3 fragLightAmbientk;
varying vec3 fragLightDiffusek;

void main() {
        //calculate normal in world coordinates
        // mat3 normalMatrix = transpose(inverse(mat3(modelview)));

        // Lighting
        fragLightPos      = lightpos; // lightpos = modelviewLightpos
        fragLightAmbientk = lightambientk;
        fragLightDiffusek = lightdiffusek;

        // "vnormal" is a vector.. we can only transform "points":
        vec4 normPoint = vec4(vertex.x + vnormal.x, vertex.y + vnormal.y, vertex.z + vnormal.z, 1);
        vec4 turnNorm  = modelview * normPoint;
        fragPos        = modelview * vertex;
        fragNormal     = vec3(turnNorm.x - fragPos.x,turnNorm.y - fragPos.y,turnNorm.z - fragPos.z );

        // Standard shader output
        fragColor   = vcolor;
        gl_Position = projection * modelview * vertex;
}
