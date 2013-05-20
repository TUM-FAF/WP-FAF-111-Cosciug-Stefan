#include <windows.h>
#include <math.h>
#include <stdio.h>

class Dock
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
        RECTD Coord;
        RGBv Color;
        Vectorial Impulse;
        float Direction;      // Degrees

        Dock(int PlayerIndex)	// {0 - User; 1 - Computer}
        {
            if (PlayerIndex == 0)
            {
                // Setting the Dock fill color to "Blue"
                Color.Red = 0;
                Color.Green = 0;
                Color.Blue = 255;

                // Defining the initial position for the user's dock
                Coord.left = 5.0;
                Coord.right = 35.0;
                Coord.top = 300.0;
                Coord.bottom = 460.0;

                // Defining the impulse direction
                Direction = 0;

                // Defining the initial impulse
                Impulse.x = 100.0;
                Impulse.y = 0;
            }
            else if (PlayerIndex == 1)
            {
                // Setting the Dock fill color to "Red"
                Color.Red = 255;
                Color.Green = 0;
                Color.Blue = 0;

                // Defining the initial position for the computer's dock
                Coord.left = 973.0;
                Coord.right = 1003.0;
                Coord.top = 300.0;
                Coord.bottom = 460.0;

                // Defining the impulse direction
                Direction = 180.0;

                // Defining the initial impulse
                Impulse.x = -100.0;
                Impulse.y = 0;
            }
        }
};

