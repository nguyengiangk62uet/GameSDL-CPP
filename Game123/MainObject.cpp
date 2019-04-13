#include "MainObject.h"

MainObject::MainObject()
{
    // Tọa độ đối tượng chính
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = WIDTH_MAIN_OBJECT;
    rect_.h = HEIGHT_MAIN_OBJECT;
    x_val_ = 0;
    y_val_ = 0;
}

MainObject::~MainObject()
{

}

// Xử lý sự kiện nhân vật chính
void MainObject::HandleInputAction(SDL_Event events, Mix_Chunk* fire_sound[2])
{
    if (events.type == SDL_KEYDOWN) // Sự kiện khi ấn phím
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_UP:
            y_val_ -= HEIGHT_MAIN_OBJECT/SPEED_MAIN;
            break;
        case SDLK_DOWN:
            y_val_ += HEIGHT_MAIN_OBJECT/SPEED_MAIN;
            break;
        case SDLK_LEFT:
            x_val_ -= WIDTH_MAIN_OBJECT/SPEED_MAIN;
            break;
        case SDLK_RIGHT:
            x_val_ += WIDTH_MAIN_OBJECT/SPEED_MAIN;
            break;
        default:
            break;
        }
    }
    else if (events.type == SDL_KEYUP) // Sự kiện khi nhả phím
    {
        switch(events.key.keysym.sym)
        {
        case SDLK_UP:
            y_val_ += HEIGHT_MAIN_OBJECT/SPEED_MAIN;
            break;
        case SDLK_DOWN:
            y_val_ -= HEIGHT_MAIN_OBJECT/SPEED_MAIN;
            break;
        case SDLK_LEFT:
            x_val_ += WIDTH_MAIN_OBJECT/SPEED_MAIN;
            break;
        case SDLK_RIGHT:
            x_val_ -= WIDTH_MAIN_OBJECT/SPEED_MAIN;
            break;
        case SDLK_KP0:
        {
            AmoObject* p_amo = new AmoObject();
            p_amo->SetWidthHeight(WIDTH_LAZER, HEIGHT_LAZER);
            p_amo->loadImg("images/laser.png");
            p_amo->set_type(AmoObject::LAZER);
            Mix_PlayChannel(-1, fire_sound[0], 0);

            p_amo->SetRect(this->rect_.x + 20, this->rect_.y + 22);
            p_amo->set_is_move(true);
            p_amo->Set_y_val(20);
            p_amo_list.push_back(p_amo);
        }
        default:
            break;
        }
    }
    else if (events.type == SDL_MOUSEBUTTONDOWN) // Sự kiện khi ấn chuột. Bắn đạn
    {
        AmoObject* p_amo = new AmoObject();
        if (events.button.button == SDL_BUTTON_LEFT)
        {
            p_amo->SetWidthHeight(WIDTH_LAZER, HEIGHT_LAZER);
            p_amo->loadImg("images/laser.png");
            p_amo->set_type(AmoObject::LAZER);
            Mix_PlayChannel(-1, fire_sound[0], 0);
        }
        else if (events.button.button == SDL_BUTTON_RIGHT)
        {
            p_amo->SetWidthHeight(WIDTH_SPHERE, HEIGHT_SPHERE);
            p_amo->loadImg("images/sphere.png");
            p_amo->set_type(AmoObject::SPHERE);
            Mix_PlayChannel(-1, fire_sound[1], 0);
        }
        p_amo->SetRect(this->rect_.x + 20, this->rect_.y + 22);
        p_amo->set_is_move(true);
        p_amo->Set_y_val(20);
        p_amo_list.push_back(p_amo);
    }
    else if (events.type == SDL_MOUSEBUTTONUP)
    {

    }
}

// Load bắn đạn của nhân vật chính
void MainObject::MakeAmo(SDL_Surface* des)
{
    for (int i = 0; i < p_amo_list.size(); i++)
    {
        AmoObject* p_amo = p_amo_list.at(i);
        if (p_amo != NULL)
        {
            if (p_amo->get_is_move())
            {
                p_amo->show(des);
                p_amo->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
            }
            else
            {
                if (p_amo != NULL)
                {
                    p_amo_list.erase(p_amo_list.begin() + i);
                    delete p_amo;
                    p_amo = NULL;
                }
            }
        }
    }
}

// Di chuyển nhân vật
void MainObject::HandleMove()
{
    rect_.x += x_val_;
    if (rect_.x < 0 || rect_.x + WIDTH_MAIN_OBJECT > SCREEN_WIDTH + 30)
    {
        rect_.x -= x_val_;
    }
    rect_.y += y_val_;
    if (rect_.y < 0 || rect_.y + HEIGHT_MAIN_OBJECT > SCREEN_HEIGHT)
    {
        rect_.y -= y_val_;
    }
}

// Xoá đạn
void MainObject::RemoveAmo(const int& idx)
{
    for (int i = 0; i < p_amo_list.size(); i++)
    {
        if (p_amo_list.size() > 0 && idx < p_amo_list.size())
        {
            AmoObject* p_amo = p_amo_list.at(idx);
            p_amo_list.erase(p_amo_list.begin() + idx);
            if(p_amo != NULL)
            {
                delete p_amo;
                p_amo = NULL;
            }
        }
    }
}
