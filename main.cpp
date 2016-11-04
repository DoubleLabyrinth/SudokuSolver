#include <iostream>
#include <SudokuSolver.h>
#include <windows.h>

using namespace std;

int main() {
    char data[9][9];
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            char srcData;
            srcData = (char)getchar();
            while(srcData == '\n') {
                srcData = (char)getchar();
            }
            data[i][j] = srcData;
        }
    }

    SudokuSolver test(data);
    cout << endl;
    test.PrintSudoku();
    LARGE_INTEGER t1, t2, t3;
    QueryPerformanceCounter(&t1);
    test.Solve();
    QueryPerformanceCounter(&t2);
    QueryPerformanceFrequency(&t3);
    cout << t2.QuadPart - t1.QuadPart << endl;
    cout << t3.QuadPart << endl;
    cout << endl;
    test.PrintSudoku();
    cout << (test.IsSudokuSolved() ? "True" : "False");
    return 0;
}
