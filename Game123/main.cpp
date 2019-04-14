#include "Common_Function.h"
#include "MainObject.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "HealthObject.h"
#include "TextObject.h"
#include <SDL_getenv.h>
#include <iostream>
#include "SDL_syswm.h"

SDL_Surface *g_object;
TTF_Font* g_font_text; //Font chữ
TTF_Font* end_game_text;

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
    g_sound_fire[0] = Mix_LoadWAV("sound/lazersound.wav");
    g_sound_fire[1] = Mix_LoadWAV("sound/rightfire.wav");
    g_sound_explo[0] = Mix_LoadWAV("sound/explosion.wav");
    g_sound_explo[1] = Mix_LoadWAV("sound/chickdie.wav");
    if (g_sound_fire[0] == NULL || g_sound_fire[1] == NULL || g_sound_explo[0] == NULL || g_sound_explo[1] == NULL)
        return false;

    if (TTF_Init() == -1)
    {
        return false;
    }
    g_font_text = TTF_OpenFont("starcraft.ttf", 20);
    end_game_text = TTF_OpenFont("starcraft.ttf", 40);

    if (!g_font_text)
    {
        return false;
    }
    return true;
}

int game();

int main(int argc, char* argv[])
{
    game();
    return 0;
}

int game()
{
    bool is_run_screen = true;
    int bground_y = 0;
    // Khởi tạo nền
    bool is_quit = false;
    if (Init() == false)
        return 0;
    SDL_WM_SetCaption("NguyenGiang-UET SDL Game", "images/eggchick.png");
    SDL_WM_SetIcon(SDL_LoadBMP("images/eggchick.bmp"), NULL);
    g_bkground = SDLCommonFunc::load_image("images/bground.png");
    if (g_bkground == NULL)
    {
        return 0;
    }
    SDLCommonFunc::ApplySurface(g_bkground, g_screen, 0, 0);
    SDL_putenv("SDL_VIDEO_CENTERED=1");

    // ---Khởi tạo các đối tượng trong game---

    // Make health player
    HealthObject health_Player;
    health_Player.Init();

    // Make game mark
    TextObject mark_game;
    TextObject win_game;
    mark_game.SetColor(TextObject::WHITE_TEXT);
    win_game.SetColor(TextObject::RED_TEXT);


    // Tạo đối tượng tên lửa
    MainObject human_object;
    human_object.SetRect(START_XPOS_MAIN, START_YPOS_MAIN);
    bool ret = human_object.loadImg("images/ok.png");
    if (!ret)
    {
        return 0;
    }

    // Tạo vụ nổ
    ExplosionObject exp_main;
    ret = exp_main.loadImg("images/exp_main.png");
    exp_main.SetClip();
    if (ret == false)
        return 0;

    //Tạo đối tượng Threats. Dùng vector để sinh nhiều threats
    ThreatsObject* p_threats = new ThreatsObject[NUM_THREAT];
    for (int i = 0; i < NUM_THREAT; i++) // Số lượng quái là cụ thể. Khi chạy sẽ lặp và sinh liên tiếp
    {
        ThreatsObject* p_threat = (p_threats + i);
        if (p_threat)
        {
            ret = p_threat->loadImg("images/normalChick.png");
            if (ret == false)
            {
                return 0;
            }
            int rand_x = rand()%1100; // Sinh Threat ngẫu nhiên
            if (rand_x > SCREEN_WIDTH)
            {
                rand_x = SCREEN_WIDTH * 0.3;
            }
            p_threat->SetRect(rand_x, 0 - i*400);
            p_threat->set_y_val(1);

            AmoObject* p_amo = new AmoObject();
            p_threat->InitAmo(p_amo, 3);  // Tạo đạn cho quái
        }
    }

    // Tạo boss chủ
    ThreatsObject* p_boss = new ThreatsObject();

    ret = p_boss->loadImg("images/uco.png");
    if (ret == false)
    {
        return 0;
    }
    p_boss->SetRect(SCREEN_WIDTH / 3, 10);
    p_boss->set_x_val(2);

    AmoObject* p_amo = new AmoObject();
    p_boss->InitAmo(p_amo, 6);  // Tạo đạn cho quái



    unsigned int die_number = 0; // Lượt chết của nhân vật chính
    unsigned int mark_value = 0; // Điểm khi bắn chết quái
    unsigned int num_fire_boss = 0;

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
        if (is_run_screen == true) // Background là tấm ảnh dài bằng 5 lần screen
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

            p_boss->HandleMoveBoss(SCREEN_WIDTH, SCREEN_HEIGHT);
            p_boss->show(g_screen);
            p_boss->MakeAmo(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
            std::vector<AmoObject*> amo_list = human_object.GetAmoList();
            for (unsigned int am = 0; am < amo_list.size(); am++)
            {
                AmoObject* p_amo = amo_list.at(am);
                if (p_amo != NULL)
                {
                    bool ret_collis = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p_boss->GetRect());
                    if (ret_collis)
                    {
                        num_fire_boss++;
                        Mix_PlayChannel(2, g_sound_explo[1], 0);
                        for (int ex = 0; ex < 2; ex++)
                        {
                            int x_pos = (p_boss->GetRect().x + p_boss->GetRect().w*0.5) - EXP_WIDTH*0.5;
                            int y_pos = (p_boss->GetRect().y + p_boss->GetRect().h*0.5) - EXP_HEIGHT*0.5;

                            exp_main.SetFrame(ex);
                            exp_main.SetRect(x_pos, y_pos);
                            exp_main.ShowExplosion(g_screen);
                            // Update screen
                            if ( SDL_Flip(g_screen) == -1)
                                return 0;
                        }
                        human_object.RemoveAmo(am);
                        if (num_fire_boss > 15)
                        {
                            std::string text("Congratulation !!! You win.");
                            win_game.SetText(text);
                            win_game.SetRect(200, 300);
                            win_game.CreateGameText(end_game_text, g_screen);
                            Mix_PlayChannel(-1, g_sound_explo[0], 0);
                            for (int ex = 0; ex < 4; ex++)
                            {
                                int x_pos = (p_boss->GetRect().x + p_boss->GetRect().w*0.5) - EXP_WIDTH*0.5;
                                int y_pos = (p_boss->GetRect().y + p_boss->GetRect().h*0.5) - EXP_HEIGHT*0.5;

                                exp_main.SetFrame(ex);
                                exp_main.SetRect(x_pos, y_pos);
                                exp_main.ShowExplosion(g_screen);
                                SDL_Delay(400);
                                // Update screen
                                if ( SDL_Flip(g_screen) == -1)
                                    return 0;
                            }
                            Sleep(2000);
                            if (MessageBox(NULL, "Do you want to continue?", "You win", MB_YESNO) == IDYES)
                            {
                                game();
                            }
                            else
                            {
                                delete[] p_threats; // Giải phóng bộ nhớ
                                SDLCommonFunc::CleanUp();
                                SDL_Quit();
                                return 0;
                            }
                        }
                    }
                }
            }
        }

        // Show health player
        health_Player.Render(g_screen);

        // Apply nhân vật vào hình
        human_object.HandleMove(); //Load hình nhân vật khi di chuyển
        human_object.show(g_screen);

        //Load đạn bay của nhân vật chính
        human_object.MakeAmo(g_screen);

        // Run threats
        for (int i = 0; i < NUM_THREAT; i++) //
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
                    die_number++;
                    Mix_PlayChannel(-1, g_sound_explo[0], 0);
                    for (int ex = 0; ex < 3; ex++)
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
                    SDL_Delay(1000);
                    if (die_number <= NUM_LIVES - 1) // Chơi tiếp nếu còn mạng
                    {
                        human_object.SetRect(START_XPOS_MAIN, START_YPOS_MAIN);
                        health_Player.Decrease();
                        health_Player.Render(g_screen);
                        if ( SDL_Flip(g_screen) == -1)
                        {
                            delete[] p_threats; // Giải phóng bộ nhớ
                            SDLCommonFunc::CleanUp();
                            SDL_Quit();
                            return 0;
                        }
                    }
                    else
                    {
                        Sleep(1000);
                        if (MessageBox(NULL, "You die! Do you want to continue?", "You lose", MB_YESNO) == IDYES)
                        {
                            delete[] p_threats; // Giải phóng bộ nhớ
                            SDLCommonFunc::CleanUp();
                            SDL_Quit();
                            game();
                        }
                        else
                        {
                            delete[] p_threats; // Giải phóng bộ nhớ
                            SDLCommonFunc::CleanUp();
                            SDL_Quit();
                            return 0;
                        }
                    }
                }

                // Va chạm giữa người và đạn của quái
                std::vector<AmoObject*> amo_list_threat = p_threat->GetAmoList();
                for (unsigned int tm = 0; tm < amo_list_threat.size(); tm++)
                {
                    AmoObject* p_amo = amo_list_threat.at(tm);
                    if (p_amo)
                    {
                        bool ret_collis = SDLCommonFunc::CheckCollision(p_amo->GetRect(), human_object.GetRect());
                        if (ret_collis == true)
                        {
                            die_number++;
                            Mix_PlayChannel(-1, g_sound_explo[0], 0);
                            for (int ex = 0; ex < 3; ex++)
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
                            SDL_Delay(1000);
                            if (die_number <= NUM_LIVES - 1)
                            {
                                human_object.SetRect(START_XPOS_MAIN, START_YPOS_MAIN);
                                health_Player.Decrease();
                                health_Player.Render(g_screen);
                                if ( SDL_Flip(g_screen) == -1)
                                {
                                    delete[] p_threats; // Giải phóng bộ nhớ
                                    SDLCommonFunc::CleanUp();
                                    SDL_Quit();
                                    return 0;
                                }
                            }
                            else
                            {
                                Sleep(1000);
                                if (MessageBox(NULL, "You die! Do you want to continue?", "You lose", MB_YESNO) == IDYES)
                                {
                                    delete[] p_threats; // Giải phóng bộ nhớ
                                    SDLCommonFunc::CleanUp();
                                    SDL_Quit();
                                    game();
                                }
                                else
                                {
                                    delete[] p_threats; // Giải phóng bộ nhớ
                                    SDLCommonFunc::CleanUp();
                                    SDL_Quit();
                                    return 0;
                                }
                            }
                        }
                    }
                }

                // Bắn trúng quái
                std::vector<AmoObject*> amo_list = human_object.GetAmoList();
                for (unsigned int am = 0; am < amo_list.size(); am++)
                {
                    AmoObject* p_amo = amo_list.at(am);
                    if (p_amo != NULL)
                    {
                        bool ret_collis = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p_threat->GetRect());
                        if (ret_collis)
                        {
                            mark_value += 100;
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
                            p_threat->Reset(0 - i*400);
                            human_object.RemoveAmo(am);
                        }
                    }
                }
            }
        }

        // Show mark in screen
        std::string val_str_mark = std::to_string(mark_value);
        std::string strMark("Score: ");
        strMark += val_str_mark;
        mark_game.SetText(strMark);
        mark_game.CreateGameText(g_font_text, g_screen);

        // Update screen
        if ( SDL_Flip(g_screen) == -1)
        {
            delete[] p_threats; // Giải phóng bộ nhớ
            SDLCommonFunc::CleanUp();
            SDL_Quit();
            return 0;
        }
    }

    delete[] p_threats; // Giải phóng bộ nhớ
    SDLCommonFunc::CleanUp();
    SDL_Quit();
    return 0;
}
