#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>

#define TEMP_BYTES 100000   // bytes a asignar para "forzar" uso de memoria (100 KB)
#define TIME_REPS 50000 

// funcion para medir tiempo
unsigned long long fact_iter_simple(int n) {
    unsigned long long res = 1;
    for (int i = 1; i <= n; i++) res *= i;
    return res;
}

unsigned long long fact_rec_simple(int n) {
    if (n <= 1) return 1;
    return (unsigned long long)n * fact_rec_simple(n - 1);
}

// Forzar el uso de la memoria
unsigned long long fact_iter_mem(int n) {
    char *buf = (char*)malloc(TEMP_BYTES);
    if (!buf) { fprintf(stderr, "malloc fallo\n"); exit(1); }
    unsigned long long r = fact_iter_simple(n);
    free(buf);
    return r;
}

unsigned long long fact_rec_mem(int n) {
    char *buf = (char*)malloc(TEMP_BYTES);
    if (!buf) { fprintf(stderr, "malloc fallo\n"); exit(1); }
    unsigned long long r;
    if (n <= 1) r = 1;
    else r = (unsigned long long)n * fact_rec_mem(n - 1);
    free(buf);
    return r;
}

long get_mem_kb() {
    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    return uso.ru_maxrss;
}

int main(void) {
    // lista de valores
    int valores[] = {5, 10, 15, 20};
    int nvals = sizeof(valores) / sizeof(valores[0]);

    FILE *csv = fopen("resultados.csv", "w");
    if (!csv) {
        fprintf(stderr, "No se pudo crear resultados.csv\n");
        return 1;
    }
    fprintf(csv, "n | factactorial iterativo | factorial recursivo | Tiempo iterativo (s) | Tiempo recursivo (s) | Memoria iterativa | Memoria recursiva\n");

    printf("Procesando %d valores...\n\n", nvals);
    for (int idx = 0; idx < nvals; idx++) {
        int n = valores[idx];

        //calcular factorials 
        unsigned long long fact_i = fact_iter_simple(n);
        unsigned long long fact_r = fact_rec_simple(n);

        //medir tiempo 
        clock_t t0 = clock();
        for (int k = 0; k < TIME_REPS; k++) fact_iter_simple(n);
        clock_t t1 = clock();
        double time_iter = (double)(t1 - t0) / CLOCKS_PER_SEC;

        t0 = clock();
        for (int k = 0; k < TIME_REPS; k++) fact_rec_simple(n);
        t1 = clock();
        double time_rec = (double)(t1 - t0) / CLOCKS_PER_SEC;

        //  memoria 
        long mem_before = get_mem_kb();
        fact_iter_mem(n);          
        long mem_after = get_mem_kb();
        long mem_iter = (mem_after > mem_before) ? (mem_after - mem_before) : 0;

        mem_before = get_mem_kb();
        fact_rec_mem(n);           
        mem_after = get_mem_kb();
        long mem_rec = (mem_after > mem_before) ? (mem_after - mem_before) : 0;

        //  CSV 
        fprintf(csv, "%d | %llu | %llu | %.6f | %.6f | %ld | %ld\n",
                n, fact_i, fact_r, time_iter, time_rec, mem_iter, mem_rec);


        printf("n = %d\n", n);
        printf("  Factorial iterativo = %llu\n", fact_i);
        printf("  Factorial recursivo = %llu\n", fact_r);
        printf("  Tiempo iterativo (x%d): %.6f s\n", TIME_REPS, time_iter);
        printf("  Tiempo recursivo (x%d): %.6f s\n", TIME_REPS, time_rec);
        printf("  Memoria iterativa (estimada): %ld KB\n", mem_iter);
        printf("  Memoria recursiva (estimada): %ld KB\n\n", mem_rec);
    }

    fclose(csv);

    
    printf("=== CONTENIDO resultados.csv ===\n");
    FILE *f = fopen("resultados.csv", "r");
    if (!f) {
        printf("No se pudo abrir resultados.csv\n");
        return 1;
    }
    char linea[512];
    while (fgets(linea, sizeof(linea), f)) {
        printf("%s", linea);
    }
    fclose(f);

    return 0;
}


