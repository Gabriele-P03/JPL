#version 330 core
in vec2 vUV;
in vec4 vColor;

uniform sampler2D uAtlas;              // atlas a canale singolo (GL_RED)

out vec4 FragColor;

void main() {
    float a = texture(uAtlas, vUV).r;
    FragColor = vec4(vColor.rgb, vColor.a * a);
}
