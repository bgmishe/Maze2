#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        size_t HeroRow; // Hero's position row
	    size_t HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }
    
        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }
        
        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }

        // gets te Hero's position
        void getHeroPosition(size_t& row, size_t& col) {
            row = HeroRow;
            col = HeroCol;
        }

        // sets the Hero's position with the passed in val
        void setHeroPosition(size_t row, size_t col) {
            this->HeroRow = row;
            this->HeroCol = col;        
        }

        // finds the address of the hero and updates the private data members HeroRow, HeroCol
        void findHero() {
            size_t row;
            size_t col;
            getHeroPosition(row, col);

            // loops thru the rows
            for(size_t r = 0; r < numRows; r++) {
                // loops thru the cols
                for(size_t c = 0; c < numCols; c++){
                    // had found the hero position
                    if(board(r, c) != nullptr && board(r, c)->display() == 'H') {
                        setHeroPosition(r, c);
                        return;
                    }
                }
            }
            setHeroPosition(-1, -1);        
        }

        // gets the old cell frees it and assigns it with nothing 
        // goes to the new destination and puts the hero
        void putCharOnBoard(size_t begR, size_t begC, size_t newR, size_t newC) {
            char typeOfHero = board(begR, begC)->display(); // gets the currHero
            freeCell(begR, begC); // frees the cell
            Nothing* no = new Nothing(begR, begC); // assigns it with nothing
            setCell(no, begR, begC); // sets the cell
            freeCell(newR, newC); // frees the new destination cell

            switch(typeOfHero) {
                // the charcter is the hero
                case 'H':
                    {
                    Hero* h = new Hero(newR, newC);
                    setCell(h, newR, newC);
                    }
                    break;
                // the caracter is the super monster
                case 'M':
                    {
                    Monster* M = new Monster(newR, newC);
                    M->setPower(2);
                    setCell(M, newR, newC);
                    }
                    break;
                // the character is the small monster
                case 'm':
                    {
                    Monster* m = new Monster(newR, newC);
                    setCell(m, newR, newC);
                    }
                    break;
                // the character is a bat
                case '~':
                    {
                    Bat* b = new Bat(newR, newC);
                    setCell(b, newR, newC);
                    }
                    break;
                default:
                    break;
            }
        }

        // return true if Hero is still on board at the end of the round
        // return false if Hero is NOT on board at the end of the round
        // moves all the figures for the particular move
        // fix some cols and/or rows if needed so the character can stay in bound or on the board overall
        bool makeMoves(char HeroNextMove) {
            // this function should use some try/catch statements for handling collision exceptions;
            // some sample code is provided to get you started...

            // determine where hero proposes to move to
            for (size_t row = 0; row < numRows; row++) {
                for (size_t col = 0; col < numCols; col++) {
                    BoardCell *cell = board(row, col);
                    if (cell->isBaddie()) {
                        board(row, col)->setMoved(false);
                    }
                }
            }

            size_t newR, newC;
            board(HeroRow,HeroCol)->setNextMove(HeroNextMove);
            board(HeroRow,HeroCol)->attemptMoveTo(newR,newC,HeroRow,HeroCol);
            bool isGone = false;
            try {
                // hero attempts to move out-of-bounds in rows
                if (newR < 0 || newR >= numRows) { 
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                newR = HeroRow;
                cout << "Changing row for Hero position to stay in-bounds" << endl;
            }
            try {
                // hero attempts to move out-of-bounds in cols
                if(newC < 0 || newC >= numCols) {
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid col");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                newC = HeroCol;
                cout << "Changing col for Hero position to stay in-bounds" << endl;
            }

            // the hero tries to move into a barrier
            try {
                // hero attempts to move into a wall
                if(board(newR, newC)->isBarrier()) {
                    throw runtime_error("Hero trying to move into a barrier");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                // the barrier is either on the left or the right
                newC = HeroCol;
                if(board(newR, newC)->isBarrier()) {
                    newR = HeroRow;
                }
                putCharOnBoard(HeroRow, HeroCol, newR, newC);
                setHeroPosition(newR, newC);
                cout << "Changing col and/or for Hero position to stay of the wall" << endl;
            }

            // hero had found the exit
            try{
                if(board(newR, newC)->isExit()) {
                    throw runtime_error("Hero found the exit");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                // setHeroPosition(-1, -1);
                wonGame = true;
                cout << "Hero had escaped" << endl;
                isGone = true;
            }

            // hero hit a hole
            try{
                if(board(newR, newC)->isHole()) {
                    throw runtime_error("Hero fall in a hole");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                cout << "Hero had died" << endl;
                isGone = true;
            }

            // the hero hit a monster/baddie
            try{
                if(board(newR, newC)->isBaddie()) {
                    throw runtime_error("Hero hit a monster");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                freeCell(HeroRow, HeroCol);
                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                
                cout << "Hero had died" << endl;
                isGone = true;
            }

            // hero just move
            try{
                if(board(newR, newC)->isSpace()) {
                    throw runtime_error("Hero just moved");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                putCharOnBoard(HeroRow, HeroCol, newR, newC);
                setHeroPosition(newR, newC);
                cout << "Hero moved" << endl;
            }
            
            // loops thru the board and updates the position of the baddies if they haven't had been moved yet
            for(size_t r = 0; r < numRows; r++) {
                for(size_t c = 0; c < numCols; c++) {
                    if(board(r, c)->isBaddie() && !(board(r, c)->getMoved())) {
                        size_t bNewR, bNewC;
                        board(r,c)->attemptMoveTo(bNewR,bNewC,HeroRow,HeroCol);
                        try {
                            // baddie attempts to move out-of-bounds in rows
                            if (bNewR < 0 || bNewR >= numRows) { 
                                throw runtime_error("Baddie is trying to move out-of-bounds with an invalid row");
                            }
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            bNewR = r;
                            cout << "Changing row for Baddie position to stay in-bounds" << endl;
                        }
                        try {
                            // baddie attempts to move out-of-bounds in cols
                            if(bNewC < 0 || bNewC >= numCols) {
                                throw runtime_error("Baddie is trying to move out-of-bounds with an invalid col");
                            }
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            bNewC = c;
                            cout << "Changing col for Baddie position to stay in-bounds" << endl;
                        }

                        // the baddie tries to move into a barrier
                        try {
                            // baddie attempts to move into a wall or exit
                            if(board(bNewR, bNewC)->isBarrier() || board(bNewR, bNewC)->isExit()) {
                                throw runtime_error("Baddie trying to move into a barrier");
                            }
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            continue;
                        }

                        // hero hit a hole
                        try{
                            if(board(bNewR, bNewC)->isHole()) {
                                throw runtime_error("Baddie fall in a hole");
                            }
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            freeCell(r, c);
                            BoardCell* no = new Nothing(r, c);
                            setCell(no, r, c);
                            cout << "Baddie is gone" << endl;
                        }

                        // the baddie hit the hero
                        try{
                            if(board(bNewR, bNewC)->isHero()) {
                                throw runtime_error("The Baddie found the Hero");
                            }
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            cout << "Hero had died" << endl;
                            isGone = true;
                            freeCell(HeroRow, HeroCol);
                            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                            board(bNewR, bNewC)->setMoved(true);
                            wonGame = false;
                        }

                        // baddie just move
                        try{
                            if(board(bNewR, bNewC)->isSpace()) {
                                throw runtime_error("Baddie just moved");
                            }
                        }
                        catch (runtime_error& excpt) {
                            cout << excpt.what() << endl;
                            putCharOnBoard(r, c, bNewR, bNewC);  
                            board(bNewR, bNewC)->setMoved(true);                          
                            cout << "Baddie moved" << endl;
                        }

                        // the baddie hits another baddie
                        if(board(bNewR, bNewC)->isBaddie()) {
                            continue;
                        }

                    }
                }
            }
            findHero();
            getHeroPosition(HeroRow, HeroCol);
            // the hero is off the board 
            // the game is over
            if(isGone) {
                setHeroPosition(-1, -1);
                return false;
            }
            // sets all the baddies as unmoved for the next move
            for(size_t r = 0; r < numRows; r++) {
                for(size_t c = 0; c < numCols; c++) {
                    if(board(r, c)->isBaddie()){
                        board(r, c)->setMoved(false);
                    }
                }
            }
            return true;
        }
       

    
};

#endif //_GAMEBOARD_H