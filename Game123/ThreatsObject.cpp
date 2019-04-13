#include "ThreatsObject.h"

ThreatsObject::ThreatsObject()
{
    rect_.x = SCREEN_WIDTH * 0.5;
    rect_.y = SCREEN_HEIGHT;
    rect_.w = WIDTH_THREAT;
    rect_.h = HEIGHT_THREAT;

    x_val_ = 0;
    y_val_ = 0;

}

// Hàm hủy quái
ThreatsObject::~ThreatsObject()
{
    if (p_amo_list.size() > 0)
    {
        for (int i = 0; i < p_amo_list.size(); i++)
        {
            AmoObject* p_amo = p_amo_list.at(i);
            if (p_amo != NULL)
            {
                delete p_amo;
                p_amo = NULL;
            }
        }
        p_amo_list.clear();
    }
}

// Đạn của quái
void ThreatsObject::InitAmo(AmoObject* p_amo, const int& speed)
{
    if (p_amo)
    {
        bool ret = p_amo->loadImg("images/egg.png");
        if (ret)
        {
            p_amo->set_is_move(true);
            p_amo->SetWidthHeight(WIDTH_SPHERE, HEIGHT_SPHERE);
            p_amo->set_type(AmoObject::SPHERE);
            p_amo->SetRect(rect_.x + rect_.h/3, rect_.y + rect_.h);
            p_amo->Set_y_val(speed);
            p_amo_list.push_back(p_amo);
        }
    }
}

// Load đạn lên screen
void ThreatsObject::MakeAmo(SDL_Surface* des, const int& x_limit, const int& y_limit)
{
    for (int i = 0; i < p_amo_list.size(); i++)
    {
        AmoObject* p_amo = p_amo_list.at(i);
        if (p_amo)
        {
            if (p_amo->get_is_move())
            {
                p_amo->show(des);
                p_amo->HandleMoveOfThreat();
            }
            else
            {
                p_amo->set_is_move(true);
                p_amo->SetRect(rect_.x + rect_.h/3, rect_.y + rect_.h);
            }
        }
    }
}

void ThreatsObject::HandleInputAction(SDL_Event events)
{

}

// Di chuyển của quái
void ThreatsObject::HandleMove(const int& x_border, const int& y_border)
{
    rect_.y += y_val_;
    if (rect_.y > SCREEN_HEIGHT)
    {
        rect_.y = 0;
        int rand_x = rand()%1100;
        if (rand_x > SCREEN_WIDTH)
        {
            rand_x = SCREEN_WIDTH * 0.3;
        }
        rect_.x = rand_x;
    }
}

// Reset quái sau khi chết
void ThreatsObject::Reset(const int& yborder)
{
    rect_.y = yborder;
    int rand_x = rand()%1100;
    if (rand_x > SCREEN_WIDTH)
    {
        rand_x = SCREEN_WIDTH * 0.3;
    }
    rect_.x = rand_x;
    for (int i = 0; i < p_amo_list.size(); i++)
    {
        AmoObject* p_amo = p_amo_list.at(i);
        if (p_amo)
        {
            ResetAmo(p_amo);
        }
    }
}

void ThreatsObject::ResetAmo(AmoObject* p_amo)
{
    p_amo->SetRect(rect_.x + rect_.h, rect_.y + rect_.h);
}

void ThreatsObject::HandleMoveBoss(const int& x_border, const int& y_border)
{
    rect_.x += x_val_;
    if (rect_.x > SCREEN_WIDTH - 400)
        rect_.x = 300;
    else if (rect_.x < 300)
        rect_.x = 300;
}

