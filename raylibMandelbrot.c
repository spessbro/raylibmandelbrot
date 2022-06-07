#include <raylib.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define SHADER_NF_ERROR_V "ERROR CHECK LOG"

int main() {

    /* make sure window is square! */
    const int screenWidth    = 1024;
    const int screenHeight   = 1024;

    //SetTraceLogLevel(LOG_ERROR);
    InitWindow(screenWidth, screenHeight, "Fractal Viewer");
    SetTargetFPS(60);

    Shader mandelbrotShader = LoadShader(0, "mandelbrot.glsl");

    int offsetCombinedLoc  = GetShaderLocation(mandelbrotShader, "offsetCombined");
    int screenCombinedLoc  = GetShaderLocation(mandelbrotShader, "screenSize");
    int zoomLoc             = GetShaderLocation(mandelbrotShader, "zoom");

    bool errorState = false;
    if ((offsetCombinedLoc == -1) || (screenCombinedLoc == -1) || (zoomLoc == -1)) {
        TraceLog(LOG_ERROR,"Shader not found or is corrupted");
        errorState = true;
    }

    float zoom               = 3.0;
    float CxOffset          = 2.2;
    float CyOffset          = 1.5;
    float offsetModifier    = 0.1;

    bool redraw              = true;
    bool screenshotRedraw   = false;
    bool informationDisplay = true;
    time_t lastInftogTime  = time(NULL);


    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_KP_1))  {offsetModifier += 0.001;}
        if (IsKeyDown(KEY_KP_2))  {offsetModifier -= 0.001;}
        if (IsKeyDown(KEY_KP_4))  {offsetModifier += 0.00001;}
        if (IsKeyDown(KEY_KP_5))  {offsetModifier -= 0.00001;}
        if (IsKeyDown(KEY_KP_3))  {zoom -= offsetModifier; redraw = true;}
        if (IsKeyDown(KEY_KP_6))  {zoom += offsetModifier; redraw = true;}
        if (IsKeyDown(KEY_UP))    {CyOffset -= offsetModifier; redraw = true;}
        if (IsKeyDown(KEY_DOWN))  {CyOffset += offsetModifier; redraw = true;}
        if (IsKeyDown(KEY_LEFT))  {CxOffset += offsetModifier; redraw = true;}
        if (IsKeyDown(KEY_RIGHT)) {CxOffset -= offsetModifier; redraw = true;}
        if (IsKeyDown(KEY_SPACE)) {screenshotRedraw = true; redraw = true;}
        if (IsKeyDown(KEY_H))     {
            if (time(NULL) > lastInftogTime) {
                informationDisplay = !informationDisplay;
                lastInftogTime = time(NULL);
            }
        }

        BeginDrawing(); {

            BeginShaderMode(mandelbrotShader); {
                if (redraw) {
                    float offsetCombined[2] = {CxOffset, CyOffset};
                    float screenCombined[2] = {(float)screenWidth, (float)screenHeight};

                    SetShaderValue(mandelbrotShader, offsetCombinedLoc, offsetCombined, SHADER_UNIFORM_VEC2);
                    SetShaderValue(mandelbrotShader, screenCombinedLoc, screenCombined, SHADER_UNIFORM_VEC2);
                    SetShaderValue(mandelbrotShader, zoomLoc, &zoom, SHADER_UNIFORM_FLOAT);
                    redraw = false;
                }
                DrawRectangle(0,0,screenWidth,screenHeight,ORANGE);
            } EndShaderMode();

            if(errorState) {
                DrawText(SHADER_NF_ERROR_V,(screenWidth-MeasureText(SHADER_NF_ERROR_V,20))/2,screenHeight/2,20,RED);
            }
            if (screenshotRedraw) {
                TakeScreenshot("output.png");
                screenshotRedraw = false;
            } else {
                DrawFPS(10,0);
                DrawText(
                        TextFormat("Cx %f Cy %f\nzoom %f offset %f",CxOffset,CyOffset,zoom,offsetModifier)
                        ,10,30,20,GREEN);

                if (informationDisplay) {
                    DrawText(
                        "Controls:\n1/2: Large speed adjust\n4/5: Minor speed adjust\n6/3: Zoom\nArrow keys: Move\nh: Toggle help"
                        ,10,90,20,BLUE);
                }
            }
        } EndDrawing();
    }

    UnloadShader(mandelbrotShader);
    CloseWindow();
    TraceLog(LOG_INFO, "Exiting");

    return 0;
}
