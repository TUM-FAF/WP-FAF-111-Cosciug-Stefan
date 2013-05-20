#include <windows.h>
#include <math.h>
#include <stdio.h>

class Ball
{
    typedef struct _RECTD       // rc
    {
        double left;
        double top;
        double right;
        double bottom;
    } RECTD;
    typedef struct RGBv
    {
        int Red;	    // red component of color
        int Green;	    // green component of color
        int Blue;	    // blue component of color
    } RGBv;
    typedef struct Vectorial
    {
        double x;
        double y;
    } Vectorial;

    public:
        double Diameter;
        double SpeedM;      // Speed multiplier
        RECTD Coord;
        RGBv Color;
        Vectorial Impulse;  // {x - Length of a vector, which is the impulse along the x-axys; y - Length of a vector, which is the impulse along the y-axys}
        float Direction;    // Degrees

        Ball()
        {
            // Setting the default color to (white)
            Color.Red = 255;
            Color.Green = 255;
            Color.Blue = 255;
        }

        void SetDirection(float value)
        {
            Direction = value;
            printf("angle is: %f \n", value);

            // Computing impulse
            if (value>=0 && value<90.0)
            {
                Impulse.x = cosf(value*3.14/180.0)*Diameter/2.0;
                Impulse.y = -sinf(value*3.14/180.0)*Diameter/2.0;
            }
            else if (value>=90.0 && value<180.0)
            {
                Impulse.x = cosf(value*3.14/180.0)*Diameter/2.0;    //-
                Impulse.y = -sinf(value*3.14/180.0)*Diameter/2.0;
            }
            else if (value>=180.0 && value<270.0)
            {
                Impulse.x = cosf(value*3.14/180.0)*Diameter/2.0; //-
                Impulse.y = -sinf(value*3.14/180.0)*Diameter/2.0; //-
            }
            else if (value>=270.0 && value<360.0)
            {
                Impulse.x = cosf(value*3.14/180.0)*Diameter/2.0;
                Impulse.y = -sinf(value*3.14/180.0)*Diameter/2.0; //-
            }
            else if (value == 360.0)
            {
                Impulse.x = cosf(0)*Diameter/2.0;
                Impulse.y = -sinf(0)*Diameter/2.0;
            }

            // Computing speed
            SpeedM = sqrtf(Impulse.x*Impulse.x+Impulse.y*Impulse.y)/(Diameter/2.0);
            printf ("Speed for ball with diameter %f, is: %f \n", Diameter,SpeedM);
        }

        void UpdateSpeed(void)
        {
            SpeedM = sqrtf(Impulse.x*Impulse.x+Impulse.y*Impulse.y)/(Diameter/2.0);
            printf ("Speed for ball with diameter %f, is: %f \n", Diameter,SpeedM);
        }
};
