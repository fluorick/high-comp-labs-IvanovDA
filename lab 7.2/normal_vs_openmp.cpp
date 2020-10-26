#include <omp.h>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <iterator>
#define POWER 5
#define NUM 9
#define THREADS_NUM 4

void fill_vect(std::vector<std::vector<int>> &vect) 
{
    int min_el = 1;
    int max_el = 10;
    srand(time(nullptr));
    
    for (int i = 0; i < NUM; i++) {
        std::vector<int> v;
        for (int j = 0; j < POWER; j++) {
            v.push_back(rand() % (max_el - min_el + 1) + min_el);
            std::cout << v[j] << " ";
        }
        std::cout << std::endl;
        vect.push_back(v);
	}
}

int main(int argc, char const *argv[])
{
	omp_set_num_threads(THREADS_NUM);
    std::vector<std::vector<int>> vect;
    fill_vect(vect);
    
    std::vector<std::vector<int>> result = {{}};
       for (const auto& u : vect) {
           std::vector<std::vector<int>> r;
           for (const auto& x : result) {
               for (const auto y : u) {
                   r.push_back(x);
                   r.back().push_back(y);
               }
           }
           result = std::move(r);
       }
    
    
    int numb = result.size();
    int *final_res = new int[numb];
    
    for (int i = 0; i < numb; i++) {
        final_res[i] = 1;
    }
    
    unsigned t0;
    t0 = clock();
    //подсчёт последовательный
    for (int i = 0; i < numb; i++) {
        for (int j = 0; j < NUM; j++)
        {
            final_res[i] *= result[i][j];
        }
    }
    std::cout << "Normal calculation: " << clock() - t0 << std::endl;
    
    int *final_res1 = new int[numb];
    
    for (int i = 0; i < numb; i++) {
        final_res1[i] = 1;
    }
    
    t0 = clock();
    int i, j;
    #pragma omp parallel for private(i, j) shared(result)
        for (i = 0; i < numb; i++) {
            for (j = 0; j < NUM; j++)
            {
                final_res1[i] *= result[i][j];
            }
        }
    std::cout << "Parallel OPENMP calculation: " << clock() - t0 << std::endl;
	return 0;
}
