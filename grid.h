//-------------------------------------------------
// Project 5 - Outlast the Baddies & Avoid the Abyss
// Name: Mihaela Harizanova
// System: Advanced zyLab
// Course: CS 211, Summer 2024, UIC
//-------------------------------------------------

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
    struct CELL {
        CELL* Next;
        T Val;
        size_t NumCols;  // total # of columns (0..NumCols-1)
        CELL(CELL* _Next = nullptr, T _Val = T(), size_t _NumCols = 0) {
            Next = _Next;
            Val = _Val;
            NumCols = _NumCols;
        }
    };
    
    size_t NumRows;  // total # of rows (0..NumRows-1)
    CELL** Rows;     // C array of linked lists
public:
    // default constructor: 4x4 Grid  
    // all elements are initialized to the default value
    Grid() {
        // initialize 4 rows
        Rows = new CELL*[4];
        NumRows = 4;
        
        // allocate the first cell of the linked list with default value:
        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r] = new CELL(nullptr, T(), 4);
            CELL* cur = Rows[r];
            
            // create the linked list for this row.
            for(size_t c = 1; c < Rows[r]->NumCols; ++c) {
                cur->Next = new CELL(nullptr, T());
                cur = cur->Next;
            }
        }
        
    }
    
    // parameterized constructor: RxC Grid
    Grid(size_t R, size_t C) {
        Rows = new CELL*[R];
        NumRows = R;
        
        // allocate the first cell of the linked list with default value:
        for (size_t r = 0; r < R; ++r) {
            Rows[r] = new CELL(nullptr, T(), C);
            CELL* cur = Rows[r];
            
            // create the linked list for this row.
            for(size_t c = 1; c < Rows[r]->NumCols; ++c) {
                cur->Next = new CELL(nullptr, T());
                cur = cur->Next;
            }
        }        
    }

    // destructor:
    virtual ~Grid() {
        // loops thru the Grid and frees the allocated memory
      for(size_t i = 0; i < NumRows; i++) {
        CELL* cur = Rows[i];
        // loops thru the curr rows and frees each one
        while(cur != nullptr) {
            CELL* next = cur->Next;
            delete cur;
            cur = next;
        }
      }
      delete[] Rows;

        
    }
    
    // copy constructor:
    Grid(const Grid<T>& other) {
        NumRows = other.NumRows;

        Rows = new CELL*[NumRows];

        // copy each row
        for(size_t i = 0; i < NumRows; ++i) {
            CELL* cur = nullptr;
            CELL* otherCur = other.Rows[i];

            // copy the first cell of the linked list
            Rows[i] = new CELL(nullptr, otherCur->Val, otherCur->NumCols);
            cur = Rows[i];
            otherCur = otherCur->Next;

            // loops thru the new cur and copies with the vals of the old cur
            while(otherCur != nullptr) {
                cur->Next = new CELL(nullptr, otherCur->Val);
                cur = cur->Next;
                otherCur = otherCur->Next;
            }
        }
        
    }
    
    // Called when you assign one vector into another, i.e. this = other;
    Grid& operator=(const Grid& other) {
        if (this == &other) {
            return *this;
        }
       
        // Deallocate the memory for the current grid
        for (size_t r = 0; r < NumRows; ++r) {
            CELL* cur = Rows[r];
            while (cur != nullptr) {
                CELL* next = cur->Next;
                delete cur;
                cur = next;
            }
        }
        delete[] Rows;
       
        // Copy the number of rows
        NumRows = other.NumRows;
       
        // Allocate memory for the array of rows
        Rows = new CELL*[NumRows];
       
        // Copy each row
        for (size_t r = 0; r < NumRows; ++r) {
            CELL* cur = other.Rows[r];
            CELL* prev = nullptr;
           
            // Copy the first cell of the linked list
            Rows[r] = new CELL(nullptr, cur->Val, cur->NumCols);
            prev = Rows[r];
            cur = cur->Next;
           
            // Copy the rest of the linked list
            while (cur != nullptr) {
                prev->Next = new CELL(nullptr, cur->Val);
                prev = prev->Next;
                cur = cur->Next;
            }
        }
       
        return *this;
    }


    // Returns the # of rows in the Grid.  The indices for these rows
    // are 0..numrows-1.
    size_t numrows() const {
        return NumRows; 
    }
    
    // Returns the # of columns in row r.  
    size_t numcols(size_t r) const {
        if(r < 0 || r >= NumRows) {
            throw invalid_argument("Invalid row index");
        }

        return Rows[r]->NumCols;  
        
    }

    // Returns the total # of elements in the grid.
    size_t size() const {
        size_t total = 0;
        for(size_t i = 0; i < NumRows; i++) {
            total += numcols(i);
        }
        return total; 
    }

    // Returns a reference to the element at location (r, c)
    T& operator()(size_t r, size_t c) {
        if (r >= NumRows || c >= Rows[r]->NumCols) {
            throw out_of_range("Invalid row or column index");
        }

        CELL* cur = Rows[r];

        for(size_t i = 0; i < c; i++){
            cur = cur->Next;
        }
        
        
        return cur->Val;
    }
    
    // Outputs the contents of the grid; for debugging purposes.  
    void _output() {
        for(size_t r = 0; r < NumRows; r++){
            CELL* cur = Rows[r];


            while(cur != nullptr){
                cout << cur->Val << " ";
                cur = cur->Next;
            }
            cout << endl;
        }
    }

    
};
