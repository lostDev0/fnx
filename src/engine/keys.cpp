namespace fnx
{
    char key_to_ascii(FNX_KEY key)
    {
        if (key >= FNX_KEY::FK_FIRST && key <= FNX_KEY::FK_GRAVE_ACCENT)
        {
            // TODO : handle things other than alphas
            auto c = static_cast<char>(key);
            auto is_shift_pressed = false;
            {
                auto [win,_] = singleton<fnx::window>::acquire();
                is_shift_pressed = win.is_key_pressed(FNX_KEY::FK_LEFT_SHIFT) || win.is_key_pressed(FNX_KEY::FK_RIGHT_SHIFT);
            }
            if (!is_shift_pressed)
            {
                if (key >= FNX_KEY::FK_A && key <= FNX_KEY::FK_GRAVE_ACCENT)
                {
                    // adjust to lower case alpha
                    c += 32;
                }
            }
            else
            {
                if (key == FNX_KEY::FK_0) { c = 41; } // 0 = 48 ) = 41
                else if (key == FNX_KEY::FK_1) { c = 33; } // 1 = 49 ! = 33
                else if (key == FNX_KEY::FK_2) { c = 64; } // 2 = 50 @ = 64
                else if (key == FNX_KEY::FK_3) { c = 35; } // 3 = 51 # = 35
                else if (key == FNX_KEY::FK_4) { c = 36; } // 4 = 52 $ = 36
                else if (key == FNX_KEY::FK_5) { c = 37; } // 5 = 53 % = 37
                else if (key == FNX_KEY::FK_6) { c = 94; } // 6 = 54 ^ = 94
                else if (key == FNX_KEY::FK_7) { c = 38; } // 7 = 55 & = 38
                else if (key == FNX_KEY::FK_8) { c = 42; } // 8 = 56 * = 42
                else if (key == FNX_KEY::FK_9) { c = 40; } // 9 = 57 ( = 40
                else if (key == FNX_KEY::FK_APOSTROPHE) { c = 34; }
                else if (key == FNX_KEY::FK_COMMA) { c = 60; }
                else if (key == FNX_KEY::FK_MINUS) { c = 95; }
                else if (key == FNX_KEY::FK_PERIOD) { c = 62; }
                else if (key == FNX_KEY::FK_SLASH) { c = 63; }
                else if (key == FNX_KEY::FK_SEMICOLON) { c = 58; }
                else if (key == FNX_KEY::FK_EQUAL) { c = 43; }
            }
            
            return c;
        }
        return 0x00;
    }
}
