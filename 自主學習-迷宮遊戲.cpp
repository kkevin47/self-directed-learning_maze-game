#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <conio.h> // �Ω� _getch() ���

class MazeGame {
private:
    int size;          // �g�c�j�p
    int playerX;       // ���a X �y��
    int playerY;       // ���a Y �y��
    int exitX;         // �X�f X �y��
    int exitY;         // �X�f Y �y��
    int playerSteps;   // ���a�B��
    int level;         // ���d

    std::vector<std::vector<char>> maze; // �g�c�a��

public:
    MazeGame(int s) : size(s), playerSteps(0), level(1) {
        srand(time(nullptr)); // �ϥήɶ��@���H���ؤl
        generateMaze();      // �ͦ��g�c�a��
    }

    void generateMaze() {
        // ��l�ưg�c�a��
        maze.assign(size, std::vector<char>(size, ' '));

        // �]�m�~��
        for (int i = 0; i < size; ++i) {
            maze[i][0] = '#';             // ����
            maze[i][size - 1] = '#';      // �k��
            maze[0][i] = '#';             // �W��
            maze[size - 1][i] = '#';      // �U��
        }

        // �N���a�M�X�f��b�﨤�u�W
        playerX = 1;
        playerY = 1;
        exitX = size - 2;
        exitY = size - 2;

        // �ͦ��g�c�a�ϡ]�]�t����M�q�D�^
        generateWalls();

        // �T�O���q���a��X�f���q��
        while (!hasPath(playerX, playerY, exitX, exitY)) {
            // �p�G�S���q���A���s�ͦ��g�c
            generateMaze();
        }
    }

    void generateWalls() {
        // �ھڷ�e���ŭp������K��
        double wallDensity = 0.5 + level * 0.00025; // �C�żW�[ 0.25% ������K��

        // �ھ�����K���H���]�m����A���ư����a�_�I�M�X�f��m
        for (int y = 1; y < size - 1; ++y) {
            for (int x = 1; x < size - 1; ++x) {
                if ((x != playerX || y != playerY) && (x != exitX || y != exitY)) {
                    if (rand() % 100 < wallDensity * 100) {
                        maze[y][x] = '#';
                    }
                }
            }
        }

        // �K�[����A�]�ݭn�ư����a�_�I�M�X�f��m
        for (int i = 0; i < 40; ++i) { // �T�O�� 40 �Ӥ����I
            int branchX, branchY;
            do {
                branchX = rand() % (size - 2) + 1;
                branchY = rand() % (size - 2) + 1;
            } while ((branchX == playerX && branchY == playerY) || (branchX == exitX && branchY == exitY));
            maze[branchY][branchX] = ' ';
        }
    }

    bool hasPath(int startX, int startY, int targetX, int targetY) {
        // �`���u���j���]DFS�^�ˬd�O�_���q�_�I��ؼЪ��q��
        std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));
        return dfs(startX, startY, targetX, targetY, visited);
    }

    bool dfs(int x, int y, int targetX, int targetY, std::vector<std::vector<bool>>& visited) {
        if (x == targetX && y == targetY) {
            return true; // ���q����ؼ�
        }

        visited[y][x] = true;

        // �ˬd�۾F����l
        static const int dx[] = { 1, -1, 0, 0 };
        static const int dy[] = { 0, 0, 1, -1 };
        for (int i = 0; i < 4; ++i) {
            int newX = x + dx[i];
            int newY = y + dy[i];
            if (newX >= 0 && newX < size && newY >= 0 && newY < size &&
                maze[newY][newX] != '#' && !visited[newY][newX]) {
                if (dfs(newX, newY, targetX, targetY, visited)) {
                    return true; // ���q����ؼ�
                }
            }
        }

        return false; // �S���q����ؼ�
    }

    void displayMaze() {
        // ���ʴ�Ш� (0, 0) ��m
        std::cout << "\033[H";

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                if (x == playerX && y == playerY) {
                    std::cout << 'P' << ' '; // ���a��m
                }
                else if (x == exitX && y == exitY) {
                    std::cout << 'E' << ' '; // �X�f��m
                }
                else {
                    std::cout << maze[y][x] << ' '; // ��L��l
                }
            }
            std::cout << std::endl;
        }
        std::cout << "���d: " << level << ", �B��: ";
        if (playerSteps == -1) {
            std::cout << "0"; // �������ū���� 0 �B
        }
        else {
            std::cout << playerSteps; // �_�h��ܷ�e�B��
        }
        std::cout << std::endl;
    }

    bool movePlayer(char direction) {
        int newX = playerX;
        int newY = playerY;
        switch (direction) {
        case 'w':
            newY--;
            break;
        case 'a':
            newX--;
            break;
        case 's':
            newY++;
            break;
        case 'd':
            newX++;
            break;
        default:
            return false;
        }

        if (newX >= 0 && newX < size && newY >= 0 && newY < size && maze[newY][newX] != '#') {
            playerX = newX;
            playerY = newY;
            playerSteps++;
            if (playerX == exitX && playerY == exitY) {
                level++;
                size++; // �U�@���żW�[�g�c�j�p
                if (level == 11) { //��10���C������
                    std::cout << "���߳q��!!!" << std::endl;
                    std::cout << "���U���N������������..." << std::endl;
                    _getch(); // ���ݥΤ��J���N����
                    return true; // �����C��
                }
                playerSteps = 0; // ���m�B��
                generateMaze();
            }
            else {
                displayMaze();
            }
        }
        return false; // �C���~��
    }
};

int main() {
    int size = 19; // ��l�g�c�j�p
    MazeGame game(size);
    game.displayMaze();

    char move;
    while (true) {
        std::cout << "�п�J w/a/s/d ���ʩ� Esc �h�X: ";
        move = _getch();
        if (move == 27) { // ���U Esc ��h�X�C��
            break;
        }
        if (!game.movePlayer(move)) {
            std::cout << "�L�Ĳ���" << std::endl;
        }
    }
    return 0;
}
