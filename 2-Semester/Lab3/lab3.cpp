#include <raylib.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

vector<vector<double>> randm(int n) {
    vector<vector<double>> T(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[i][j] = (2.0 * rand()) / (RAND_MAX + 1.0);
        }
    }
    return T;
}

vector<vector<int>> mulmr(const vector<vector<double>>& T, double k) {
    int n = T.size();
    vector<vector<int>> A(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double val = T[i][j] * k;
            A[i][j] = (val >= 1.0) ? 1 : 0;
        }
    }
    return A;
}

vector<vector<int>> makeUndirected(const vector<vector<int>>& Adir) {
    int n = Adir.size();
    vector<vector<int>> Aundir = Adir;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (Adir[i][j] == 1) {
                Aundir[i][j] = 1;
                Aundir[j][i] = 1;
            }
        }
    }
    return Aundir;
}

void printMatrix(const string& title, const vector<vector<int>>& matrix) {
    cout << title << ":\n";
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void DrawGraph(const vector<vector<int>>& matrix, Vector2 center, float radius, bool isDirected) {
    int n = matrix.size();
    vector<Vector2> pos(n);
    float nodeRadius = 18.0f;

    for (int i = 0; i < n; i++) {
        float angle = i * (2 * PI / n);
        pos[i].x = center.x + radius * cos(angle);
        pos[i].y = center.y + radius * sin(angle);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                if (isDirected) {
                    Vector2 p1 = pos[i];
                    Vector2 p2 = pos[j];
                    
                    float dx = p2.x - p1.x;
                    float dy = p2.y - p1.y;
                    float length = sqrt(dx*dx + dy*dy);
                    
                    if (length > 0) {
                        dx /= length;
                        dy /= length;

                        Vector2 startP = { p1.x + dx * (nodeRadius + 2.0f), p1.y + dy * (nodeRadius + 2.0f) };

                        Vector2 arrowTip = { p2.x - dx * (nodeRadius + 4.0f), p2.y - dy * (nodeRadius + 4.0f) };

                        float arrowLen = 12.0f;
                        float arrowSpread = 0.45f;
                        Vector2 leftWing = { arrowTip.x - dx * arrowLen - dy * arrowLen * arrowSpread,
                                             arrowTip.y - dy * arrowLen + dx * arrowLen * arrowSpread };
                        Vector2 rightWing = { arrowTip.x - dx * arrowLen + dy * arrowLen * arrowSpread,
                                              arrowTip.y - dy * arrowLen - dx * arrowLen * arrowSpread };

                        DrawLineEx(startP, arrowTip, 2.0f, GRAY);
                        DrawLineEx(arrowTip, leftWing, 2.0f, RED);
                        DrawLineEx(arrowTip, rightWing, 2.0f, RED);
                    }
                } else {
                    if (i <= j) {
                        DrawLineEx(pos[i], pos[j], 2.0f, DARKGRAY);
                    }
                }
            }
        }
    }


    for (int i = 0; i < n; i++) {
        DrawCircleV(pos[i], nodeRadius, RAYWHITE);
        DrawCircleLines(pos[i].x, pos[i].y, nodeRadius, BLACK);
        
        string text = to_string(i);
        int textWidth = MeasureText(text.c_str(), 20);
        DrawText(text.c_str(), pos[i].x - textWidth / 2, pos[i].y - 10, 20, BLACK);
    }
}

int main() {
    srand(5426);
    int n = 11;
    double k = 0.73;

    vector<vector<double>> T = randm(n);
    vector<vector<int>> Adir = mulmr(T, k);
    vector<vector<int>> Aundir = makeUndirected(Adir);

    printMatrix("Матриця напрямленого графа (Adir)", Adir);
    printMatrix("Матриця ненапрямленого графа (Aundir)", Aundir);

    const int screenWidth = 1200;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Лабораторна робота 3: Графи (Raylib)");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Directed graph", 200, 30, 20, BLACK);
        DrawGraph(Adir, {300.0f, 320.0f}, 220.0f, true);

        DrawText("Undirected graph", 800, 30, 20, BLACK);
        DrawGraph(Aundir, {900.0f, 320.0f}, 220.0f, false);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}