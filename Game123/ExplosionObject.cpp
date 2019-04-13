#include "ExplosionObject.h"

ExplosionObject::ExplosionObject()
{
    frame_ = 0;
}

ExplosionObject::~ExplosionObject()
{

}

// Set vị trí clip nổ
void ExplosionObject::SetClip()
{
    clip_right[0].x = 0;
    clip_right[0].y = 0;
    clip_right[0].w = EXP_WIDTH;
    clip_right[0].h = EXP_HEIGHT;

    clip_right[1].x = EXP_WIDTH;
    clip_right[1].y = 0;
    clip_right[1].w = EXP_WIDTH;
    clip_right[1].h = EXP_HEIGHT;

    clip_right[2].x = 2 * EXP_WIDTH;
    clip_right[2].y = 0;
    clip_right[2].w = EXP_WIDTH;
    clip_right[2].h = EXP_HEIGHT;

    clip_right[3].x = 3 * EXP_WIDTH;
    clip_right[3].y = 0;
    clip_right[3].w = EXP_WIDTH;
    clip_right[3].h = EXP_HEIGHT;
}

void ExplosionObject::ShowExplosion(SDL_Surface* des)
{
    if (frame_ >= 4)
    {
        frame_ = 0;
    }
    SDLCommonFunc::ApplySurfaceClip(this->p_object_, des, &clip_right[frame_], rect_.x, rect_.y);
}
