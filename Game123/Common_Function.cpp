#include "Common_Function.h"

SDL_Surface* SDLCommonFunc::load_image(std::string file_path)
{
    SDL_Surface* loadImage = NULL;
    SDL_Surface* optimizeImage = NULL;
    loadImage = IMG_Load(file_path.c_str());
    if(loadImage != NULL)
    {
        optimizeImage = SDL_DisplayFormat(loadImage);
        SDL_FreeSurface(loadImage);

    }

    // Xóa background color của ảnh
    if (file_path == "images/egg.png")
    {
        UINT32 color_key = SDL_MapRGB(optimizeImage->format, 0x22, 0x3E, 0x4A);
        SDL_SetColorKey(optimizeImage, SDL_SRCCOLORKEY, color_key);
    }
    else if (optimizeImage != NULL)
    {
        UINT32 color_key2 = SDL_MapRGB(optimizeImage->format, 0, 0xFF, 0xFF);
        SDL_SetColorKey(optimizeImage, SDL_SRCCOLORKEY, color_key2);
    }
    return optimizeImage;
}

void SDLCommonFunc::ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(src, NULL, des, &offset);
}

// Apply hoạt ảnh nổ
void SDLCommonFunc::ApplySurfaceClip(SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip, int x, int y)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(src, clip, des, &offset);
}

void SDLCommonFunc::CleanUp()
{
    SDL_FreeSurface(g_screen);
    SDL_FreeSurface(g_bkground);
}

// ---Kiểm tra va chạm---
bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
    int left_a = object1.x;
    int right_a = object1.x + object1.w;
    int top_a = object1.y;
    int bottom_a = object1.y + object1.h;

    int left_b = object2.x;
    int right_b = object2.x + object2.w;
    int top_b = object2.y;
    int bottom_b = object2.y + object2.h;

    // Case 1: size object 1 < size object 2
    if (left_a > left_b && left_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (left_a > left_b && left_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (top_a > top_b && top_a < bottom_b)
        {
            return true;
        }
    }

    if (right_a > left_b && right_a < right_b)
    {
        if (bottom_a > top_b && bottom_a < bottom_b)
        {
            return true;
        }
    }

    // Case 2: size object 1 < size object 2
    if (left_b > left_a && left_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (left_b > left_a && left_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (top_b > top_a && top_b < bottom_a)
        {
            return true;
        }
    }

    if (right_b > left_a && right_b < right_a)
    {
        if (bottom_b > top_a && bottom_b < bottom_a)
        {
            return true;
        }
    }

    // Case 3: size object 1 = size object 2
    if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
    {
        return true;
    }

    return false;
}
