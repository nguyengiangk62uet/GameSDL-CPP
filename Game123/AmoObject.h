// Đối tượng đạn bắn

#ifndef AMO_OBJECT_H_
#define AMO_OBJECT_H_

#include "BaseObject.h"

#define WIDTH_LAZER 35
#define HEIGHT_LAZER 5

#define WIDTH_SPHERE 10
#define HEIGHT_SPHERE 10

class AmoObject : public BaseObject
{
public:
    enum AmoType
    {
        NONE = 0,
        LAZER = 1,
        SPHERE = 2
    };
    AmoObject();
    ~AmoObject();
    void HandleInputAction(SDL_Event events);
    void HandleMove(const int& x_border, const int& y_border);
    void HandleMoveOfThreat();

    int get_type() const
    {
        return amo_type_;
    }
    void set_type(const int& type)
    {
        amo_type_ = type;
    }
    bool get_is_move() const
    {
        return is_move_;
    }
    void set_is_move(bool is_move)
    {
        is_move_ = is_move;
    }
    void SetWidthHeight(const int& width, const int& height)
    {
        rect_.w = width;
        rect_.h = height;
    }

    void Set_x_val(const int& val)
    {
        x_val_ = val;
    }
    void Set_y_val(const int& val)
    {
        y_val_ = val;
    }

    int Get_x_val() const
    {
        return x_val_;
    }
    int Get_y_val() const
    {
        return y_val_;
    }

private:
    int x_val_;
    int y_val_;
    bool is_move_;
    int amo_type_;
};
#endif // AMO_OBJECT_H_
