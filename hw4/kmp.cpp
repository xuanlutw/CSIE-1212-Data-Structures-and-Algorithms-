#include <cstdio>
#include <cstdlib>
#include <cstring>
#define NUM_T 10010
 
/* list_string */
#define NUM_DATA 128
#define AT_FRONT 0
#define AT_BACK 1
typedef struct ls_item {
    char data[NUM_DATA];
    struct ls_item* next;
    struct ls_item* pre;
    short begin;
    short end;
} ls_item;
 
typedef struct ls {
    ls_item* head;
    ls_item* tail;
} ls;
 
typedef struct ls_iterator {
    ls_item* now_item;
    short now_num;
} ls_iterator;
 
ls_item* ls_item_init(int where) {
    ls_item* ret = (ls_item*)malloc(sizeof(ls_item));
    if (where == AT_FRONT) {
        ret->begin = NUM_DATA;
        ret->end = NUM_DATA;
    }
    else if (where == AT_BACK) {
        ret->begin = 0;
        ret->end = 0;
    }
    ret->next = NULL;
    ret->pre = NULL;
    return ret;
}
 
ls* ls_init() {
    ls* ret = (ls*)malloc(sizeof(ls));
    ret-> tail = ret->head = ls_item_init(AT_FRONT);
    return ret;
}
 
void ls_destroy(ls* src) {
    ls_item* tmp;
    while (src->head) {
        tmp = src->head->next;
        free(src->head);
        src->head = tmp;
    }
    free(src);
    return;
}
 
void ls_push_back(ls* src, char c) {
    ls_item* tar = src->tail;
    if (tar->end != NUM_DATA) {
        tar->data[tar->end] = c;
        ++tar->end;
    }
    else {
        tar->next = ls_item_init(AT_BACK);
        tar->next->pre = tar;
        tar->next->data[0] = c;
        ++tar->next->end;
        src->tail = tar->next;
    }
    return;
}
 
void ls_push_front(ls* src, char c) {
    ls_item* tar = src->head;
    if (tar->begin != 0) {
        --tar->begin;
        tar->data[tar->begin] = c;
    }
    else {
        tar->pre = ls_item_init(AT_FRONT);
        tar->pre->next = tar;
        tar->pre->data[NUM_DATA - 1] = c;
        --tar->pre->begin;
        src->head = tar->pre;
    }
    return;
}
 
ls_iterator* ls_iterator_init(ls* src) {
    ls_iterator* ret = (ls_iterator*)malloc(sizeof(ls_iterator));
    ret->now_item = src->head;
    ret->now_num = src->head->begin;
    return ret;
}
 
inline void ls_iterator_destroy(ls_iterator* iter) {
    free(iter);
}
 
char ls_vist(ls_iterator* src) {
    if (src->now_num != src->now_item->end)
        return src->now_item->data[src->now_num++];
    if (src->now_item->next == NULL)
        return 0;
    src->now_item = src->now_item->next;
    src->now_num = src->now_item->begin;
    return ls_vist(src);
}
 
/*kmp*/
int* kmp_pre(char* t, int t_len) {
    int* pre = (int*)malloc(sizeof(int) * (strlen(t) + 1));
    pre[0] = -1;
    int j = -1;
    for (int i = 1;i <= t_len;++i) {
        while (j > -1 && t[j + 1] != t[i])
            j = pre[j];
        if (t[j + 1] == t[i])
            ++j;
        pre[i] = j;
    }
    return pre;
}
 
int kmp(ls* str, char* t) {
    int t_len = strlen(t);
    int* pre = kmp_pre(t, t_len);
    ls_iterator* iter = ls_iterator_init(str);
    int j = -1, ans = 0;
    for (char c_now = ls_vist(iter);c_now;c_now = ls_vist(iter)) {
        while (j > -1 && t[j + 1] != c_now)
            j = pre[j];
        if (t[j + 1] == c_now)
            ++j;
        if (j == t_len - 1) {
            ++ans;
            j = pre[j];
        }
    }
    ls_iterator_destroy(iter);
    free(pre);
    return ans;
}
 
int main() {
    char c, t[NUM_T];
    ls* str = ls_init();
    int q, op;
    while (scanf("%c", &c) && (c != '\n'))
        ls_push_back(str, c);
    scanf("%d", &q);
    while (q--) {
        scanf("%d", &op);
        if (op == 1) {
            scanf("%*c%c", &c);
            ls_push_front(str, c);
        }
        if (op == 2) {
            scanf("%*c%c", &c);
            ls_push_back(str, c);
        }
        if (op == 3){
            scanf("%s", t);
            printf("%d\n", kmp(str, t));
        }
    }
    ls_destroy(str);
    return 0;
    /*
    ls_iterator* iter = ls_iterator_init(str);
    while (1) {
        char tmp = ls_vist(iter);
        if (!tmp)
            break;
        printf("%c", tmp);
    }
    printf("\n");
    */
}

