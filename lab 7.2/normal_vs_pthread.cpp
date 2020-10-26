#include <omp.h>
#include <iostream>
#include <pthread.h>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <vector>
#include <iterator>
#define POWER 5
#define NUM 10
#define THREADS_NUM 4
std::vector<std::vector<int>> result = {{}};
int *final_res;



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

void* func(void *param) 
{
    int N = result.size();
    int thread_share = (int)floor(N/THREADS_NUM);
    for (int i = ((intptr_t)param)*thread_share; i < (((intptr_t)param) + 1)*thread_share; i++) {
        for (int j = 0; j < NUM; j++)
        {
            final_res[i] *= result[i][j];
        }
    }
    if (((intptr_t)param == 3)&&(thread_share*THREADS_NUM < N)) {
        for (int i = thread_share*THREADS_NUM; i < N; i++) {
            for (int j = 0; j < NUM; j++)
        {
            final_res[i] *= result[i][j];
        }
        }
    }
}


int main(int argc, char const *argv[])
{
    std::vector<std::vector<int>> vect;
    fill_vect(vect);
    
    
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
    final_res = new int[numb];
    
    int *final_ress = new int[numb];
    
    for (int i = 0; i < numb; i++) {
        final_res[i] = 1;
        final_ress[i] = 1;
    }
    
    unsigned t0;
    t0 = clock();
    //подсчёт последовательный
    for (int i = 0; i < numb; i++) {
        for (int j = 0; j < NUM; j++)
        {
            final_ress[i] *= result[i][j];
        }
    }
    std::cout << "Normal calculation: " << clock() - t0 << std::endl;
    
    t0 = clock();
    //подсчёт параллельный pthread
    pthread_t mythread1, mythread2, mythread3;
    pthread_create(&mythread1, NULL, func, (void *)1);
    pthread_create(&mythread2, NULL, func, (void *)2);
    pthread_create(&mythread3, NULL, func, (void *)3);
    func((void *)0);
    
    pthread_join(mythread1, NULL);
    pthread_join(mythread2, NULL);
    pthread_join(mythread3, NULL);
    std::cout << "Parallel PTHREAD calculation: " << clock() - t0 << std::endl;
    
    
    
	return 0;
}
