#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#define PLAYER_X 1
#define PLAYER_O 2


//顯示規則
void ShowRules() {
    system("cls");
    printf("|------------------------------------------------------------------------------------------|\n");
    printf("|                                      Introduction Rules                                  |\n");
    printf("|------------------------------------------------------------------------------------------|\n");
    printf("| 1. Two players take turns making moves. Until one player connects five pieces.           |\n");
    printf("| 2. Use WASD to control the position, press the spacebar to place a piece.                  |\n");
    printf("| 3. Press L to end the game.                                                              |\n");
    printf("|------------------------------------------------------------------------------------------|\n");
    system("pause");
}

//把int轉成char，turn是用int存的，方便陣列做判斷
char intToChar(int num) {
    switch (num) {
        case 0:
            return '.';
        case PLAYER_X:
            return 'X';
        case PLAYER_O:
            return 'O';
        case 9:
            return '+';
    }
}

//顯示棋盤
void ShowBoard(int board[][15], int *position, int currentPlayer) {
    system("cls");
    printf("Now is  %c  turn \n", intToChar(currentPlayer));

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (i == position[0] && j == position[1]) {
                printf("[ %c ]", intToChar(board[i][j]));
            } else {
                printf("  %c  ", intToChar(board[i][j]));
            }
        }
        printf("\n\n");
    }
}

//檢查x軸是否五子
int checkHorizontal(int board[][15], int y, int x, int player) {
    int stoneCount = 0;
    for (int i = x - 4; i <= x + 4; i++) {
        if (board[y][i] == player) {
            stoneCount++;
            if (stoneCount == 5) {
                break;
            }
        } else {
            stoneCount = 0;
        }
    }
    //當布林值用
    return (stoneCount >= 5);
}


//檢查y軸是否五子
int checkVertical(int board[][15], int y, int x, int player) {
    int stoneCount = 0;
    for (int i = y - 4; i <= y + 4; i++) {
        if (board[i][x] == player) {
            stoneCount++;
            if (stoneCount == 5) {
                break;
            }
        } else {
            stoneCount = 0;
        }
    }
    return (stoneCount >= 5);
}


//檢查y=-x線上是否五子
int checkDiagonalSlash(int board[][15], int y, int x, int player) {
    int stoneCount = 0;
    for (int i = y - 4, j = x - 4; i <= y + 4; i++, j++) {
        if (board[i][j] == player) {
            stoneCount++;
            if (stoneCount == 5) {
                break;
            }
        } else {
            stoneCount = 0;
        }
    }
    return (stoneCount >= 5);
}

//檢查y=x線上是否五子
int checkDiagonalBackslash(int board[][15], int y, int x, int player) {
    int stoneCount = 0;
    for (int i = y + 4, j = x - 4; i >= y - 4; i--, j++) {
        if (board[i][j] == player) {
            stoneCount++;
            if (stoneCount == 5) {
                break;
            }
        } else {
            stoneCount = 0;
        }
    }
    return (stoneCount >= 5);
}

//保存紀錄
void SaveHistory(char *player1, char *player2, char *winner) {
    FILE *file = fopen("history.txt", "a"); //a是append在後面寫上新資料
    if (file != NULL) { //除錯
        fprintf(file, "player1: %s, player2: %s, winner: %s\n", player1, player2, winner); //寫入txt
        fclose(file);
    }
}

//顯示紀錄
void ShowHistory() {
    FILE *file = fopen("history.txt", "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    } else {
        printf("|------------------------------------------------------------------------------------------|\n");
        printf("|                            Cannot find the history record.                               |\n");
        printf("|------------------------------------------------------------------------------------------|\n");
    }
    system("pause");
}

//開始遊戲
void startNewGame() {
    system("cls");
    int board[15][15] = {0};
    board[3][3] = board[3][11] = board[7][7] = board[11][3] = board[11][11] = 9; //intToChar(9)=+

    int position[] = {7, 7}; //一開始的位置(列,行)
    int currentPlayer = PLAYER_X;   //X先手
    int winner = 0; //還沒有人贏
    char player1[50], player2[50];
    printf("|------------------------------------------------------------------------------------------|\n");
    printf("|                                  Please enter player names                               |\n");
    printf("|------------------------------------------------------------------------------------------|\n");
    printf("| Please enter p1 name: ");
    gets(player1);
    printf("| Please enter p2 name: ");
    gets(player2);
    printf("|------------------------------------------------------------------------------------------|\n");

    while (1) {
        //不斷刷新
        ShowBoard(board, position, currentPlayer);

        if (winner != 0) {
            char *winnerName;
            if (winner == PLAYER_X) {
                winnerName = player1;
            } else {
                winnerName = player2;
            }
            SaveHistory(player1, player2, winnerName);
            char choice;
            printf("Chicken chicken winner dinner,\n %s (%c) wins!\n",winnerName , intToChar(winner));
            printf("Again?(Y/N): ");
            choice = getch();
            if (choice == 'Y' || choice == 'y') {
                startNewGame();
            } else {
                printf("Game over.\n");
                return;
            }
        }

        int key = getch();

        switch (key) {
            case 'w':   //大小寫都接收
            case 'W':
                if (position[0] == 0) position[0] = 14; //如果到最上面了，跳到最後一行
                else position[0]--; //其餘時候往上一列
                break;
            case 'a':
            case 'A':
                if (position[1] == 0) position[1] = 14; //同上
                else position[1]--;
                break;
            case 's':
            case 'S':
                if (position[0] == 14) position[0] = 0;
                else position[0]++;
                break;
            case 'd':
            case 'D':
                if (position[1] == 14) position[1] = 0;
                else position[1]++;
                break;
            case 32: //32是空白鍵的ASCII
                //檢查該格是不是空的(不是X也不是O)，空的話則把把棋盤上的那格改成 currentPlayer(玩家的子)
                if (board[position[0]][position[1]] != PLAYER_X && board[position[0]][position[1]] != PLAYER_O) {
                    board[position[0]][position[1]] = currentPlayer;
                    //檢查任意方向是否連珠
                    if (checkHorizontal(board, position[0], position[1], currentPlayer) ||
                        checkVertical(board, position[0], position[1], currentPlayer) ||
                        checkDiagonalSlash(board, position[0], position[1], currentPlayer) ||
                        checkDiagonalBackslash(board, position[0], position[1], currentPlayer)) {
                        winner = currentPlayer;
                        break;
                    }
                    //換邊
                    if (currentPlayer == PLAYER_X) currentPlayer = PLAYER_O;
                        else currentPlayer = PLAYER_X;
                    //上面那個if沒過，代表此處有子了
                    }
                else {
                    printf("Already one here, please try another place. \n");
                    system("pause");
                }
                break;
            case 'l':
            case 'L':
                system("cls");
                printf("End program ..\n");
                system("pause");
                return;
            default:
                printf("Invalid keypress !\n");
                system("pause");
                break;
        }
    }
}


int main(void) {


    while (1) {
        system("cls");
        printf("|------------------------------------------------------------------------------------------|\n");
        printf("|                                         Menu                                             |\n");
        printf("|------------------------------------------------------------------------------------------|\n");
        printf("| 0. Start                                                                                 |\n");
        printf("| 1. Rules                                                                                 |\n");
        printf("| 2. History                                                                               |\n");
        printf("|------------------------------------------------------------------------------------------|\n");

        int choice = getch();
        switch (choice) {
            case '0':
                startNewGame();
                break;
            case '1':
                ShowRules();
                break;
            case '2':
                ShowHistory();
                break;
            default:
                printf("Invalid keypress !\n");
                system("pause");
                break;
        }
    }
    return 0;
}
