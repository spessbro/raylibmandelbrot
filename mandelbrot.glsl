#version 400

in vec4 gl_FragCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform vec2 offsetCombined;
uniform vec2 screenSize;
uniform float zoom;

#define ITERATION_MAX 256
#define ITERATION_INC 1

void main() {

    vec2 uv = gl_FragCoord.xy / screenSize;

    vec2 C = vec2(
        (gl_FragCoord.x/screenSize.x)*zoom-offsetCombined.x,
        (gl_FragCoord.y/screenSize.y)*zoom-offsetCombined.y
        );

    vec2 Z    = vec2(0.0,0.0);
    vec2 Zsqr = vec2(0.0,0.0);

    int iteration = 0;
    for (iteration = 0;iteration < ITERATION_MAX;iteration+=ITERATION_INC) {
        Zsqr = vec2 (
            Z.x*Z.x,
            Z.y*Z.y
            );
        Z = vec2(
            Zsqr.x - Zsqr.y,
            (Z.x + Z.x) * Z.y
            ) + C;

        if ((Zsqr.x + Zsqr.y) > 4.0) break;
    }

    float angle = 2 * radians(180) * float(iteration) / float(ITERATION_MAX);

    finalColor.r = 1.0 * (2 * sin(15 * angle + 1) + 1);
    finalColor.g = 1.0 * (2 * sin(10 * angle + 1) + 1);
    finalColor.b = 1.0 * (2 * sin(5  * angle + 1) + 1);
    finalColor.a = 1.0;

}
