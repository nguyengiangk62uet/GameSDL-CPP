#include "Common_Function.h"
#include "MainObject.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include <SDL_getenv.h>

SDL_Surface *g_object;

bool Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        return false;
    }
    g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    if (g_screen == NULL)
    {
        return false;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        return false;
    }
    // Read file audio wav
    g_sound_fire[0] = Mix_LoadWAV("lazersound.wav");
    g_sound_fire[1] = Mix_LoadWAV("rightfire.wav");
    g_sound_explo[0] = Mix_LoadWAV("explosion.wav");
    g_sound_explo[1] = Mix_LoadWAV("chickdie.wav");
    if (g_sound_fire[0] == NULL || g_sound_fire[1] == NULL || g_sound_explo[0] == NULL || g_sound_explo[1] == NULL)
        return false;
    return true;
}


int main(int argc, char* argv[])
{
    bool is_run_screen = true;
    int bground_y = 0;
    // Khởi tạo nền
    bool is_quit = false;
    if (Init() == false)
        return 0;
    SDL_putenv("SDL_VIDEO_CENTERED=2");
    g_bkground = SDLCommonFunc::load_image("bground.png");
    if (g_bkground == NULL)
    {
        return 0;
    }
    SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, 0);

    // ---Khởi tạo các đối tượng trong game---
    // Tạo đối tượng tên lửa
    MainObject human_object;
    human_object.SetRect(SCREEN_WIDTH/2, 500);
    bool ret = human_object.loadImg("ok.png");
    if (!ret)
    {
        return 0;
    }

    // Tạo vụ nổ
    ExplosionObject exp_main;
    ret = exp_main.loadImg("exp_main.png");
    exp_main.SetClip();
    if (ret == false)
        return 0;

    //Tạo đối tượng Threats. Dùng vector để sinh nhiều threats
    ThreatsObject* p_threats = new ThreatsObject[NUM_THREAT];
    for (int i = 0; i < NUM_THREAT; i++)
    {
        ThreatsObject* p_threat = (p_threats + i);
        if (p_threat)
        {
            ret = p_threat->loadImg("normalChick.png");
            if (ret == false)
            {
                return 0;
            }
            int rand_x = rand()%600; // Sinh Threat ngẫu nhiên
            if (rand_x > SCREEN_WIDTH)
            {
                rand_x = SCREEN_WIDTH * 0.3;
            }
            p_threat->SetRect(rand_x, 0 - i*200);
            p_threat->set_y_val(1);

            AmoObject* p_amo = new AmoObject();
            p_threat->InitAmo(p_amo);  // Tạo đạn cho quái
        }
    }

    // ---Load liên tục screen (Tất cả các đối tượng phải chạy trong vòng lặp này)---
    while (!is_quit)
    {
        while (SDL_PollEvent(&g_even))
        {
            if (g_even.type == SDL_QUIT)
            {
                is_quit = true;
                break;
            }
            human_object.HandleInputAction(g_even, g_sound_fire);
        }

        // Apply Background
        if (is_run_screen == true)
        {
            bground_y -= 1;
            if (bground_y <= -(BACKGROUND_HEIGHT - SCREEN_HEIGHT))
            {
                is_run_screen = false;
            }
            else
            {
                SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, bground_y);
            }
        }
        else
        {
            SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, bground_y);
        }

        // Apply nhân vật vào hình
        human_object.HandleMove(); //Load hình nhân vật khi di chuyển
        human_object.show(g_screen);

        //Load đạn bay của nhân vật chính
        human_object.MakeAmo(g_screen);

        // Run threats
        for (int i = 0; i < NUM_THREAT; i++)
        {
            ThreatsObject* p_threat = (p_threats + i);
            if (p_threat)
            {
                p_threat->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat->show(g_screen);
                p_threat->MakeAmo(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);

                // Kiểm tra va chạm người với quái
                bool is_collis = SDLCommonFunc::CheckCollision(human_object.GetRect(), p_threat->GetRect());
                if (is_collis == true)
                {
                    Mix_PlayChannel(-1, g_sound_explo[0], 0);
                    for (int ex = 0; ex < 4; ex++)
                    {
                        int x_pos = (human_object.GetRect().x + human_object.GetRect().w*0.5) - EXP_WIDTH*0.5;
                        int y_pos = (human_object.GetRect().y + human_object.GetRect().h*0.5) - EXP_HEIGHT*0.5;

                        exp_main.SetFrame(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.ShowExplosion(g_screen);
                        SDL_Delay(100);
                        // Update screen
                        if ( SDL_Flip(g_screen) == -1)
                            return 0;
                    }
                    SDL_Delay(2000);
                    if (MessageBox(NULL, "Game Over !!! You are lose the game.", "End Game", MB_OK) == IDOK)
                    {
                        delete[] p_threats; // Giải phóng bộ nhớ
                        SDLCommonFunc::CleanUp();
                        SDL_Quit();
                        return 1;
                    }
                }

                // Va chạm giữa người và đạn của quái
                std::vector<AmoObject*> amo_list_threat = p_threat->GetAmoList();
                for (int tm = 0; tm < amo_list_threat.size(); tm++)
                {
                    AmoObject* p_amo = amo_list_threat.at(tm);
                    if (p_amo)
                    {
                        bool ret_collis = SDLCommonFunc::CheckCollision(p_amo->GetRect(), human_object.GetRect());
                        if (ret_collis == true)
                        {
                            Mix_PlayChannel(-1, g_sound_explo[0], 0);
                            for (int ex = 0; ex < 4; ex++)
                            {
                                int x_pos = (human_object.GetRect().x + human_object.GetRect().w*0.5) - EXP_WIDTH*0.5;
                                int y_pos = (human_object.GetRect().y + human_object.GetRect().h*0.5) - EXP_HEIGHT*0.5;

                                exp_main.SetFrame(ex);
                                exp_main.SetRect(x_pos, y_pos);
                                exp_main.ShowExplosion(g_screen);
                                SDL_Delay(100);
                                // Update screen
                                if ( SDL_Flip(g_screen) == -1)
                                    return 0;
                            }
                            SDL_Delay(2000);
                            if (MessageBox(NULL, "Game Over !!! You are lose the game.", "End Game", MB_OK) == IDOK)
                            {
                                delete[] p_threats; // Giải phóng bộ nhớ
                                SDLCommonFunc::CleanUp();
                                SDL_Quit();
                                return 1;
                            }
                        }
                    }
                }

                // Bắn trúng quái
                std::vector<AmoObject*> amo_list = human_object.GetAmoList();
                for (int am = 0; am < amo_list.size(); am++)
                {
                    AmoObject* p_amo = amo_list.at(am);
                    if (p_amo != NULL)
                    {
                        bool ret_collis = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p_threat->GetRect());
                        if (ret_collis)
                        {
                            Mix_PlayChannel(2, g_sound_explo[1], 0);
                            for (int ex = 0; ex < 2; ex++)
                            {
                                int x_pos = (p_threat->GetRect().x + p_threat->GetRect().w*0.5) - EXP_WIDTH*0.5;
                                int y_pos = (p_threat->GetRect().y + p_threat->GetRect().h*0.5) - EXP_HEIGHT*0.5;

                                exp_main.SetFrame(ex);
                                exp_main.SetRect(x_pos, y_pos);
                                exp_main.ShowExplosion(g_screen);
                                // Update screen
                                if ( SDL_Flip(g_screen) == -1)
                                    return 0;
                            }
                            p_threat->Reset(0 - i*200);
                            human_object.RemoveAmo(am);
                        }
                    }
                }
            }
        }

        // Update screen
        if ( SDL_Flip(g_screen) == -1)
            return 0;
    }

    delete[] p_threats; // Giải phóng bộ nhớ
    SDLCommonFunc::CleanUp();
    SDL_Quit();
    return 1;
}
