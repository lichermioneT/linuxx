#include "circle.h"
#include "point.h"
    void circle::setR(int r)
    {
        m_R = r;
    }

    void circle::setCenter(Point center)
    {
        m_Center = center;
    }

    int  circle::getr()
    {
        return m_R;
    }

    Point circle::getCenter()
    {
        return m_Center;
    }
