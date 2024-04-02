#version 150 core
// Version-Info: 130 needed because of datatype "ivec3".

// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
attribute vec4  vertex;
attribute vec3  vnormal;
attribute vec2  vtexcoords;
attribute ivec3 boneIndices;   // Bone indices associated with the vertex
attribute vec3  boneWeights;   // Bone weights associated with the vertex

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4      projection;
uniform mat4      modelview;
uniform mat4      bones[120];  // Array of bone transformation matrices
uniform int       numBones;
uniform int       pushToBack;

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
        // gl_Position   = projection * modelview * vertex; // * vec4(vertex.x, vertex.y, vertex.z, 1); // vertex;


        /*
        if (pushToBack > 0)
            gl_Position.z = gl_Position.w - 0.0001;
        */
        gl_Position   = projection * modelview * vec4(vertex.x, vertex.y, vertex.z, 1);

        float z_ndc = gl_Position.z / gl_Position.w;
        if (z_ndc >= 1) {
            // Adjust z to be just slightly less than w, keeping it within the far clipping range
            gl_Position.z = gl_Position.w - 0.0001;
        }


}
