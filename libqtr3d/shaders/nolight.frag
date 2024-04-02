#version 110



struct Fog {
    vec4  color;
    float distance;
};
uniform Fog     fog;

struct Texture {
    sampler2D textureID;
    int       width;
    int       height;
};
uniform Texture texture;


// Parameters from the vertex shader
#QTR3d_SHADER_PASS
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




    return color11;

    /*


    // Left-Line
    if (weightLeft < 1.0 && weightTop >= 1.0 && weightBottom >= 1.0) {
        // return mix(myColor,colorTop,weightTop);
        return mix(colorLeft,myColor,weightLeft);
    }
    */

}

//-------------------------------------------------------------------------------------------------
vec4 interpolatedTextureColor4(sampler2D id, vec2 coords) {
    vec2 textureSize = vec2((float)texture.width,(float)texture.height);
    vec2 texelSize = 1.0 / textureSize; // Calculate the size of a texel

    vec2 f = fract( coords * textureSize );
    coords += ( .5 - f ) * texelSize;    // move uv to texel centre
    vec4 tl = texture2D(id, coords);
    vec4 tr = texture2D(id, coords + vec2(texelSize.x, 0.0));
    vec4 bl = texture2D(id, coords + vec2(0.0, texelSize.y));
    vec4 br = texture2D(id, coords + vec2(texelSize.x, texelSize.y));
    vec4 tA = mix( tl, tr, f.x );
    vec4 tB = mix( bl, br, f.x );
    return mix( tA, tB, f.y );
}

//-------------------------------------------------------------------------------------------------
vec4 interpolatedTextureColor3(sampler2D id, vec2 coords) {

    vec4 myColor = texture2D(id, coords);

    vec2 textureSize = vec2((float)texture.width,(float)texture.height);
    vec2 texelSize = 1.0 / textureSize; // Calculate the size of a texel

    vec2 texelPos1 = coords - texelSize;
    vec2 texelPos2 = coords;
    vec2 texelPos3 = coords + vec2(texelSize.x, 0);
    vec2 texelPos4 = coords + vec2(2.0 * texelSize.x, 0);
    vec2 texelPos5 = coords + vec2(2.0 * texelSize.x, texelSize.y);
    vec2 texelPos6 = coords + vec2(texelSize.x, texelSize.y);
    vec2 texelPos7 = coords + vec2(0, texelSize.y);
    vec2 texelPos8 = coords + vec2(0, 2.0 * texelSize.y);

    vec4 texel1 = texture2D(id, texelPos1);
    vec4 texel2 = texture2D(id, texelPos2);
    vec4 texel3 = texture2D(id, texelPos3);
    vec4 texel4 = texture2D(id, texelPos4);
    vec4 texel5 = texture2D(id, texelPos5);
    vec4 texel6 = texture2D(id, texelPos6);
    vec4 texel7 = texture2D(id, texelPos7);
    vec4 texel8 = texture2D(id, texelPos8);

    // Calculate the weights for each texel based on the distance from the current texture coordinate
    vec2 weights1 = fract(textureSize * texelPos1);
    vec2 weights2 = fract(textureSize * texelPos2);
    vec2 weights3 = fract(textureSize * texelPos3);
    vec2 weights4 = fract(textureSize * texelPos4);
    vec2 weights5 = fract(textureSize * texelPos5);
    vec2 weights6 = fract(textureSize * texelPos6);
    vec2 weights7 = fract(textureSize * texelPos7);
    vec2 weights8 = fract(textureSize * texelPos8);

    // Perform bilinear interpolation
    vec4 finalColor = mix(
                mix(mix(texel8, texel7, weights8.x), mix(texel6, texel5, weights6.x), weights6.y),
                mix(mix(texel4, texel3, weights4.x), mix(texel2, texel1, weights2.x), weights2.y),
                weights2.y
                );

    return finalColor;

}
//-------------------------------------------------------------------------------------------------
vec4 interpolatedTextureColor2(sampler2D id, vec2 coords) {

    vec4 myColor = texture2D(id, coords);
    return myColor;

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

    vec4 colorTop   = myColor;
    if (weightTop < 1.0) {
        colorTop = texture2D(id, vec2(x,y+stepY));
    }

    vec4 colorBottom = myColor;
    if (weightBottom < 1.0) {
        colorBottom = texture2D(id, vec2(x,y-stepY));
    }

    vec4 colorLeft = myColor;
    if (weightLeft < 1.0) {
        colorLeft = texture2D(id, vec2(x-stepX,y));
    }

    vec4 colorRight = myColor;
    if (weightRight < 1.0) {
        // return vec4(0,1,0,1);
        colorRight = texture2D(id, vec2(x+stepX,y));
    }

    vec4 colorTopRight = myColor;
    if (weightTop < 1.0 && weightRight < 1.0) {
        colorTopRight = texture2D(id, vec2(x+stepX,y+stepY));
    }

    vec4 colorTopLeft = myColor;
    if (weightTop < 1.0 && weightLeft < 1.0) {
        colorTopLeft = texture2D(id, vec2(x-stepX,y+stepY));
    }

    vec4 colorBottomRight = myColor;
    if (weightBottom < 1.0 && weightRight < 1.0) {
        colorBottomRight = texture2D(id, vec2(x+stepX,y-stepY));
    }

    vec4 colorBottomLeft = myColor;
    if (weightBottom < 1.0 && weightLeft < 1.0) {
        colorBottomLeft = texture2D(id, vec2(x-stepX,y-stepY));
    }


    // Top-Line
    if (weightTop < 1.0 && weightRight >= 1.0 && weightLeft >= 1.0) {
        // return mix(myColor,colorTop,weightTop);
        return mix(colorTop,myColor,weightTop);
    }
    // Bottom-Line
    if (weightBottom < 1.0 && weightRight >= 1.0 && weightLeft >= 1.0) {
        // return mix(myColor,colorTop,weightTop);
        return mix(colorBottom,myColor,weightBottom);
    }
    // Right-Line
    if (weightRight < 1.0 && weightTop >= 1.0 && weightBottom >= 1.0) {
        // return mix(myColor,colorTop,weightTop);
        return mix(colorRight,myColor,weightRight);
    }
    // Left-Line
    if (weightLeft < 1.0 && weightTop >= 1.0 && weightBottom >= 1.0) {
        // return mix(myColor,colorTop,weightTop);
        return mix(colorLeft,myColor,weightLeft);
    }

    // Top/right
    if (weightTop < 1.0 && weightRight < 1.0) {
        vec4 wColorTop   = mix(colorTop,  myColor,weightTop);
        vec4 wColorRight = mix(colorRight,myColor,weightRight);

        return mix(colorTop,myColor,weightTop);
    }



    return myColor;

    if (weightTop < 1.0 && weightRight < 1.0) {

        return mix(mix(colorTop,myColor,weightTop),
                   colorRight, weightRight);
    }


    // top/right
    if (weightRight < 1.0 && weightTop < 1.0) {
        vec4 topRightColor = texture2D(id, vec2(x+stepX,y+stepY));



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

//-------------------------------------------------------------------------------------------------
void main() {

    vec4 inColor = #QTR3D_FRAGMENT_COLOR

            // Alpha-Transparency
            if(inColor.a < 0.1)
            discard;

    if (fog.distance > 0.0) {
        float distance = length(fragPos);
        if (distance > fog.distance)
            discard;
        gl_FragColor = mix(inColor, fog.color, distance/fog.distance);
        return;
    }

    gl_FragColor = inColor;
}
