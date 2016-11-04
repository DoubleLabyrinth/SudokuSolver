#include "SudokuSolver.h"

UnknownPointQueue::UnknownPointQueue() {
    LastUnknownPointPosition = -1;
}

bool UnknownPointQueue::AddUnknownPoint(const Vector2D& srcUnknownPoint) {
    if(LastUnknownPointPosition <= 79) {
        UnknownPoint[++LastUnknownPointPosition] = srcUnknownPoint;
        return true;
    }
    return false;
}

bool UnknownPointQueue::UnknownPointOut() {
    if(LastUnknownPointPosition == -1) return false;
    for(int i = 0; i < LastUnknownPointPosition; i++) {
        UnknownPoint[i] = UnknownPoint[i + 1];
    }
    LastUnknownPointPosition--;
    return true;
}

Vector2D UnknownPointQueue::GetUnknownPoint(int i) {
    if(i > LastUnknownPointPosition || i < 0) return {-1, -1};
    return UnknownPoint[i];
}

int UnknownPointQueue::GetUnknownPointNumber() {
    return LastUnknownPointPosition + 1;
}

void UnknownPointQueue::Clear() {
    LastUnknownPointPosition = -1;
}

SudokuSolver::SudokuSolver() {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            SudokuData[i][j] = '*';
        }
    }
}

SudokuSolver::SudokuSolver(char (*srcSudokuData)[9]) {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if(srcSudokuData[i][j] <= '9' && srcSudokuData[i][j] >= '1') {
                SudokuData[i][j] = srcSudokuData[i][j];
            } else {
                SudokuData[i][j] = '*';
            }
        }
    }
}

bool SudokuSolver::IsSudokuSolved() {
    unsigned short Mask = 0;

    for(int i = 0; i < 9; i++) {
        Mask = 0;
        for(int j = 0; j < 9; j++) {
            int diff = SudokuData[i][j] - '1';
            if(diff > 8 || diff < 0) return false;   //illegal number showed. Sudoku must not be solved.
            if((Mask & NumberMask[diff]) != 0) return false;
            Mask ^= NumberMask[diff];
        }
        if(Mask != 0x1FF) return false;
    }

    for(int i = 0; i < 9; i++) {
        Mask = 0;
        for(int j = 0; j < 9; j++) {
            int diff = SudokuData[j][i] - '1';
            if(diff > 8 || diff < 0) return false;   //illegal number showed. Sudoku must not be solved.
            if((Mask & NumberMask[diff]) != 0) return false;
            Mask ^= NumberMask[diff];
        }
        if(Mask != 0x1FF) return false;
    }

    return true;
}

void SudokuSolver::PrintSudoku() {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            std::cout << SudokuData[i][j] << "  ";
        }
        std::cout << std::endl;
    }
}

Vector2D SudokuSolver::GetPart(const Vector2D& srcVector2D) {
    Vector2D ret;
    ret.X = srcVector2D.X / 3;
    ret.Y = srcVector2D.Y / 3;
    return ret;
}

unsigned short SudokuSolver::GetKnownNumberMaskFromPart(const Vector2D& srcPart) {
    unsigned short ret = 0;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            char tmp = SudokuData[3 * srcPart.Y + i][3 * srcPart.X + j];
            if(tmp == '*') continue;
            ret |= NumberMask[tmp - '1'];
        }
    }
    return ret;
}

bool SudokuSolver::IsNumberPossible(const Vector2D& srcVector2D, char Number) {
    for(int i = 0; i < 9; i++) if(SudokuData[srcVector2D.Y][i] == Number && i != srcVector2D.X) return false;
    for(int j = 0; j < 9; j++) if(SudokuData[j][srcVector2D.X] == Number && j != srcVector2D.Y) return false;
    return true;
}

void SudokuSolver::Solve() {
    bool DoAgain = false;
    bool Changed = false;
    UnknownPointQueue unkPoint;

    do {
        DoAgain = false;
        do {
            Changed = false;
            for(int i = 0; i < 9; i++) {
                for(int j = 0; j < 9; j++) {
                    if(SudokuData[i][j] == '*') unkPoint.AddUnknownPoint({j, i});
                }
            }

            int unkPointNumber = unkPoint.GetUnknownPointNumber();
            for(int k = 0; k < unkPointNumber; k++) {
                unsigned short KnownNumberMask = 0;
                Vector2D unknownPoint = unkPoint.GetUnknownPoint(k);
                Vector2D PartBelong = GetPart(unknownPoint);

                for(int i = 0; i < 9; i++) {
                    char tmp = SudokuData[i][unknownPoint.X];
                    if(tmp == '*') continue;
                    KnownNumberMask |= NumberMask[tmp - '1'];
                }

                for(int j = 0; j < 9; j++) {
                    char tmp = SudokuData[unknownPoint.Y][j];
                    if(tmp == '*') continue;
                    KnownNumberMask |= NumberMask[tmp - '1'];
                }

                KnownNumberMask |= GetKnownNumberMaskFromPart(PartBelong);

                int PossibleNumberCount = 0;
                char PossibleNumber = 0;
                for(int i = 0; i < 9; i++) {
                    if((KnownNumberMask & NumberMask[i]) == 0) {
                        PossibleNumberCount++;
                        if(PossibleNumberCount == 1) PossibleNumber = Number[i];
                    }
                }
                if(PossibleNumberCount == 1) {
                    SudokuData[unknownPoint.Y][unknownPoint.X] = PossibleNumber;
                    Changed = DoAgain = true;
                }
            }

            unkPoint.Clear();
        } while(Changed);

        do {
            Changed = false;

            for(int k = 0; k < 9; k++) {

                for(int a = 0; a < 3; a++) {
                    for(int b = 0; b < 3; b++) {
                        if(SudokuData[3 * Parts[k].Y + a][3 * Parts[k].X + b] == '*') unkPoint.AddUnknownPoint({3 * Parts[k].X + b, 3 * Parts[k].Y + a});
                    }
                }

                unsigned short KnownNumberMask = GetKnownNumberMaskFromPart(Parts[k]);
                int UnknownNumberCount = unkPoint.GetUnknownPointNumber();
                for(int i = 0; i < 9; i++) {
                    if((NumberMask[i] & KnownNumberMask) != 0) continue;
                    int ImpossibleCount = 0;
                    Vector2D PossiblePoint;
                    for(int j = 0; j < UnknownNumberCount; j++) {
                        if(!IsNumberPossible(unkPoint.GetUnknownPoint(j), Number[i])) ImpossibleCount++;
                        else PossiblePoint = unkPoint.GetUnknownPoint(j);
                    }
                    if(ImpossibleCount == UnknownNumberCount - 1) {
                        SudokuData[PossiblePoint.Y][PossiblePoint.X] = Number[i];
                        Changed = DoAgain = true;
                    }
                }
                unkPoint.Clear();
            }
        } while(Changed);
        if(IsSudokuSolved()) return;
    } while(DoAgain);
}
