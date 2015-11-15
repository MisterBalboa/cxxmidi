#ifndef HARP_H
#define HARP_H

#include <string>

class Harp
{
    static int diatonicHarpMap[8][10];

public:
    Harp(int key_);
    std::string howToPlay(int note_) const;

private:
    int _key;



};

#endif // HARP_H
