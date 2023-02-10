/**
 * @file consts.hh
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Display Constants (color values, text addrs, ...)
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

namespace color{
    static constexpr unsigned long Red = 0xff0000UL;
    static constexpr unsigned long Orange = 0xffa500UL;
    static constexpr unsigned long Green = 0x00ff00UL;
    static constexpr unsigned long Blue = 0x0000ffUL;
    static constexpr unsigned long LightBlue = 0x5dade2L;
    static constexpr unsigned long Yellow = 0xffff00UL;
    static constexpr unsigned long Magenta = 0xff00ffUL;
    static constexpr unsigned long Purple = 0x800080UL;
    static constexpr unsigned long White = 0xffffffUL;
    static constexpr unsigned long Black = 0x000000UL;
} // namespace color

namespace fonts
{
    static constexpr int custom = 1;
    static constexpr int error = 18;
    static constexpr int regularLarge = 24;
    // MUST BE LOADED FROM ROM
    static constexpr int large = 34;

} // namespace fonts

namespace coordinates
{
    
    namespace rpm
    {
        static constexpr int x = 245;
        static constexpr int y = 44;
    } // namespace rpm

    namespace gear
    {
        static constexpr int x = 165;
        static constexpr int y = 88;
    } // namespace gear

    namespace errorText
    {
        static constexpr int x = 245;
        static constexpr int y = 260;
    } // namespace errorText

    namespace speed
    {
        static constexpr int x = 0;
        static constexpr int y = 100;
    } // namespace speed

    namespace speedText
    {
        static constexpr int x = 5;
        static constexpr int y = 190;
    } // namespace speedText

    namespace coolantBar
    {
        static constexpr int x = 475;
        static constexpr int y = 265;
        static constexpr int width = 35;
        static constexpr int height = 18;
        static constexpr int vertical_gap = 4;
    } // namespace coolantBar

    namespace coolant
    {
        static constexpr int x = 430;
        static constexpr int y = 100;
    } // namespace coolant

    namespace coolantText
    {
        static constexpr int x = 362;
        static constexpr int y = 190;
    } // namespace coolantText
    
} // namespace coordinates

namespace engineData
{
    static constexpr int maxTemperature = 120;
} // namespace engineData
