#include "rect.h"

UIRect ui_rect_from_rect2(Rect2 rect){
    UIRect output;
    
    output.x1 = rect.x; output.y1 = rect.y;
    output.x2 = rect.x + rect.width; output.y2 = rect.y;
    output.x3 = rect.x + rect.width; output.y3 = rect.y + rect.height;
    output.x4 = rect.x; output.y4 = rect.y + rect.height;

    return output;
}
