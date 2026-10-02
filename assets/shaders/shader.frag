# version 410 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D u_texture0;

void main() {
    // FragColor = vec4(0.5f, 0.7f, 0.5f, 1.0);
    FragColor = texture(u_texture0, texCoord);
}
