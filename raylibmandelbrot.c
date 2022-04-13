#include <raylib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define GLSL_VERSION 330
#define SHADER_NF_ERROR_V "ERROR CHECK LOG"

int main() {

    /* make sure window is square! */
    const int screenwidth    = 1024;
    const int screenheight   = 1024;

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(screenwidth, screenheight, "Fractal Viewer");
    SetTargetFPS(60);

    Shader mandelbrot_shader = LoadShader(0, "mandelbrot.frag");

    int offset_combinedLoc  = GetShaderLocation(mandelbrot_shader, "offset_combined");
    int screen_combinedLoc  = GetShaderLocation(mandelbrot_shader, "screen_size");
    int zoomLoc             = GetShaderLocation(mandelbrot_shader, "zoom");

    bool error_state = false;
    if ((offset_combinedLoc == -1) || (screen_combinedLoc == -1) || (zoomLoc == -1)) {
        TraceLog(LOG_ERROR,"Shader not found or is corrupted");
        error_state = true;
    }

    float zoom               = 3.0;
    float Cx_offset          = 2.2;
    float Cy_offset          = 1.5;
    float offset_modifier    = 0.1;

    bool redraw              = true;
    bool screenshot_redraw   = false;
    bool information_display = true;
    time_t last_inftog_time    = time(NULL);


    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_KP_1))  {offset_modifier += 0.001;}
        if (IsKeyDown(KEY_KP_2))  {offset_modifier -= 0.001;}
        if (IsKeyDown(KEY_KP_4))  {offset_modifier += 0.00001;}
        if (IsKeyDown(KEY_KP_5))  {offset_modifier -= 0.00001;}
        if (IsKeyDown(KEY_KP_3))  {zoom -= offset_modifier; redraw = true;}
        if (IsKeyDown(KEY_KP_6))  {zoom += offset_modifier; redraw = true;}
        if (IsKeyDown(KEY_UP))    {Cy_offset -= offset_modifier; redraw = true;}
        if (IsKeyDown(KEY_DOWN))  {Cy_offset += offset_modifier; redraw = true;}
        if (IsKeyDown(KEY_LEFT))  {Cx_offset += offset_modifier; redraw = true;}
        if (IsKeyDown(KEY_RIGHT)) {Cx_offset -= offset_modifier; redraw = true;}
        if (IsKeyDown(KEY_SPACE)) {screenshot_redraw = true; redraw = true;}
        if (IsKeyDown(KEY_H))     {
            if (time(NULL) > last_inftog_time) {
                information_display = !information_display;
                last_inftog_time = time(NULL);
            }
        }

        BeginDrawing(); {

            BeginShaderMode(mandelbrot_shader); {
                if (redraw) {
                    float offset_combined[2] = {Cx_offset, Cy_offset};
                    float screen_combined[2] = {(float)screenwidth, (float)screenheight};

                    SetShaderValue(mandelbrot_shader, offset_combinedLoc, offset_combined, SHADER_UNIFORM_VEC2);
                    SetShaderValue(mandelbrot_shader, screen_combinedLoc, screen_combined, SHADER_UNIFORM_VEC2);
                    SetShaderValue(mandelbrot_shader, zoomLoc, &zoom, SHADER_UNIFORM_FLOAT);
                    redraw = false;
                }
                DrawRectangle(0,0,screenwidth,screenheight,ORANGE);
            } EndShaderMode();

            if(error_state) {
                DrawText(SHADER_NF_ERROR_V,(screenwidth-MeasureText(SHADER_NF_ERROR_V,20))/2,screenheight/2,20,RED);
            }
            if (screenshot_redraw) {
                TakeScreenshot("output.png");
                screenshot_redraw = false;
            } else {
                DrawFPS(10,0);
                DrawText(
                        TextFormat("Cx %f Cy %f\nzoom %f offset %f",Cx_offset,Cy_offset,zoom,offset_modifier)
                        ,10,30,20,GREEN);

                if (information_display) {
                    DrawText(
                        "Controls:\n1/2: Large speed adjust\n4/5: Minor speed adjust\n6/3: Zoom\nArrow keys: Move\nh: Toggle help"
                        ,10,90,20,BLUE);
                }
            }
        } EndDrawing();
    }

    CloseWindow();

    return 0;
}
