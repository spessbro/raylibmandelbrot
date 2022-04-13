#version 400

in vec4 gl_FragCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform vec2 offset_combined;
uniform vec2 screen_size;
uniform float zoom;

#define ITERATION_MAX 1
#define ITERATION_INC 0.001

void main() {
    vec2 C = vec2(
        (gl_FragCoord.x/screen_size.x)*zoom-offset_combined.x,
        (gl_FragCoord.y/screen_size.y)*zoom-offset_combined.y);
    vec2 Z = vec2(0.0,0.0);
    float iteration = 0;
    for (iteration = 0;iteration < ITERATION_MAX;iteration+=ITERATION_INC) {
        Z = vec2(
            Z.x*Z.x - Z.y*Z.y,
            (Z.x + Z.x) * Z.y
            ) + C;

        if ((Z.x*Z.x + Z.y*Z.y) > 4.0) break;
    }
    
    float color = iteration;
    finalColor = vec4(color,color,color, 1.0);
}
