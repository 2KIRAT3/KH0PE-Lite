#include "Helpers.h"
void Bhop(uintptr_t localplayer, uintptr_t forcejump, int m_flags)
{

   std::cout << "Bhop Key[SPACE]" << std::endl;

    while (true)
    {
        int AirOrGround = RPM<int>(localplayer + m_flags);
        if (GetAsyncKeyState(VK_SPACE))
        {

            if (AirOrGround == 257 || AirOrGround == 263)
            {
                WPM<int>(forcejump, 5);
            }
            else
            {
                WPM<int>(forcejump, 4);
            }
            Sleep(5);
        }


    }
}