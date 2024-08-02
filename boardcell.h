#ifndef _BOARDCELL_H
#define _BOARDCELL_H

using namespace std;

// First, the BoardCell abstract base class 

class BoardCell {
	
    public:  
		BoardCell() {} // default contstructor (do nothing)
        virtual ~BoardCell() {} // destructor (do nothing)
        
        virtual char display( ) = 0; // pure virtual function; this is an abstract base class

    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            if(isStatic()) {
                newR = myRow;
                newC = myCol;
            }      
        }
        
        virtual bool isHero( ) {return false;}
    	virtual bool isBaddie( ) {return false;}
    	virtual bool isSpace( ) {return false;}
        virtual bool isBarrier( ) {return false;}
        virtual bool isHole( ) {return false;}
    	virtual bool isExit( ) {return false;}
    	virtual bool isStatic( ) {return true;}

        virtual void setPower(int num) {}
        virtual int getPower() {return 1;}
        virtual void setNextMove(char inChar) {}
        
        void setMoved(bool m) {moved = m;}
        bool getMoved() {return moved;}
        void setRow(size_t r) {myRow = r;}
        size_t getRow() {return myRow;}
        void setCol(size_t c) {myCol = c;}
        size_t getCol() {return myCol;}
        void setPos(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }

    private:
        size_t myRow; // current row for this board cell in a 2D grid
        size_t myCol; // current column for this board cell in a 2D grid
        bool moved;   // true = this board cell already moved in the current round

}; // BoardCell (abstract base class)


// Then, all the derived classes

class Hero: public BoardCell {
	
    public:
    	Hero(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        
        virtual bool isHero( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return 'H';}
        
        virtual void setNextMove(char inChar ) {
            nextMove = inChar;
        }
    	
        // moves the hero
        virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {

            switch(nextMove) {
                // up and left
                case 'q':
                    newR = hRow - 1;
                    newC = hCol - 1;
                    break;
                // up
                case 'w':
                    newR = hRow - 1;
                    newC = hCol;
                    break;
                // up and right
                case 'e':
                    newR = hRow - 1;
                    newC = hCol + 1;
                    break;
                // left
                case 'a':
                    newR = hRow;
                    newC = hCol - 1;
                    break;
                // stay
                case 's':
                    newR = hRow;
                    newC = hCol;
                    break;
                // right
                case 'd':
                    newR = hRow;
                    newC = hCol + 1;
                    break;
                // down and left
                case 'z':
                    newR = hRow + 1;
                    newC = hCol - 1;
                    break;
                // down
                case 'x':
                    newR = hRow + 1;
                    newC = hCol;
                    break;
                // down and right
                case 'c':
                    newR = hRow + 1;
                    newC = hCol + 1;
                    break;
                default: 
                    break;
            }
        }

    private:
        char nextMove;

}; // Hero


class Monster: public BoardCell {
    
    public:
		Monster(size_t r, size_t c) {
            setRow(r);
            setCol(c);
            power = 1;
        }
        
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        
        virtual char display( ) {
            if (power == 2) {
                return 'M'; // Super Monster
            } else {
                return 'm'; // Monster
            } 
        }

        virtual void setPower(int num) {
            power = num;
        }

        virtual int getPower() {
            return power;
        }
        
        // moves both monsters
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            // works for both monsters
            size_t mRow = getRow();
            size_t mCol = getCol();
            // checks the row of the monster
            // the row is the same as the hero one
            if(hRow == mRow) {
                newR = hRow;
            }
            // the hero is lower than the monster
            else if(hRow > mRow){
                newR = mRow + getPower();
            }
            // the hero is above the monster
            else {
                newR = mRow - getPower();
            }

            // checks the col of the monster
            if(hCol == mCol) {
                newC = hCol;
            }
            // the hero is lower than the monster
            else if(hCol > mCol){
                newC = mCol + getPower();
            }
            // the hero is above the monster
            else {
                newC = mCol - getPower();
            }
        }
    
    private:
        int power; // power = 1 for Monster, power = 2 for SuperMonster

}; // Monster

class Bat: public BoardCell {
    public:
		Bat(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return '~';}
        
        // moves the bat
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            newR = getRow();
            newC = hCol;      
        }

}; // Bat


class Abyss: public BoardCell {
	public:
    	Abyss(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '#';}
        virtual bool isHole( ) {return true;}
}; // Abyss


class Wall: public BoardCell {
	public:
    	Wall(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '+';}
    	virtual bool isBarrier( ) {return true;}
}; // Wall


class Nothing: public BoardCell {
	public:
    	Nothing(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return ' ';}
    	virtual bool isSpace( ) {return true;}
}; // Nothing


class EscapeLadder: public BoardCell {
	public:
    	EscapeLadder(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '*';}
    	virtual bool isExit( ) {return true;}
}; // EscapeLadder


#endif //_BOARDCELL_H
