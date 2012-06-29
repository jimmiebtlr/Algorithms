/* 
 * File:   Move.cpp
 * Author: jimmie
 * 
 * Created on April 28, 2012, 5:04 PM
 */

#include "Move.h"

Move::Move(unsigned short elementMoved, unsigned short movedTo, unsigned short movedFrom)
{
    element_moved = elementMoved;
    moved_to = movedTo;
    moved_from = movedFrom;
}

