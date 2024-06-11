SingleJoystick::SingleJoystick(uint8_t , uint8_t y)
{
    xPin = x; yPin = y;
}
int SingleJoystick::getX()
{
    return analogRead(xPin);
}
int SingleJoystick::getY()
{
    return analogRead(yPin);
}

void SingleJoystick::init()
{
    nowX = getX(); nowY = getY();
    preX = nowX;   preY = nowY;
}

#define JOYSTCK_X A0
#define JOYSTCK_Y A1
SingleJoystick joystick(JOYSTCK_X, JOYSTCK_Y);

inline boolean SingleJoystick::isLow(const int value)
{
    return (value <= min); // min = ADC_MIN + AXIS_DEVIATION, ADC_MIN = 0
}                          // AXIS_DEVIATION = 100
inline boolean SingleJoystick::isHigh(const int value)
{
    return (value >= max); // max = ADC_MAX - AXIS_DEVIATION, ADC_MAX = 1023
}

boolean SingleJoystick::isUp() { return isHigh(getY()); }
boolean SingleJoystick::isDown() { return isLow(getY()); }
boolean SingleJoystick::isRight() { return isHigh(getX()); }
boolean SingleJoystick::isLeft() { return isLow(getX()); }

uint8_t SingleJoystick::isChange()
{
    uint8_t flag_change = 0;

    nowX = getX();
    if((nowX < (preX-50)) || nowX > (prexX+50))
    {
        preX = nowX;
        flag_change = 1;   
    }

    nowY = getY();
    if((nowY < (preY-50)) || nowY > (preY+50))
    {
        preY = nowY;
        flag_change = 1;
    }

    return flag_change;
}

uint8_t SingleJoystick::multipleRead()
{
    if(isUp()){
        return MOVE_UP;
    } 
    else if(isDown()){
        return MOVE_DOWN;
    }
    else if(isRight()){
        return MOVE_RIGHT;
    }
    else if(isLeft()){
        return MOVE_LEFT;
    }
    return MOVE_NOT;
}