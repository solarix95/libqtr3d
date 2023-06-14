#version 150 core
// Version-Info: 130 needed because of datatype "ivec3".

// Simple vertex shader.
// Transforms and projects vertices and calculates parameters for lighting.

// Attributes: Position, normal, texture coordinates
// VBO 1 - Vertex
in vec4  vertex;
in vec3  vnormal;
in vec3  boneIndices;   // Bone indices associated with the vertex
in vec3  boneWeights;   // Bone weights associated with the vertex

// VBO 2 - Color
in vec4  vcolor;

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;   // WorldMatrix * ModelView
uniform mat4 bones[120];  // Array of bone transformation matrices
uniform int  numBones;

// Parameters passed to the fragment shader.
out vec4 fragColor;
out vec3 fragNormal;
out vec4 fragPos;

void main() {

    // Apply bone transformations
    mat4 boneTransform = mat4(0.0);
    if (numBones > 0) {
        for (int i = 0; i < 3; ++i) {
            int boneIndex = int(boneIndices[i]);

            if (boneIndex < 0) break;
            if (boneIndex >= numBones) break;

            float boneWeight = boneWeights[i];

            boneTransform += bones[boneIndex] * boneWeight;
        }
        boneTransform = modelview * boneTransform;
    } else {
        boneTransform = modelview ;
    }

    fragPos     = boneTransform * vertex;

    vec4 normPoint = vec4(vertex.x + vnormal.x, vertex.y + vnormal.y, vertex.z + vnormal.z, 1);
    vec4 turnNorm  = boneTransform * normPoint;
    fragNormal     = normalize(vec3(turnNorm.x - fragPos.x,turnNorm.y - fragPos.y,turnNorm.z - fragPos.z));

    // send color to the fragment shader
    fragColor   = vcolor;

    // gl_Position = projection * modelview * vertex;
    // gl_Position = projection * boneTransform * vertex;
    gl_Position = projection * fragPos;
}
