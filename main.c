#include <stdio.h>
#include <locale.h>

#define N 5

typedef struct {
    int time;
    int type;
    int count;
    float value;
} Incident;

float calc_q(float x_curr, float x_prev, float d) {
    return (x_curr - x_prev) / d;
}

int get_state(float q, float l) {
    if (q < -l - 1e-5f) return -1;
    if (q > l + 1e-5f) return 1; /*На всякий случай прибавляем погрешность*/
    return 0;
}

void print_state(int state) {
    if (state == -1) printf("СНИЖЕНИЕ\n");
    else if (state == 1) printf("РОСТ\n");
    else printf("НОРМА\n");
}

int main() {
    setlocale(LC_ALL, ".utf-8");

    int k = 2;
    float l = 0.8f;
    float d = 5.0f;

    /*Значения за 5 отсчетов по времени (t = 1...5)
     Для RPC на t=2 и t=4 взяты измерения, дающие максимальное отклонение*/
    float lat[N] = { 43.7f, 44.2f, 44.7f, 50.6f, 51.1f };
    float qry[N] = { 53.8f, 57.2f, 55.2f, 53.2f, 51.2f };
    float rpc[N] = { 32.0f, 31.3f, 30.6f, 37.5f, 34.5f };

    printf("КТ-1. Вариант 20\n");
    printf("Параметры: k = %d, L = %.1f, D = %.1f\n\n", k, l, d);

    for (int i = k; i < N; i++) {
        int t = i + 1;
        printf("--- Отсчет %d (пара с отсчетом %d) ---\n", t, t - k);

        float q_lat = calc_q(lat[i], lat[i - k], d);
        int st_lat = get_state(q_lat, l);
        printf("LAT: X[%d]=%.1f, X[%d]=%.1f -> Q = %.2f | ", t, lat[i], t - k, lat[i - k], q_lat);
        print_state(st_lat);

        float q_qry = calc_q(qry[i], qry[i - k], d);
        int st_qry = get_state(q_qry, l);
        printf("QRY: X[%d]=%.1f, X[%d]=%.1f -> Q = %.2f | ", t, qry[i], t - k, qry[i - k], q_qry);
        print_state(st_qry);

        float q_rpc = calc_q(rpc[i], rpc[i - k], d);
        int st_rpc = get_state(q_rpc, l);
        printf("RPC: X[%d]=%.1f, X[%d]=%.1f -> Q = %.2f | ", t, rpc[i], t - k, rpc[i - k], q_rpc);
        print_state(st_rpc);

        int is_incident = (st_lat != 0) && (st_qry != 0 || st_rpc != 0);

        if (is_incident) {
            Incident inc;
            inc.time = t;
            inc.type = 1;
            inc.count = (st_lat != 0) + (st_qry != 0) + (st_rpc != 0);
            inc.value = q_lat;

            printf("-> ИНЦИДЕНТ! time=%d, type=%d, count=%d, value=%.2f\n",
                inc.time, inc.type, inc.count, inc.value);
        }
        else printf("-> Инцидентов нет\n");

        printf("\n");
    }

    return 0;
}