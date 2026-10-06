#version 330 core
layout(location = 0) in vec2 aQuad;    // quad unitario (0..1)
layout(location = 1) in vec2 iPos;     // per istanza
layout(location = 2) in vec2 iSize;
layout(location = 3) in vec4 iUV;      // u0, v0, u1, v1
layout(location = 4) in vec4 iColor;

uniform vec2 uScreen;                  // dimensioni in pixel

out vec2 vUV;
out vec4 vColor;

void main() {
    vec2 p = iPos + aQuad * iSize;
    gl_Position = vec4(p.x / uScreen.x * 2.0 - 1.0,
                       1.0 - p.y / uScreen.y * 2.0, 0.0, 1.0);
    vUV = mix(iUV.xy, iUV.zw, aQuad);
    vColor = iColor;
}
