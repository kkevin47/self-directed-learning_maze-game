#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <conio.h> // 用於 _getch() 函數

class MazeGame {
private:
    int size;          // 迷宮大小
    int playerX;       // 玩家 X 座標
    int playerY;       // 玩家 Y 座標
    int exitX;         // 出口 X 座標
    int exitY;         // 出口 Y 座標
    int playerSteps;   // 玩家步數
    int level;         // 關卡

    std::vector<std::vector<char>> maze; // 迷宮地圖

public:
    MazeGame(int s) : size(s), playerSteps(0), level(1) {
        srand(time(nullptr)); // 使用時間作為隨機種子
        generateMaze();      // 生成迷宮地圖
    }

    void generateMaze() {
        // 初始化迷宮地圖
        maze.assign(size, std::vector<char>(size, ' '));

        // 設置外牆
        for (int i = 0; i < size; ++i) {
            maze[i][0] = '#';             // 左牆
            maze[i][size - 1] = '#';      // 右牆
            maze[0][i] = '#';             // 上牆
            maze[size - 1][i] = '#';      // 下牆
        }

        // 將玩家和出口放在對角線上
        playerX = 1;
        playerY = 1;
        exitX = size - 2;
        exitY = size - 2;

        // 生成迷宮地圖（包含牆壁和通道）
        generateWalls();

        // 確保有從玩家到出口的通路
        while (!hasPath(playerX, playerY, exitX, exitY)) {
            // 如果沒有通路，重新生成迷宮
            generateMaze();
        }
    }

    void generateWalls() {
        // 根據當前等級計算牆壁密度
        double wallDensity = 0.5 + level * 0.00025; // 每級增加 0.25% 的牆壁密度

        // 根據牆壁密度隨機設置牆壁，但排除玩家起點和出口位置
        for (int y = 1; y < size - 1; ++y) {
            for (int x = 1; x < size - 1; ++x) {
                if ((x != playerX || y != playerY) && (x != exitX || y != exitY)) {
                    if (rand() % 100 < wallDensity * 100) {
                        maze[y][x] = '#';
                    }
                }
            }
        }

        // 添加分支，也需要排除玩家起點和出口位置
        for (int i = 0; i < 40; ++i) { // 確保有 40 個分支點
            int branchX, branchY;
            do {
                branchX = rand() % (size - 2) + 1;
                branchY = rand() % (size - 2) + 1;
            } while ((branchX == playerX && branchY == playerY) || (branchX == exitX && branchY == exitY));
            maze[branchY][branchX] = ' ';
        }
    }

    bool hasPath(int startX, int startY, int targetX, int targetY) {
        // 深度優先搜索（DFS）檢查是否有從起點到目標的通路
        std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));
        return dfs(startX, startY, targetX, targetY, visited);
    }

    bool dfs(int x, int y, int targetX, int targetY, std::vector<std::vector<bool>>& visited) {
        if (x == targetX && y == targetY) {
            return true; // 找到通路到目標
        }

        visited[y][x] = true;

        // 檢查相鄰的格子
        static const int dx[] = { 1, -1, 0, 0 };
        static const int dy[] = { 0, 0, 1, -1 };
        for (int i = 0; i < 4; ++i) {
            int newX = x + dx[i];
            int newY = y + dy[i];
            if (newX >= 0 && newX < size && newY >= 0 && newY < size &&
                maze[newY][newX] != '#' && !visited[newY][newX]) {
                if (dfs(newX, newY, targetX, targetY, visited)) {
                    return true; // 找到通路到目標
                }
            }
        }

        return false; // 沒有通路到目標
    }

    void displayMaze() {
        // 移動游標到 (0, 0) 位置
        std::cout << "\033[H";

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                if (x == playerX && y == playerY) {
                    std::cout << 'P' << ' '; // 玩家位置
                }
                else if (x == exitX && y == exitY) {
                    std::cout << 'E' << ' '; // 出口位置
                }
                else {
                    std::cout << maze[y][x] << ' '; // 其他格子
                }
            }
            std::cout << std::endl;
        }
        std::cout << "關卡: " << level << ", 步數: ";
        if (playerSteps == -1) {
            std::cout << "0"; // 完成等級後顯示 0 步
        }
        else {
            std::cout << playerSteps; // 否則顯示當前步數
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
                size++; // 下一等級增加迷宮大小
                if (level == 11) { //第10關遊戲結束
                    std::cout << "恭喜通關!!!" << std::endl;
                    std::cout << "按下任意按鍵關閉視窗..." << std::endl;
                    _getch(); // 等待用戶輸入任意按鍵
                    return true; // 結束遊戲
                }
                playerSteps = 0; // 重置步數
                generateMaze();
            }
            else {
                displayMaze();
            }
        }
        return false; // 遊戲繼續
    }
};

int main() {
    int size = 19; // 初始迷宮大小
    MazeGame game(size);
    game.displayMaze();

    char move;
    while (true) {
        std::cout << "請輸入 w/a/s/d 移動或 Esc 退出: ";
        move = _getch();
        if (move == 27) { // 按下 Esc 鍵退出遊戲
            break;
        }
        if (!game.movePlayer(move)) {
            std::cout << "無效移動" << std::endl;
        }
    }
    return 0;
}
