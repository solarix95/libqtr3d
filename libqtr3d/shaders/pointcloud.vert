#version 330 core
// Version-Info: 130 needed because of datatype "ivec3".

// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
// VBO 1 - Vertex
in vec3  vertex;

// VBO 2 - Color
in vec3   vcolor;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;   // WorldMatrix * ModelView

// Parameters passed to the fragment shader.
out vec4 fragColor;
out vec4 fragPos;

void main() {
    fragPos     = modelview * vec4(vertex.x, vertex.y, vertex.z, 1);

    // send color to the fragment shader
    fragColor   =  vec4(vcolor.x, vcolor.y, vcolor.z, 1);;
    gl_Position = projection * fragPos;
}
