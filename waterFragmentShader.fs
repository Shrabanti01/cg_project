#version 330 core

out vec4 FragColor;

in vec2 TexCoord; // Interpolated texture coordinates from the vertex shader

uniform sampler2D waterTexture; // The water texture sampler
uniform float time; // Time for wave animation


void main() {
    // Basic wave effect by modifying texture coordinates
    float wave = sin(10.0 * TexCoord.x + time*0.5) * 0.05; // Add wave distortion
    vec2 distortedTexCoord = TexCoord + vec2(time * 0.01,wave); // Offset the texture coordinates

    // Sample the water texture with distorted coordinates
    vec4 waterColor = texture(waterTexture, distortedTexCoord);

    FragColor = waterColor; // Set the output color
}
