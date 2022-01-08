#pragma once

#include <stdint.h>

namespace Devices
{
    struct InternationalLayout
    {
        static constexpr const uint8_t s_layoutSize = 186;
        static constexpr const uint8_t s_translationSize = 22;

        int GetKeyCode(int scancode);
        int GetTranslation(int keycode);

        static int s_layout[s_layoutSize];
        static int s_translation[s_translationSize][2];
    };
}