#version 330 core
// Version-Info: 130 needed because of datatype "ivec3".

// Attributes: Position, normal, texture coordinates
in vec4  vertex;
in vec3  vnormal;
in vec3  boneIndices;   // Bone indices associated with the vertex, "ivec3" would be better, but Qt has no glVertexAttribIPointer
in vec3  boneWeights;   // Bone weights associated with the vertex

// Same for the whole model or scene: Projection and Modelview matrices
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 bones[120];  // Array of bone transformation matrices
uniform int  numBones;

uniform vec4 modelcolor;  // same color for complete model

// Parameters passed to the fragment shader.
out vec4 fragColor;
out vec3 fragNormal;
out vec4 fragPos;

void main() {

        // Apply bone transformations

        mat4 boneTransform = mat4(0.0);

        for (int i = 0; i < 3; ++i) {
               int boneIndex = int(boneIndices[i]);

               if (boneIndex < 0) break;
               if (boneIndex >= numBones) break;

               float boneWeight = boneWeights[i];
               boneTransform += bones[boneIndex] * boneWeight;
        }

        // Transform the vertex according to modelview
        // fragPos        = (boneTransform * modelview) * vertex;
        // fragPos           = modelview * vertex;
        // boneTransform  = boneTransform*modelview;

        boneTransform = modelview * boneTransform;
        fragPos       = boneTransform * vertex;
        // fragPos       = modelview * vertex;

        // fragNormal   = normalize( (vec4(vnormal, 0.0) * normalview ).xyz );
        vec4 normPoint = vec4(vertex.x + vnormal.x, vertex.y + vnormal.y, vertex.z + vnormal.z, 1);
        vec4 turnNorm  = boneTransform * normPoint;
        fragNormal     = normalize(vec3(turnNorm.x - fragPos.x,turnNorm.y - fragPos.y,turnNorm.z - fragPos.z));

        // fragNormal   = (modelview * vec4(vnormal.x,vnormal.y,vnormal.z,1)).xyz;

        // Project and send to the fragment shader
        fragColor   = modelcolor;

        // gl_Position = projection * modelview * vertex;
        gl_Position = projection * fragPos;
}

