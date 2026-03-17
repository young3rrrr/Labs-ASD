#include <iostream>
#include <utility>
#include <cmath>

double hyperbolic_cosine_descent(int i, int n, double x, double curr_F, double curr_sum) {

    curr_sum += curr_F;

    if (i == n) {
        return curr_sum; 
    } 
    else {

        double next_F = curr_F * ((x * x) / (4.0 * i * i - 2.0 * i));
        
        return hyperbolic_cosine_descent(i + 1, n, x, next_F, curr_sum);
    }
}


std::pair<double, double> hyperbolic_cosine_return(int i, double x){
    if (i == 1){
        return {1.0, 1.0};
    }

    std::pair<double, double> prev = hyperbolic_cosine_return(i - 1, x);
    double prev_F = prev.first;
    double prev_sum = prev.second;
    

    double curr_i = i - 1.0;

    double curr_F = prev_F * ((x * x) / (4.0 * curr_i * curr_i - 2.0 * curr_i));
    double curr_sum = prev_sum + curr_F;

    return {curr_F, curr_sum};
}

double hyperbolic_cosine_mixed(int i, int n, double x, double curr_F) {
    if (i == n) {
        return curr_F; 
    }

    double current_i = i;
    double next_F = curr_F * ((x * x) / (4.0 * current_i * current_i - 2.0 * current_i));

    double sum_of_rest = hyperbolic_cosine_mixed(i + 1, n, x, next_F);

    return curr_F + sum_of_rest;
}

int main() {

    int n;

    double x;

    std::cout << "Enter n: ";

    std::cin >> n;

    std::cout << "Enter x: ";

    std::cin >> x;

    double result1 = hyperbolic_cosine_descent(1, n, x, 1.0, 0.0);
    std::cout << "Result of hyperbolic_cosine_descent: " << result1 << std::endl;
    double result2 = hyperbolic_cosine_mixed(1, n, x, 1.0);
    std::cout << "Result of hyperbolic_cosine_mixed: " << result2 << std::endl;
    std::pair<double, double> result3 = hyperbolic_cosine_return(n, x);
    std::cout << "Result of hyperbolic_cosine_return: " << result3.second << std::endl;
    return 0;
}
