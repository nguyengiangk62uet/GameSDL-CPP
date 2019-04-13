// Đối tượng xuất hiện khi va chạm

#ifndef EXPLOSION_OBJECT_H_
#define EXPLOSION_OBJECT_H_

#include "BaseObject.h"
#include "Common_Function.h"

const int EXP_WIDTH = 165;
const int EXP_HEIGHT = 165;

class ExplosionObject : public BaseObject
{
public:
    ExplosionObject();
    ~ExplosionObject();

    void SetClip();
    void SetFrame(const int& fr)
    {
        frame_ = fr;
    }
    void ShowExplosion(SDL_Surface* des);

private:
    int frame_;
    SDL_Rect clip_right[4];
};


#endif // EXPLOSION_OBJECT_H_
