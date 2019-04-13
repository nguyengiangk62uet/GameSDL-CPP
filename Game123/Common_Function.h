//Những thành phần sử dụng chung đưa vào 1 namespace để gọi

#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <SDL.h>
#include <string>
#include <windows.h>
#include <SDL_image.h>

const int BACKGROUND_HEIGHT = 3350;
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 670;
const int SCREEN_BPP = 32;
const int NUM_THREAT = 3; // Số lượng quái
const int SPEED_MAIN = 8;

static SDL_Surface *g_screen = NULL;
static SDL_Surface *g_bkground = NULL;
static SDL_Event g_even;

namespace SDLCommonFunc //Tránh trùng lặp
{
    SDL_Surface* load_image(std::string file_path);
    void ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y);
    void ApplySurfaceClip(SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip, int x, int y);
    void CleanUp();
    bool CheckCollision(const SDL_Rect& object_1, const SDL_Rect& object_2);
}




#endif // COMMON_FUNCTION_H_
