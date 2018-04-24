#include <cstdio>
#include <cstdlib>

int is_stack(int n, int** data) {
    int stack[n];
    int top = 0;
    for (int i = 0;i < n;++i) {
        if (data[0][i] == 1) {
            stack[top] = data[1][i];
            ++top;
        }
        else {
            if (stack[top - 1] != data[1][i])
                return 0;
            --top;
        }
    }
    return 1;
}

int is_queue(int n, int** data) {
    int queue[n];
    int top = 0, back = 0;
    for (int i = 0;i < n;++i) {
        if (data[0][i] == 1) {
            queue[back] = data[1][i];
            ++back;
        }
        else {
            if (queue[top] != data[1][i])
                return 0;
            ++top;
        }
    }
    return 1;
}

int main() {
    int n;
    scanf("%d", &n);
    int** data = (int**)malloc(sizeof(int*) * n);
    for (int i = 0;i < n;++i)
        data[i] = (int*)malloc(sizeof(int) * 2);
    for (int i = 0;i < n;++i)
        scanf("%d%d", &data[0][i], &data[1][i]);
    int ans = 0;
    if (is_stack(n, data)) 
        ans += 1;
    if (is_queue(n, data))
        ans += 2;
    if (ans == 0)
        printf("neither");
    else if (ans == 1)
        printf("stack");
    else if (ans == 2)
        printf("queue");
    else
        printf("both");
    return 0;
}
