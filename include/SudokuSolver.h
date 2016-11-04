#ifndef SUDOKUSOLVER_H
#define SUDOKUSOLVER_H

#include <iostream>

const unsigned short NumberMask[9] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100};
const char Number[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

typedef struct _Vector2D {
    int X;
    int Y;
} Vector2D;

const Vector2D Parts[9] = { {0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}, {0, 2}, {1, 2}, {2, 2} };

class UnknownPointQueue {
private:
    Vector2D UnknownPoint[81];
    int LastUnknownPointPosition;
public:
    UnknownPointQueue();
    bool AddUnknownPoint(const Vector2D& srcUnknownPoint);
    bool UnknownPointOut();
    Vector2D GetUnknownPoint(int i);
    int GetUnknownPointNumber();
    void Clear();
};

class SudokuSolver {
private:
    char SudokuData[9][9];
    Vector2D GetPart(const Vector2D& srcVector2D);
    unsigned short GetKnownNumberMaskFromPart(const Vector2D& srcPart);

public:
    bool IsNumberPossible(const Vector2D& srcVector2D, char Number);
    SudokuSolver();
    SudokuSolver(char (*srcSudokuData)[9]);
    bool IsSudokuSolved();
    void PrintSudoku();
    void Solve();
};

#endif // SUDOKUSOLVER_H
