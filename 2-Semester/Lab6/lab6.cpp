#include <raylib.h>
#include <vector>
#include <list>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;

struct Edge {
    int u, v, weight;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// DSU для перевірки циклів
struct DSU {
    vector<int> parent;

    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

// Генерація графа
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
            A[i][j] = (T[i][j] * k >= 1.0) ? 1 : 0;
        }
    }
    return A;
}

// Краскал
vector<Edge> getKruskalMST(vector<Edge> edges, int n) {
    vector<Edge> mst;

    sort(edges.begin(), edges.end());

    DSU dsu(n);

    for (Edge e : edges) {
        if (dsu.find(e.u) != dsu.find(e.v)) {
            dsu.unite(e.u, e.v);
            mst.push_back(e);
        }
    }

    return mst;
}

// Візуалізація
void DrawWeightedGraphAnimated(int n, const vector<Edge>& allEdges, const vector<Edge>& activeEdges, int currentStep, Vector2 center, float radius) {
    vector<Vector2> pos(n);
    float nodeRadius = 22.0f;

    for (int i = 0; i < n; i++) {
        float angle = i * (2 * PI / n);
        pos[i].x = center.x + radius * cos(angle);
        pos[i].y = center.y + radius * sin(angle);
    }

    for (const Edge& e : allEdges) {
        Vector2 p1 = pos[e.u];
        Vector2 p2 = pos[e.v];

        DrawLineEx(p1, p2, 1.0f, LIGHTGRAY);

        int midX = (p1.x + p2.x) / 2;
        int midY = (p1.y + p2.y) / 2;
        string wText = to_string(e.weight);

        DrawRectangle(midX - 10, midY - 10, MeasureText(wText.c_str(), 16) + 4, 20, Fade(RAYWHITE, 0.8f));
        DrawText(wText.c_str(), midX - 8, midY - 8, 16, DARKGRAY);
    }

    int totalWeight = 0;
    for (int k = 0; k < currentStep && k < activeEdges.size(); k++) {
        Edge e = activeEdges[k];
        Vector2 p1 = pos[e.u];
        Vector2 p2 = pos[e.v];

        DrawLineEx(p1, p2, 5.0f, LIME);

        int midX = (p1.x + p2.x) / 2;
        int midY = (p1.y + p2.y) / 2;
        string wText = to_string(e.weight);
        DrawText(wText.c_str(), midX - 8, midY - 8, 16, BLACK);

        totalWeight += e.weight;
    }

    for (int i = 0; i < n; i++) {
        DrawCircleV(pos[i], nodeRadius, RAYWHITE);
        DrawCircleLines(pos[i].x, pos[i].y, nodeRadius, BLACK);
        string text = to_string(i);
        DrawText(text.c_str(), pos[i].x - MeasureText(text.c_str(), 20) / 2, pos[i].y - 10, 20, BLACK);
    }

    DrawText(TextFormat("Total MST Weight: %d", totalWeight), 50, 50, 20, RED);
}
// g++ lab6.cpp -o lab6 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
int main() {
    srand(5426);
    int n = 12;  
    double k = 0.90;

    vector<vector<double>> T = randm(n);
    vector<vector<int>> Adir = mulmr(T, k);

    vector<list<pair<int, int>>> adjList(n);
    vector<Edge> allEdges;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (Adir[i][j] == 1 || Adir[j][i] == 1) {
                int weight = rand() % 200 + 1;

                adjList[i].push_back({j, weight});
                adjList[j].push_back({i, weight});

                allEdges.push_back({i, j, weight});
            }
        }
    }

    vector<Edge> mstEdges = getKruskalMST(allEdges, n);

    const int screenWidth = 1000;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Lab 6: Kruskal's MST (Variant 26)");
    SetTargetFPS(60);

    int currentStep = 0;
    float timer = 0.0f;
    float stepDelay = 0.4f;
    bool isRunning = false;

    Rectangle btnRun = { 400, 20, 200, 50 };

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, btnRun) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            isRunning = true;
            currentStep = 0;
            timer = 0.0f;
        }

        if (isRunning && currentStep < mstEdges.size()) {
            timer += GetFrameTime();
            if (timer >= stepDelay) {
                currentStep++;
                timer = 0.0f;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleRec(btnRun, CheckCollisionPointRec(mousePoint, btnRun) ? LIGHTGRAY : GRAY);
        DrawText("Run Kruskal MST", btnRun.x + 15, btnRun.y + 15, 20, BLACK);

        DrawWeightedGraphAnimated(n, allEdges, mstEdges, currentStep, {500.0f, 420.0f}, 300.0f);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}