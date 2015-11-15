#include "harp.h"

int Harp::diatonicHarpMap[8][10] =
{
    // 1   2   3   4   5   6   7   8   9  10
    {  3,  8, 12, 15, 18, 22, 25, 29, 32, 37 }, // overblow
    { -1, -1, -1, -1, -1, -1, -1, 27, 30, 34 }, // 1 step blow bend
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, 35 }, // 0.5 step blow bend
    {  0,  4,  7, 12, 16, 19, 24, 28, 31, 36 }, // blow
    {  2,  7, 11, 14, 17, 21, 23, 26, 29, 33 }, // draw
    {  1,  6, 10, 13, -1, 20, -1, -1, -1, -1 }, // 0.5 step draw bend
    { -1,  5,  9, -1, -1, -1, -1, -1, -1, -1 }, // 1 step draw bend
    { -1, -1,  8, -1, -1, -1, -1, -1, -1, -1 }  // 1.5 step draw bend
};

Harp::Harp(int key_)
    : _key(key_)
{

}

std::string Harp::howToPlay(int note_) const
{
    int relativeNote = note_ - _key;

    if( (relativeNote < 0) || (relativeNote > 37))
        return "---";

    switch(relativeNote)
    {
    case 0 : return "1 blow";
    case 1 : return "1 draw bend 0.5 step";
    case 2 : return "1 draw";
    case 3 : return "1 overblow";
    case 4 : return "2 blow";
    case 5 : return "2 draw bend 1 step";
    case 6 : return "2 draw bend 0.5 step";
    case 7 : return "2 draw / 3 blow";
    case 8 : return "3 draw bend 1.5 step / 2 overblow";
    case 9 : return "3 draw bend 1 step";
    case 10 : return "3 draw bend 0.5 step";
    case 11 : return "3 draw";
    case 12 : return "4 blow / 3 overblow";
    case 13 : return "4 draw bend 0.5 step";
    case 14 : return "4 draw";
    case 15 : return "4 overblow";
    case 16 : return "5 blow";
    case 17 : return "5 draw";
    case 18 : return "5 overblow";
    case 19 : return "6 blow";
    case 20 : return "6 draw bend 0.5 step";
    case 21 : return "6 draw";
    case 22 : return "6 overblow";
    case 23 : return "7 draw";
    case 24 : return "7 blow";
    case 25 : return "7 overblow";
    case 26 : return "8 draw";
    case 27 : return "8 blow bend 1 step";
    case 28 : return "8 blow";
    case 29 : return "9 draw / 8 overblow";
    case 30 : return "9 blow bend 1 step";
    case 31 : return "9 blow";
    case 32 : return "9 overblow";
    case 33 : return "10 draw";
    case 34 : return "10 blow bend 1 step";
    case 35 : return "10 blow bend 0.5 step";
    case 36 : return "10 blow";
    case 37 : return "10 overblow";
    default: break;
    }

    return "???";
}
