#include <H4P_FlasherController.h>
#include "status.hh"

extern H4P_FlasherController h4fc;
extern const int APP_STATUS_LED_PIN;

void updateStatus(Status status) 
{
    String pattern = String("0");
    switch (status)
    {
    case Status::boot:
        pattern = String("10");
        break;
    case Status::disconnected:
        pattern = String("1100");
        break;
    case Status::running:
        pattern = String("111100001100");
        break;
    default:
        pattern = String("10");
        break;
    }
    h4fc.flashPattern(pattern.c_str(), 250, APP_STATUS_LED_PIN, ACTIVE_LOW); 
}