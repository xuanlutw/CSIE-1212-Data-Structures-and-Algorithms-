#include <cstdio>
#include <cstdlib>
#include <cassert>

#define HASH_MAX 666649
#define MASK(x) ((x) & (((long)1 << 55) - 1))
#define RESULT(x) (((x) >> 55) & 3) 
#define ROUND(x) (((x) >> 50) & 31) 
#define GET(x, i, j) (((x) >> (((i) * 5 + (j)) * 2)) & 3)
#define GET2(x, i, j) ((GET((x), (i), (j)) == 2)? 0: GET((x), (i), (j)))     // X -> 0
#define COLOR(x) (((ROUND(x) >> 1) & 1) + 1)

#define UP_ROUND(x) ((x) + ((long)2 << 50))
#define SET(x, i, j, c) ((x) + ((c) << (((i) * 5 + (j)) * 2)))
#define SET_RESULT(x, r) ((x) | ((long)(r) << 55))
#define SET_ROUND(x, r) ((x) | ((long)(r) << 50))

#define HASH(x) (MASK(x) % HASH_MAX)
/*
   O 1
   X 2
*/
/*
   Draw 0
   O 1
   X 2
*/
/*
   56-55    54-50   49-0
   result   round   table
*/

typedef struct record{
    u_long table;
    struct record* next;
} Record;

Record* hash_table[HASH_MAX];

void put_table(const u_long table);
/*
u_long rotate_table(u_long table) {
    u_long tmp[7] = {0, 0, 0, 0, 0, 0, 0}, max = MASK(table);
    for (int i = 0;i < 5;++i) {
        for (int j = 0;j < 5;++j) {
            tmp[0] = (tmp[0] << 2) + GET(table, i, 4 - j);
            tmp[1] = (tmp[1] << 2) + GET(table, 4 - i, j);
            tmp[2] = (tmp[2] << 2) + GET(table, 4 - i, 4 - j);
            tmp[3] = (tmp[3] << 2) + GET(table, j, i);
            tmp[4] = (tmp[4] << 2) + GET(table, j, 4 - i);
            tmp[5] = (tmp[5] << 2) + GET(table, 4 - j, i);
            tmp[6] = (tmp[6] << 2) + GET(table, 4 - j, 4 - i);
        }
    }
    for (int i = 0;i < 7;++i)
        if (tmp[i] < max) max = tmp[i];
    return SET_ROUND(max, ROUND(table));
}*/

void hash_insert(u_long table, int result) {
    //table = rotate_table(table);
    Record* rec = (Record*)malloc(sizeof(Record));
    rec->next = NULL;
    rec->table = SET_RESULT(table, result);
    int key = HASH(table);
    Record* tmp = hash_table[key];
    if (!tmp) {
        hash_table[key] = rec;
        return;
    }
    while (tmp->next) tmp = tmp->next;
    tmp->next = rec;
    return;
}

inline int hash_search(u_long table) {
    //table = rotate_table(table);
    for (Record* tmp = hash_table[HASH(table)];tmp;tmp = tmp->next) 
        if (MASK(table) == MASK(tmp->table))
            return RESULT(tmp->table);
    return -1;
}

char valid_table(u_long table) {
    assert(ROUND(table) == 22);
    int count = 0, sum;
    for (int i = 0;i < 5;++i) {
        sum = 0;
        for (int j = 0;j < 5;++j) sum += GET2(table, i, j);
        if (sum >= 4 || sum <= 1) count += (sum >= 4? 1: -1); 
        sum = 0;
        for (int j = 0;j < 5;++j) sum += GET2(table, j, i);
        if (sum >= 4 || sum <= 1) count += (sum >= 4? 1: -1); 
    }
    sum = 0;
    for (int i = 0;i < 5;++i) sum += GET2(table, i, i);
    if (sum >= 4 || sum <= 1) count += (sum >= 4? 1: -1); 
    sum = 0;
    for (int i = 0;i < 5;++i) sum += GET2(table, i, 4 - i);
    if (sum >= 4 || sum <= 1) count += (sum >= 4? 1: -1); 
    
    if (count > 0) return 1;
    if (count < 0) return 2;
    return 0;
}

/*
   ret value
   0 unknowm
   1 O win
   2 X win
   */
int expected_table(u_long table) {
    int real[2] = {0, 0}, max[2] = {0, 0}, sum[3];
    for (int i = 0;i < 5;++i) {
        sum[0] = sum[1] = sum[2] = 0;
        for (int j = 0;j < 5;++j) ++sum[GET(table, i, j)];
        if (sum[1] >= 4) ++real[0];  
        if (sum[2] >= 4) ++real[1];  
        if (sum[1] + sum[0] >= 4) ++max[0];  
        if (sum[2] + sum[0] >= 4) ++max[1];  
        sum[0] = sum[1] = sum[2] = 0;
        for (int j = 0;j < 5;++j) ++sum[GET(table, j, i)];
        if (sum[1] >= 4) ++real[0];  
        if (sum[2] >= 4) ++real[1];  
        if (sum[1] + sum[0] >= 4) ++max[0];  
        if (sum[2] + sum[0] >= 4) ++max[1];  
    }
    sum[0] = sum[1] = sum[2] = 0;
    for (int i = 0;i < 5;++i) ++sum[GET(table, i, i)];
    if (sum[1] >= 4) ++real[0];  
    if (sum[2] >= 4) ++real[1];  
    if (sum[1] + sum[0] >= 4) ++max[0];  
    if (sum[2] + sum[0] >= 4) ++max[1];  
    sum[0] = sum[1] = sum[2] = 0;
    for (int i = 0;i < 5;++i) ++sum[GET(table, i, 4 - i)];
    if (sum[1] >= 4) ++real[0];  
    if (sum[2] >= 4) ++real[1];  
    if (sum[1] + sum[0] >= 4) ++max[0];  
    if (sum[2] + sum[0] >= 4) ++max[1];

    if (real[0] > max[1]) return 1;
    if (real[1] > max[0]) return 2;
    return 0;
}
/*
   if set on, high order has draw sol. that means draw and best sol are the same
 */
char value_table(u_long table, int set) {
    int pre = hash_search(table) ;
    if (pre != -1) return pre;
    if (ROUND(table) == 22) return valid_table(table);
    int state = 0;
    int result = 0;//expected_table(table);
    /*
       0 opposite color
       1 draw
       2 same color
    */
    if (!result) {
        for (int i = 0;i < 25;++i) {
            if (GET(table, i / 5, i % 5)) continue;
            u_long tmp = SET(table, i / 5, i % 5, COLOR(table));
            for (int j = 24;j > i;--j) {
                if (GET(tmp, j / 5, j % 5)) continue;
                u_long next = UP_ROUND(SET(tmp, j / 5, j % 5, COLOR(tmp)));
                int next_result = value_table(next, state);
                if (COLOR(table) == next_result) {
                    state = 2;
                    break;
                }
                if (!next_result && !state) {
                    state = 1;
                    if (set) break;
                }
            }
        }
        if (state == 2) result = COLOR(table);
        else if (state == 1) result = 0;
        else result = COLOR(table)? 2: 1;
    }
    hash_insert(table, result);
    return result;
}

u_long get_table() {
    u_long ans = 0;
    u_long num = 0;
    for (int i = 0;i < 5;++i) {
        for (int j = 0;j < 5;++j) {
            char c = getchar();
            ans <<= 2;
            if (c == 'O') ans += 1;
            if (c == 'X') ans += 2;
            if (c != '.') ++num;
        }
        getchar();
    }
    ans += (num << 50);
    return ans;
}

void put_table(const u_long table) {
    for (int i = 0;i < 5;++i) {
        for (int j = 0;j < 5;++j) {
            if (GET(table, i, j) == 1) putchar('O');
            else if (GET(table, i, j) == 2) putchar('X');
            else putchar('.');
        }
        printf("\n");
    }
    printf("%lu\n", ROUND(table));
    return;
}
    
int main() {
    int n;
    scanf("%d\n", &n);
    for (int num = 0;num < n;++num) {
        u_long table = get_table();
        char result = value_table(table, 0);
        if (result == 0) printf("Draw\n");
        if (result == 1) printf("O win\n");
        if (result == 2) printf("X win\n");
        //put_table(table);
        for (int i = 0;i < HASH_MAX;++i) 
            for (Record* tmp = hash_table[i];tmp;tmp = tmp->next, free(tmp)) ;
    }
    /* test hash
    int sum = 0, max = 0;
    for (int i = 0;i < HASH_MAX;++i) {
        int sub_sum = 0;
        Record* tmp = hash_table[i];
        while (tmp) {
            ++sub_sum;
            tmp = tmp->next;
        }
        sum += sub_sum;
        if (sub_sum > max) max = sub_sum;
    }
    printf("sum %d\n", sum);
    printf("max %d\n", max);
    */
    return 0;
}
