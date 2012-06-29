/* 
 * File:   Move.h
 * Author: jimmie
 *
 * Created on April 28, 2012, 5:04 PM
 */

#ifndef MOVE_H
#define	MOVE_H

class Move {
public:
      unsigned short element_moved; // Points index
      unsigned short moved_to; //containers index
      unsigned short moved_from; // container index the point moved from
      Move(unsigned short elementMoved, unsigned short movedTo, unsigned short movedFrom);

private:

};

#endif	/* MOVE_H */

