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

    //return texture2D(id, coords);

    if (texture.width <= 0)
        return vec4(1,0,0,1);

    if (texture.height <= 0)
        return vec4(1,0,0,1);

    float x = coords[0];
    float y = coords[1];

    vec4 color = vec4(0.0);

    float stepX = 1.0/(float)texture.width;
    float stepY = 1.0/(float)texture.height;
    float stepX2 = stepX/2.0;
    float stepY2 = stepY/2.0;

    int pixelX = (int)(x * (float)texture.width);
    int pixelY = (int)(y * (float)texture.height);

    /*
    #extension GL_EXT_gpu_shader4 : enable
    if ((pixelX % 2) == 0)
        return vec4(1,0,0,1);
    */

    float centerX = (((float)pixelX)/(float)texture.width)  + stepX2;
    float centerY = (((float)pixelY)/(float)texture.height) + stepY2;

    float weightLeft  = 1.0;
    float weightRight = 1.0;

    /*

     // Debug only

    if (pixelX == 0)
        return vec4(0,0,1,1);

    if (pixelX == 1)
        return vec4(0,1,1,1);
    */

    /*

      Debug and verify calculation: split rectangle into 4 sub rectangles

    if (coords[0] > centerX && coords[1] > centerY) // right, top
        return vec4(1,0,0,1);
    if (coords[0] > centerX && coords[1] < centerY) // right, bottom
        return vec4(0,1,0,1);
    if (coords[0] <= centerX && coords[1] > centerY) // left, top
        return vec4(0,0,1,1);
    if (coords[0] <= centerX && coords[1] < centerY) // left, bottom
        return vec4(1,1,1,1);
    */


    if (pixelX == 0 || x > centerX) {               // First column.. or x is right
        weightLeft = 1.0;
    }

    if (pixelX == texture.width-1 || x < centerX) { // Last column..  or x is left
        weightRight = 1.0;
    }

    if (x > centerX) {
        weightRight = 0.5 + 0.5*(stepX2-(x-centerX))/stepX2;
    }

    if (x < centerX) {
        weightLeft = 0.5 + 0.5*(stepX2-(centerX-x))/stepX2;
    }

    /*
    if (weightLeft < 1.0) {
        // return vec4(2.0*(weightLeft-0.5),2.0*(weightLeft-0.5),2.0*(weightLeft-0.5),1);
        return (weightLeft * texture2D(id, coords)) + ((1.0-weightLeft) * texture2D(id, vec2(x-stepX,y)));
    }
    if (weightRight < 1.0) {
        // return vec4(0,1,0,1);
        return (weightRight * texture2D(id, coords)) + ((1.0-weightRight) * texture2D(id, vec2(x+stepX,y)));
    }
    */

    float weightTop    = 1.0;
    float weightBottom = 1.0;


    if (pixelY == 0 || y > centerY) {                // First row.. or y is top
        weightBottom = 1.0;
    }

    if (pixelY == texture.height-1 || y < centerY) { // Last row..  or y is bottom
        weightTop = 1.0;
    }

    if (y > centerY) {
        weightTop = 0.5 + 0.5*(stepY2-(y-centerY))/stepY2;
    }

    if (y < centerY) {
        weightBottom = 0.5 + 0.5*(stepY2-(centerY-y))/stepY2;
    }

    vec4 colorTop = vec4(0);
    if (weightTop < 1.0) {
        colorTop = ((1.0-weightTop) * texture2D(id, vec2(x,y+stepY)));
    }

    vec4 colorBottom = vec4(0);
    if (weightBottom < 1.0) {
        colorBottom = ((1.0-weightBottom) * texture2D(id, vec2(x,y-stepY)));
    }


    vec4 colorLeft = vec4(0);
    if (weightLeft < 1.0) {
        colorLeft = ((1.0-weightLeft) * texture2D(id, vec2(x-stepX,y)));
    }

    vec4 colorRight = vec4(0);
    if (weightRight < 1.0) {
        // return vec4(0,1,0,1);
        colorRight = ((1.0-weightRight) * texture2D(id, vec2(x+stepX,y)));
    }


    // top/right
    if (weightRight < 1.0 && weightTop < 1.0) {
        return colorRight + colorTop +
               (weightRight + weightTop - 1.0)* texture2D(id, coords);
    }

    // top/left
    if (weightLeft < 1.0 && weightTop < 1.0) {
        return colorLeft + colorTop +
               (weightLeft + weightTop - 1.0)* texture2D(id, coords);
    }

    // bottom/right
    if (weightRight < 1.0 && weightBottom < 1.0) {
        return colorRight + colorBottom +
               (weightRight + weightBottom - 1.0)* texture2D(id, coords);
    }

    // bottom/left
    if (weightLeft < 1.0 && weightBottom < 1.0) {
        return colorLeft + colorBottom +
               (weightLeft + weightBottom - 1.0)* texture2D(id, coords);
    }

    return texture2D(id, coords);

    /*
    float offsetX = 2.0/(float)texture.width;
    float offsetY = 2.0/(float)texture.height;

    float samples = 0.0;
    for(float x = -offsetX; x <= offsetX; x += stepX) {
        for(float y = -offsetY; y <= offsetY; y += stepY) {
            color += texture2D(id, coords + vec2(x, y));
            samples += 1.0;
        }
    }
    return color/samples;
    */
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
