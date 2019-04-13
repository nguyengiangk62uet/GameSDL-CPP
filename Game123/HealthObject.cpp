#include "HealthObject.h"

HealthObject::HealthObject()
{

}

HealthObject::~HealthObject()
{

}

void HealthObject::AddPosition(const int& xpos)
{
    pos_list_.push_back(xpos);
}

void HealthObject::Render(SDL_Surface* des)
{
    if (number_ == pos_list_.size())
    {
        for (int i = 0; i < pos_list_.size(); i++)
        {
            rect_.x = pos_list_.at(i);
            show(des);
        }
    }
}

void HealthObject::Init()
{
    loadImg("images/health.png");
    number_ = NUM_LIVES;
    if (pos_list_.size() > 0)
    {
        pos_list_.clear();
    }
    AddPosition(20);
    AddPosition(45);
    AddPosition(70);
}

void HealthObject::Decrease()
{
    number_--;
    pos_list_.pop_back();
}
