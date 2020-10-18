#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <vector>
#define THREADS_NUM 4
#define ELEMENTS_NUM 50000000


void fill_vector(int el_num, std::vector<int> &v) {
	int min_el = 1;
	int max_el = 300;
	srand(time(nullptr));

	for (int i = 0; i < el_num; i++) {
		v.push_back(rand() % (max_el - min_el + 1) + min_el);
	}
}


int main(int argc, char* argv[]) {
    omp_set_num_threads(THREADS_NUM);
    
    std::vector<int> vect;
    fill_vector(ELEMENTS_NUM, vect);

    int total = 0;
    
    unsigned t0;
    t0 = clock();

    for (int i = 0; i < ELEMENTS_NUM; i += 2) {
    	total += vect[i];
    }
    for (int i = 1; i < ELEMENTS_NUM; i += 2) {
    	total -= vect[i];
    }
    std::cout << "Result: " << total << std::endl;
    std::cout << "Normal Execution time: " << clock() - t0 << std::endl;


    total = 0;
    t0 = clock();
    int i;
    #pragma omp parallel for private(i) shared(vect) reduction(+:total) 
        for (int i = 0; i < ELEMENTS_NUM; i += 2) {
        	total += vect[i];
        }
        for (int i = 1; i < ELEMENTS_NUM; i += 2) {
        	total -= vect[i];
        }
    unsigned exec_t = clock() - t0;
    std::cout << "Result: " << total << std::endl;
    std::cout << "Parallel Execution time: " << exec_t << std::endl;
    return 0;
}