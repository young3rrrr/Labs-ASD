#include <raylib.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <queue>

using namespace std;

// Структура для зберігання кроку анімації (ребро дерева обходу)
struct EdgeStep {
    int from;
    int to;
};

// --- ГЕНЕРАЦІЯ ГРАФА ---
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

// Пошук початкової вершини
int findStartNode(const vector<vector<int>>& Adir) {
    int n = Adir.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (Adir[i][j] == 1) return i; 
        }
    }
    return 0; 
}

// Обхід в ширину (BFS)
vector<EdgeStep> getBFS_Tree(const vector<vector<int>>& Adir, int startNode) {
    int n = Adir.size();
    vector<EdgeStep> treeEdges;
    vector<bool> visited(n, false);
    queue<int> q;

    for (int i = 0; i < n; i++) {
        int currentStart = (i == 0) ? startNode : i;
        
        if (!visited[currentStart]) {
            bool hasOut = false;
            for(int j=0; j<n; j++) if(Adir[currentStart][j] == 1) hasOut = true;
            
            if (!hasOut && i != 0) continue;

            visited[currentStart] = true;
            q.push(currentStart);

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v = 0; v < n; v++) {
                    if (Adir[u][v] == 1 && !visited[v]) {
                        visited[v] = true;
                        treeEdges.push_back({u, v});
                        q.push(v);
                    }
                }
            }
        }
    }
    return treeEdges;
}

// Обхід в глибину (DFS)
vector<EdgeStep> getDFS_Tree(const vector<vector<int>>& Adir, int startNode) {
    int n = Adir.size();
    vector<EdgeStep> treeEdges;
    vector<bool> visited(n, false);
    
    auto dfs = [&](auto& self, int u) -> void {
        visited[u] = true;
        for (int v = 0; v < n; v++) {
            if (Adir[u][v] == 1 && !visited[v]) {
                treeEdges.push_back({u, v});
                self(self, v);
            }
        }
    };

    for (int i = 0; i < n; i++) {
        int currentStart = (i == 0) ? startNode : i;
        if (!visited[currentStart]) {
             bool hasOut = false;
             for(int j=0; j<n; j++) if(Adir[currentStart][j] == 1) hasOut = true;
             if (!hasOut && i != 0) continue;
             
             dfs(dfs, currentStart);
        }
    }
    return treeEdges;
}

void DrawGraphAnimated(const vector<vector<int>>& matrix, Vector2 center, float radius, 
                       const vector<EdgeStep>& activeEdges, int currentStep, Color highlightColor) {
    int n = matrix.size();
    vector<Vector2> pos(n);
    float nodeRadius = 22.0f;

    for (int i = 0; i < n; i++) {
        float angle = i * (2 * PI / n);
        pos[i].x = center.x + radius * cos(angle);
        pos[i].y = center.y + radius * sin(angle);
    }

    // БАЗОВИЙ граф
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1 && i != j) {
                Vector2 p1 = pos[i];
                Vector2 p2 = pos[j];
                float dx = p2.x - p1.x; float dy = p2.y - p1.y;
                float length = sqrt(dx * dx + dy * dy);
                if (length > 0) { dx /= length; dy /= length; }

                float arrowOffset = nodeRadius + 3.0f; 
                Vector2 startP = { p1.x + dx * nodeRadius, p1.y + dy * nodeRadius };
                Vector2 endP = { p2.x - dx * arrowOffset, p2.y - dy * arrowOffset };
                DrawLineEx(startP, endP, 1.0f, LIGHTGRAY); // Тонка лінія
            }
        }
    }

    // ДЕРЕВО ОБХОДУ
    for (int k = 0; k < currentStep && k < activeEdges.size(); k++) {
        int i = activeEdges[k].from;
        int j = activeEdges[k].to;
        
        Vector2 p1 = pos[i];
        Vector2 p2 = pos[j];
        float dx = p2.x - p1.x; float dy = p2.y - p1.y;
        float length = sqrt(dx * dx + dy * dy);
        if (length > 0) { dx /= length; dy /= length; }

        float arrowOffset = nodeRadius + 3.0f; 
        Vector2 startP = { p1.x + dx * nodeRadius, p1.y + dy * nodeRadius };
        Vector2 endP = { p2.x - dx * arrowOffset, p2.y - dy * arrowOffset };

        DrawLineEx(startP, endP, 4.0f, highlightColor); // Товста лінія

        float arrowSize = 18.0f;
        Vector2 arrowP1 = { endP.x - dx * arrowSize - dy * arrowSize * 0.4f, 
                            endP.y - dy * arrowSize + dx * arrowSize * 0.4f };
        Vector2 arrowP2 = { endP.x - dx * arrowSize + dy * arrowSize * 0.4f, 
                            endP.y - dy * arrowSize - dx * arrowSize * 0.4f };
        DrawTriangle(endP, arrowP1, arrowP2, highlightColor);
        DrawTriangle(endP, arrowP2, arrowP1, highlightColor);
    }

    // ВЕРШИНИ
    for (int i = 0; i < n; i++) {
        DrawCircleV(pos[i], nodeRadius, RAYWHITE);
        DrawCircleLines(pos[i].x, pos[i].y, nodeRadius, BLACK);
        string text = to_string(i);
        DrawText(text.c_str(), pos[i].x - MeasureText(text.c_str(), 20) / 2, pos[i].y - 10, 20, BLACK);
    }
}
// g++ lab5.cpp -o lab5 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
int main() {
    srand(5426);
    int n = 12;  
    double k = 1.0 - 2*0.01 - 6*0.005 - 0.15;

    vector<vector<double>> T = randm(n);
    vector<vector<int>> Adir = mulmr(T, k);

    int startNode = findStartNode(Adir);
    cout << "Стартова вершина для обходу: " << startNode << endl;

    const int screenWidth = 1000;
    const int screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "Lab 5: BFS and DFS Animation");
    SetTargetFPS(60);

    vector<EdgeStep> currentPath;
    int currentStep = 0;
    float timer = 0.0f;
    float stepDelay = 0.5f;
    Color highlightColor = DARKGRAY;
    string currentMode = "Idle";

    Rectangle btnBFS = { 100, 30, 150, 40 };
    Rectangle btnDFS = { 300, 30, 150, 40 };

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();
        
        if (CheckCollisionPointRec(mousePoint, btnBFS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            currentPath = getBFS_Tree(Adir, startNode);
            currentStep = 0; timer = 0.0f;
            highlightColor = LIME;
            currentMode = "BFS Running...";
        }
        
        if (CheckCollisionPointRec(mousePoint, btnDFS) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            currentPath = getDFS_Tree(Adir, startNode);
            currentStep = 0; timer = 0.0f;
            highlightColor = RED;
            currentMode = "DFS Running...";
        }

        if (currentStep < currentPath.size()) {
            timer += GetFrameTime();
            if (timer >= stepDelay) {
                currentStep++;
                timer = 0.0f;
            }
        } else if (currentPath.size() > 0) {
            currentMode = "Finished";
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleRec(btnBFS, CheckCollisionPointRec(mousePoint, btnBFS) ? LIGHTGRAY : GRAY);
        DrawText("Run BFS", btnBFS.x + 35, btnBFS.y + 10, 20, BLACK);

        DrawRectangleRec(btnDFS, CheckCollisionPointRec(mousePoint, btnDFS) ? LIGHTGRAY : GRAY);
        DrawText("Run DFS", btnDFS.x + 35, btnDFS.y + 10, 20, BLACK);

        DrawText(("Mode: " + currentMode).c_str(), 500, 40, 20, highlightColor);

        DrawGraphAnimated(Adir, {500.0f, 350.0f}, 250.0f, currentPath, currentStep, highlightColor);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}