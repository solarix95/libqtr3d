#version 110
// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
attribute vec4 vertex;
attribute vec3 vnormal;
attribute vec2 vtexcoords;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;
// uniform mat4 normalview;
uniform vec3 lightpos;   // = LightPosition * modelview
uniform vec3 lightambientk;
uniform vec3 lightdiffusek;

// Parameters passed to the fragment shader.
varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec4 fragPos;
varying vec3 fragLightPos;
varying vec3 fragLightAmbientk;
varying vec3 fragLightDiffusek;

void main() {
        // Transform the vertex according to modelview
        fragPos           = modelview * vertex;
        fragLightPos      = lightpos;
        fragLightAmbientk = lightambientk;
        fragLightDiffusek = lightdiffusek;

        // fragNormal   = normalize( (vec4(vnormal, 0.0) * normalview ).xyz );
        vec4 normPoint = vec4(vertex.x + vnormal.x, vertex.y + vnormal.y, vertex.z + vnormal.z, 1);
        vec4 turnNorm  = modelview * normPoint;
        fragNormal     = normalize(vec3(turnNorm.x - fragPos.x,turnNorm.y - fragPos.y,turnNorm.z - fragPos.z));

        // fragNormal   = (modelview * vec4(vnormal.x,vnormal.y,vnormal.z,1)).xyz;

        // Project and send to the fragment shader
        fragTexcoord = vtexcoords;
        gl_Position  = projection * modelview * vertex;
}
