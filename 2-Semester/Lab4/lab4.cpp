#include <raylib.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ГЕНЕРУВАННЯ МАТРИЦІ
vector<vector<double>> randm(int n) {
    vector<vector<double>> T(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            T[i][j] = (2.0 * rand()) / (RAND_MAX + 1.0);
        }
    }
    return T;
}

// МНОЖЕННЯ ТА ОКРУГЛЕННЯ
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

// МАТРИЦЯ НЕНАПРЯМОГО ГРАФА
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

// МАЛЮВАННЯ ГРАФА
void DrawGraph(const vector<vector<int>>& matrix, Vector2 center, float radius, bool isDirected, Color graphColor) {
    int n = matrix.size();
    vector<Vector2> pos(n);
    float nodeRadius = 20.0f;

    
    
    pos[0].x = center.x;
    pos[0].y = center.y;
    
    
    for (int i = 1; i < n; i++) {
        
        float angle = (i - 1) * (2 * PI / (n - 1));
        pos[i].x = center.x + radius * cos(angle);
        pos[i].y = center.y + radius * sin(angle);
    }
    

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                if (i == j) { 
                    DrawCircleLines(pos[i].x, pos[i].y - nodeRadius - 5, 12, graphColor);
                    continue; 
                }

                Vector2 p1 = pos[i];
                Vector2 p2 = pos[j];
                float dx = p2.x - p1.x;
                float dy = p2.y - p1.y;
                float length = sqrt(dx * dx + dy * dy);
                
                if (length > 0) { dx /= length; dy /= length; }

                if (isDirected) {
                    float arrowOffset = nodeRadius + 3.0f; 
                    Vector2 startP = { p1.x + dx * nodeRadius, p1.y + dy * nodeRadius };
                    Vector2 endP = { p2.x - dx * arrowOffset, p2.y - dy * arrowOffset };

                    DrawLineEx(startP, endP, 2.0f, graphColor);

                    float arrowSize = 15.0f;
                    Vector2 arrowP1 = { endP.x - dx * arrowSize - dy * arrowSize * 0.4f, 
                                        endP.y - dy * arrowSize + dx * arrowSize * 0.4f };
                    Vector2 arrowP2 = { endP.x - dx * arrowSize + dy * arrowSize * 0.4f, 
                                        endP.y - dy * arrowSize - dx * arrowSize * 0.4f };
                    
                    DrawTriangle(endP, arrowP1, arrowP2, graphColor);
                    DrawTriangle(endP, arrowP2, arrowP1, graphColor);
                } else {
                    if (i < j) { 
                        Vector2 startP = { p1.x + dx * nodeRadius, p1.y + dy * nodeRadius };
                        Vector2 endP = { p2.x - dx * nodeRadius, p2.y - dy * nodeRadius };
                        DrawLineEx(startP, endP, 2.0f, graphColor);
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        DrawCircleV(pos[i], nodeRadius, RAYWHITE);
        DrawCircleLines(pos[i].x, pos[i].y, nodeRadius, BLACK);
        string text = to_string(i);
        DrawText(text.c_str(), pos[i].x - MeasureText(text.c_str(), 20) / 2, pos[i].y - 10, 20, BLACK);
    }
}

// МАТРИЦЯ ЗАПИСУ
void writeMatrixToFile(ofstream& file, const string& title, const vector<vector<int>>& matrix) {
    file << "=== " << title << " ===\n";
    for (const auto& row : matrix) {
        for (int val : row) {
            file << val << " ";
        }
        file << "\n";
    }
    file << "\n";
}

// ХАРАКТЕРИСТИКИ ВЕРШИН
void calculateAndWriteStats(ofstream& file, const vector<vector<int>>& Adir, const vector<vector<int>>& Aundir) {
    int n = Adir.size();
    vector<int> inDegree(n, 0), outDegree(n, 0), dirDegree(n, 0), undirDegree(n, 0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (Adir[i][j] == 1) outDegree[i]++;
            if (Adir[j][i] == 1) inDegree[i]++; 
            if (Aundir[i][j] == 1) undirDegree[i]++;
        }
        dirDegree[i] = inDegree[i] + outDegree[i]; 
    }

    file << "=== ХАРАКТЕРИСТИКИ ВЕРШИН ===\n";
    file << "Вершина | Напiвстепiнь виходу | Напiвстепiнь заходу | Степiнь (Напрям.) | Степiнь (Ненапрям.)\n";
    file << "-----------------------------------------------------------------------------------------\n";
    for (int i = 0; i < n; i++) {
        file << "   " << i << "\t|\t" << outDegree[i] << "\t    |\t" << inDegree[i] 
             << "\t\t|\t" << dirDegree[i] << "\t    |\t" << undirDegree[i] << "\n";
    }
    file << "\n";

    bool isRegular = true;
    int firstDegree = undirDegree[0];
    for (int i = 1; i < n; i++) {
        if (undirDegree[i] != firstDegree) {
            isRegular = false;
            break;
        }
    }
    file << "Однорiднiсть графа (ненапрям.): " << (isRegular ? "ТАК (Степiнь: " + to_string(firstDegree) + ")" : "НІ") << "\n\n";

    vector<int> isolated, pendant;
    for (int i = 0; i < n; i++) {
        if (undirDegree[i] == 0) isolated.push_back(i);
        if (undirDegree[i] == 1) pendant.push_back(i);
    }

    file << "Iзольованi вершини (степiнь 0): ";
    if (isolated.empty()) file << "Немає";
    else for (int v : isolated) file << v << " ";
    file << "\n";

    file << "Висячi вершини (степiнь 1): ";
    if (pendant.empty()) file << "Немає";
    else for (int v : pendant) file << v << " ";
    file << "\n\n";
}

// МНОЖЕННЯ МАТРИЦЬ
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// ПОШУК ШЛЯХІВ
void findAndWritePaths(ofstream& file, const vector<vector<int>>& Adir, const vector<vector<int>>& A_power, int length) {
    int n = Adir.size();
    file << "=== ШЛЯХИ ДОВЖИНОЮ " << length << " ===\n";
    
    int totalPaths = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            
            if (A_power[i][j] > 0) {
                
                if (length == 2) {
                    
                    for (int v = 0; v < n; v++) {
                        if (Adir[i][v] == 1 && Adir[v][j] == 1) {
                            file << i << " -> " << v << " -> " << j << "\n";
                            totalPaths++;
                        }
                    }
                } 
                else if (length == 3) {
                    
                    for (int v1 = 0; v1 < n; v1++) {
                        if (Adir[i][v1] == 1) {
                            for (int v2 = 0; v2 < n; v2++) {
                                if (Adir[v1][v2] == 1 && Adir[v2][j] == 1) {
                                    file << i << " -> " << v1 << " -> " << v2 << " -> " << j << "\n";
                                    totalPaths++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    if (totalPaths == 0) {
        file << "Шляхiв такої довжини не знайдено.\n";
    } else {
        file << "Загальна кiлькiсть шляхiв: " << totalPaths << "\n";
    }
    file << "\n";
}

// МАТРИЦЯ ДОСЯЖНОСТІ
vector<vector<int>> getReachabilityMatrix(const vector<vector<int>>& A) {
    int n = A.size();
    vector<vector<int>> R = A;
    
    
    for (int i = 0; i < n; i++) R[i][i] = 1;

    
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                R[i][j] = R[i][j] || (R[i][k] && R[k][j]);
            }
        }
    }
    return R;
}

// МАТРИЦЯ СИЛЬНОЇ ЗВ'ЯЗНОСТІ
vector<vector<int>> getStrongConnectivityMatrix(const vector<vector<int>>& R) {
    int n = R.size();
    vector<vector<int>> S(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            S[i][j] = R[i][j] && R[j][i];
        }
    }
    return S;
}

// КОМПОНЕНТИ ЗВ'ЯЗНОСТІ
vector<vector<int>> findSCCs(const vector<vector<int>>& S) {
    int n = S.size();
    vector<bool> visited(n, false);
    vector<vector<int>> components;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            vector<int> comp;
            for (int j = 0; j < n; j++) {
                if (S[i][j] == 1) {
                    comp.push_back(j);
                    visited[j] = true;
                }
            }
            components.push_back(comp);
        }
    }
    return components;
}

// ГРАФ КОНДЕНСАЦІЇ
vector<vector<int>> getCondensationGraph(const vector<vector<int>>& Adir, const vector<vector<int>>& sccs) {
    int num_scc = sccs.size();
    vector<vector<int>> condGraph(num_scc, vector<int>(num_scc, 0));

    
    vector<int> vertex_to_scc(Adir.size());
    for (int c = 0; c < num_scc; c++) {
        for (int v : sccs[c]) {
            vertex_to_scc[v] = c;
        }
    }

    
    for (int i = 0; i < Adir.size(); i++) {
        for (int j = 0; j < Adir.size(); j++) {
            if (Adir[i][j] == 1) {
                int scc_i = vertex_to_scc[i];
                int scc_j = vertex_to_scc[j];
                
                if (scc_i != scc_j) {
                    condGraph[scc_i][scc_j] = 1;
                }
            }
        }
    }
    return condGraph;
}

// g++ lab4.cpp -o lab4 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

int main() {
    
    srand(5426); 
    int n = 12;  
    double k = 0.62; 

    vector<vector<double>> T = randm(n);
    vector<vector<int>> Adir = mulmr(T, k);
    vector<vector<int>> Aundir = makeUndirected(Adir);

    
    double k2 = 1.0 - 2*0.005 - 6*0.005 - 0.27; 
    
    vector<vector<int>> Adir2 = mulmr(T, k2);

    ofstream outFile("lab4_results.txt");
    if (outFile.is_open()) {
        outFile << "ЛАБОРАТОРНА РОБОТА 4 (Варіант 26)\n";
        outFile << "=======================================\n\n";
        
        writeMatrixToFile(outFile, "Матриця напрямленого графа (Adir, k=0.62)", Adir);
        writeMatrixToFile(outFile, "Матриця ненапрямленого графа (Aundir, k=0.62)", Aundir);
        
        calculateAndWriteStats(outFile, Adir, Aundir);

        
        outFile << "\n=======================================\n";
        outFile << "ЧАСТИНА 2: НОВИЙ КОЕФІЦІЄНТ ТА ШЛЯХИ\n";
        outFile << "=======================================\n\n";

        writeMatrixToFile(outFile, "НОВА Матриця напрямленого графа (Adir2, k=0.69)", Adir2);

        
        int n_size = Adir2.size();
        outFile << "--- Пiвстепенi нового орграфа ---\n";
        for (int i = 0; i < n_size; i++) {
            int inD = 0, outD = 0;
            for (int j = 0; j < n_size; j++) {
                if (Adir2[i][j] == 1) outD++;
                if (Adir2[j][i] == 1) inD++;
            }
            outFile << "В" << i << ": Захiд = " << inD << ", Вихiд = " << outD << "\n";
        }
        outFile << "\n";

        
        vector<vector<int>> A2 = multiplyMatrices(Adir2, Adir2);
        vector<vector<int>> A3 = multiplyMatrices(A2, Adir2);

        writeMatrixToFile(outFile, "Матриця A^2 (шляхи довжиною 2)", A2);
        findAndWritePaths(outFile, Adir2, A2, 2);

        writeMatrixToFile(outFile, "Матриця A^3 (шляхи довжиною 3)", A3);
        findAndWritePaths(outFile, Adir2, A3, 3);
        
        
        outFile << "=======================================\n";
        outFile << "ЧАСТИНА 3: ЗВ'ЯЗНІСТЬ ТА КОНДЕНСАЦІЯ\n";
        outFile << "=======================================\n\n";

        vector<vector<int>> ReachMat = getReachabilityMatrix(Adir2);
        writeMatrixToFile(outFile, "Матриця досяжностi (Алгоритм Уоршалла)", ReachMat);

        vector<vector<int>> StrongConnMat = getStrongConnectivityMatrix(ReachMat);
        writeMatrixToFile(outFile, "Матриця сильної зв'язностi", StrongConnMat);

        vector<vector<int>> SCCs = findSCCs(StrongConnMat);
        outFile << "=== Компоненти сильної зв'язностi (КСС) ===\n";
        for (size_t c = 0; c < SCCs.size(); c++) {
            outFile << "КСС " << c << ": { ";
            for (int v : SCCs[c]) {
                outFile << v << " ";
            }
            outFile << "}\n";
        }
        outFile << "\n";

        vector<vector<int>> CondensationMat = getCondensationGraph(Adir2, SCCs);
        writeMatrixToFile(outFile, "Матриця сумiжностi графа конденсацiї", CondensationMat);
        
        outFile.close();
        cout << "[УСПІХ] Результати успішно записані у файл 'lab4_results.txt'!\n";
    }

    
    const int screenWidth = 1500; 
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Lab 4:(Variant 26)");
    SetTargetFPS(60);

    
    vector<vector<int>> CondensationMat = getCondensationGraph(Adir2, findSCCs(getStrongConnectivityMatrix(getReachabilityMatrix(Adir2))));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        
        DrawText("Directed graph (k=0.69)", 150, 30, 20, BLACK);
        DrawGraph(Adir2, {250.0f, 320.0f}, 180.0f, true, RED);

        
        DrawText("Undirected graph", 650, 30, 20, BLACK);
        DrawGraph(Aundir, {750.0f, 320.0f}, 180.0f, false, DARKBLUE);

        
        DrawText("Condensation Graph", 1150, 30, 20, BLACK);
        
        DrawGraph(CondensationMat, {1250.0f, 320.0f}, 180.0f, true, DARKGREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}