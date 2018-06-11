#include <cstdio>
#include <cfloat>
#include <cstring>
#define MAX 25

void pre_proc(int n, int* coord, int* pre) {
    for (int i = 0;i < n;++i) {
        pre[i * MAX + i] = (1 << i);
        for (int j = i + 1;j < n;++j) {
            int ans = ((1 << i) | (1 << j));
#define x1 (double)coord[2 * i]
#define y1 (double)coord[2 * i + 1]
#define x2 (double)coord[2 * j]
#define y2 (double)coord[2 * j + 1]
#define x3 (double)coord[2 * k]
#define y3 (double)coord[2 * k + 1]
            if (x1 == x2) continue;
            double a = ((x2 * y1) - (x1 * y2)) / ((x1 - x2) * x1 * x2);
            double b = (-(x2 * x2 * y1) + (x1 * x1 * y2)) / ((x1 - x2) * x1 * x2);
            if (a > -DBL_EPSILON) continue;
            for (int k = 0;k < n;++k) {
                if (k == i) continue;
                if (k == j) continue;
                double del = y3 - a * x3 * x3 - b * x3;
                //printf("<<%lf %lf %lf %d\n", del, x2, (double)coord[2 * i], coord[2 * i]);
                if (del < 0) del = -del;
                if (del < DBL_EPSILON) ans |= (1 << k);
            }
            pre[i * MAX + j] = ans;
            pre[j * MAX + i] = ans;
        }
    }
    return;
}

int dp(int n, const int state, int* pre, int* dp_arr) {
    if (!state) return 0;
    if (dp_arr[state]) return dp_arr[state];
    //printf("%d %d\n", state, dp_arr[state]);
    for (int i = 0;;++i) {
        if (!(state & (1 << i))) continue;
        int min = MAX * MAX;
        int pstate;
        for (int j = 0;j < n;++j) {
            if (!pre[i * MAX + j]) continue;
            int tmp = dp(n, state & (~pre[i * MAX + j]), pre, dp_arr);
            if (tmp < min) {
                min = tmp;
                pstate = state & (~pre[i*MAX +j]);
            }
        }
        dp_arr[state] = min + 1;
        //printf("%d %d %d\n", state, pstate, min);
        break;
    }
    return dp_arr[state];
}
            
int dp_arr[1 << MAX];

int main() {
    int t;
    scanf("%d", &t);
    while(t--) {
        int n;
        int coord[2 * MAX] = {0}; 
        int pre[MAX * MAX] = {0};
        memset(dp_arr, 0, sizeof(int) * (1 << MAX));
        scanf("%d", &n);
        for (int i = 0;i < n;++i)
            scanf("%d%d", &coord[2 * i], &coord[2 * i + 1]);
        pre_proc(n, coord, pre);
        /*
        for (int i = 0;i < n;++i)
            for (int j = 0;j < n;++j)
                printf("%d %d %p\n", i, j, pre[i * MAX + j]);
        */
        printf("%d\n", dp(n, (1 << n) - 1, pre, dp_arr));
        //for (int i = 0;i < 1 << n;++i)
            //printf ("%d\n", dp_arr[i]);
    }
    return 0;
}
