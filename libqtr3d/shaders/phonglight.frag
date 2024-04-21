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

struct Texture {
    sampler2D textureID;
    int       width;
    int       height;
};
uniform Texture texture;

uniform Material material;
uniform Light    light;

// Parameters from the vertex shader
#QTR3d_SHADER_PASS
varying vec3 fragNormal;
varying vec4 fragPos;

//-------------------------------------------------------------------------------------------------
vec4 interpolatedTextureColor(sampler2D id, vec2 coords) {

    // return texture2D(id,coords);

    if (texture.width <= 0)
        return vec4(1,0,0,1);

    if (texture.height <= 0)
        return vec4(1,0,0,1);

    float x = coords[0];
    float y = coords[1];

    float stepX = 1.0/(float)texture.width;
    float stepY = 1.0/(float)texture.height;
    float stepX2 = stepX/2.0;
    float stepY2 = stepY/2.0;

    int pixelX = (int)(x * (float)texture.width);
    int pixelY = (int)(y * (float)texture.height);

    float centerX = (((float)pixelX)/(float)texture.width)  + stepX2;
    float centerY = (((float)pixelY)/(float)texture.height) + stepY2;

    float weightLeft   = 1.0;
    float weightRight  = 1.0;
    float weightTop    = 1.0;
    float weightBottom = 1.0;

    // Left/Right
    if (pixelX == 0 || x > centerX) {               // First column.. or x is right
        weightLeft = 1.0;

        /*
        if (pixelX == 0)
            return vec4(0.5,0.5,0,1);
        return vec4(0,0,1,1);
        */

    }
    if (pixelX > 0 && x < centerX) {
        weightLeft = (stepX2-(centerX-x))/stepX2;

    }


    if (pixelX == texture.width-1 || x < centerX) { // Last column..  or x is left
        weightRight = 1.0;
    }

    if (pixelX < texture.width-1 && x > centerX) {
        weightRight = (stepX2-(x-centerX))/stepX2;
        // return vec4(0,0,1,1);
    }


    // Top/Bottom
    if (pixelY == 0 || y > centerY) {                // First row.. or y is top
        weightBottom = 1.0;
    }
    if (pixelY > 0 && y < centerY) {
        weightBottom = (stepY2-(centerY-y))/stepY2;
    }


    if (pixelY == texture.height-1 || y < centerY) { // Last row..  or y is bottom
        weightTop = 1.0;
    }

    if (pixelY < texture.height-1 && y > centerY) {
        weightTop = (stepY2-(y-centerY))/stepY2;
    }


    // Bi-Linear Filter
    vec4 color11 = texture2D(id, coords);
    vec4 color21;
    vec4 color12;
    vec4 color22;

    float weight1;
    float weight2;
    float weight3;

    // Top-Line
    if (weightTop < 1.0 && weightRight >= 1.0 && weightLeft >= 1.0) {
        vec4 colorTop = (0.5*color11 + 0.5*texture2D(id, vec2(x,y+stepY)));
        return mix(colorTop,color11,weightTop);
    }

    // Bottom-Line
    if (weightBottom < 1.0 && weightRight >= 1.0 && weightLeft >= 1.0) {
        vec4 colorBottom = (0.5*color11 + 0.5*texture2D(id, vec2(x,y-stepY)));
        return mix(colorBottom,color11,weightBottom);
    }

    // Left Line
    if (weightLeft < 1.0 && weightTop >= 1.0 && weightBottom >= 1.0) {
        vec4 colorLeft = (0.5*color11 + 0.5*texture2D(id, vec2(x-stepX,y)));
        return mix(colorLeft,color11,weightLeft);
    }

    // Right-Line
    if (weightRight < 1.0 && weightTop >= 1.0 && weightBottom >= 1.0) {
        vec4 colorRight = (0.5*color11 + 0.5*texture2D(id, vec2(x+stepX,y)));
        return mix(colorRight,color11,weightRight);
    }

    // Top/Right
    if (weightTop < 1.0 && weightRight < 1.0) {
        vec4 colorTop       = (0.5*color11 + 0.5*texture2D(id, vec2(x,       y+stepY)));
        vec4 colorRight     = (0.5*color11 + 0.5*texture2D(id, vec2(x+stepX, y)));
        vec4 colorTopRight  = 0.25*color11 + 0.25*texture2D(id, vec2(x+stepX, y+stepY)) +
                              0.25*texture2D(id, vec2(x,       y+stepY))+ 0.25*texture2D(id, vec2(x+stepX, y));



        vec4 myColorTop    = mix(colorTopRight,colorTop,weightRight);
        vec4 myColorRight  = mix(colorRight, color11,   weightRight);

        // return texture2D(id, vec2(x+stepX, y+stepY));
        return mix(myColorTop,myColorRight,weightTop);
    }



    // Top/Left
    if (weightTop < 1.0 && weightLeft < 1.0) {
        vec4 colorTop       = (0.5*color11 + 0.5*texture2D(id, vec2(x,       y+stepY)));
        vec4 colorLeft      = (0.5*color11 + 0.5*texture2D(id, vec2(x-stepX, y)));
        vec4 colorTopLeft   = 0.25*color11 + 0.25*texture2D(id, vec2(x-stepX, y+stepY)) +
                              0.25*texture2D(id, vec2(x,       y+stepY))+ 0.25*texture2D(id, vec2(x-stepX, y));

        vec4 myColorTop    = mix(colorTopLeft,colorTop,weightLeft);
        vec4 myColorLeft   = mix(colorLeft, color11,   weightLeft);

        // return texture2D(id, vec2(x-stepX, y+stepY));
        return mix(myColorTop,myColorLeft,weightTop);
    }


    // Bottom/Right
    if (weightBottom < 1.0 && weightRight < 1.0) {
        vec4 colorBottom      = (0.5*color11 + 0.5*texture2D(id, vec2(x,       y-stepY)));
        vec4 colorRight       = (0.5*color11 + 0.5*texture2D(id, vec2(x+stepX, y)));
        vec4 colorBottomRight = 0.25*color11 + 0.25*texture2D(id, vec2(x+stepX, y-stepY)) +
                                0.25*texture2D(id, vec2(x,       y-stepY)) + 0.25*texture2D(id, vec2(x+stepX, y));

        vec4 myColorBottom    = mix(colorBottomRight,colorBottom,weightRight);
        vec4 myColorRight     = mix(colorRight, color11,   weightRight);

        return mix(myColorBottom,myColorRight,weightBottom);
    }

    // Bottom/Left
    if (weightBottom < 1.0 && weightLeft < 1.0) {
        vec4 colorBottom     = (0.5*color11 + 0.5*texture2D(id, vec2(x,       y-stepY)));
        vec4 colorLeft       = (0.5*color11 + 0.5*texture2D(id, vec2(x-stepX, y)));
        vec4 colorBottomLeft =  0.25*color11 + 0.25*texture2D(id, vec2(x-stepX, y-stepY)) +
                                0.25*texture2D(id, vec2(x,       y-stepY)) + 0.25*texture2D(id, vec2(x-stepX, y));

        vec4 myColorBottom = mix(colorBottomLeft,colorBottom,weightLeft);
        vec4 myColorLeft   = mix(colorLeft, color11,   weightLeft);

        return mix(myColorBottom,myColorLeft,weightBottom);
    }

    return color11;
}



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
