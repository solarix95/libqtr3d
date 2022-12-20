#version 110
// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
attribute vec4 vertex;
attribute vec3 vnormal;
attribute vec2 vtexcoords;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4      projection;
uniform mat4      modelview;
uniform sampler2D textureId;

// Parameters passed to the fragment shader.
varying vec2 fragTexcoords;
varying vec3 fragNormal;
varying vec4 fragPos;

void main() {
        // Transform the vertex according to modelview
        fragPos      = modelview * vertex;
        fragTexcoords= vtexcoords;

        vec4 normPoint = vec4(vertex.x + vnormal.x, vertex.y + vnormal.y, vertex.z + vnormal.z, 1);
        vec4 turnNorm  = modelview * normPoint;
        fragNormal     = normalize(vec3(turnNorm.x - fragPos.x,turnNorm.y - fragPos.y,turnNorm.z - fragPos.z));

        // Project and send to the fragment shader
        gl_Position  = projection * modelview * vertex;
}
