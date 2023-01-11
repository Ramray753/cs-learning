#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#define TABLE_SIZE 64
#define BUFFER_SIZE 64

int hit_count, miss_count, eviction_count;
int s, E, b, v;
int B, S;
long mask_t, mask_s;
char *t;

typedef struct _node{
    struct _node *next_list;
    struct _node *pre_list;
    struct _node *next_table;
    struct _node *pre_table;
    long line;
} node;

typedef struct{
    node *head;
    node *tail;
    node **table;
    int size;
} set_lru;

typedef set_lru* vcache;

// 函数原型
int initOptions(int argc, char **argv);
void printHelpInfo(void);
vcache initCache(void); 
void fetch(vcache cache, long addr);
node* getNode(set_lru* set, long addr); 
void updateNode(set_lru* set, node *found);
void putNode(set_lru* set, long addr);
void deleteNode(set_lru *set);
int hash(long tag);
void freeCache(vcache cache); 
void freeSet(set_lru* set); 

int main(int argc, char **argv){
    // 解析命令行参数参数
    if (!initOptions(argc, argv)){
        printHelpInfo();
        return 1;
    } 

    // 初始化文件
    FILE *fp = fopen(t, "r");
    if (!fp){
        printHelpInfo();
        return 1;
    }

    // 初始化虚拟缓存
    vcache cache = initCache();

    // 解析文件
    char *buffer = malloc(BUFFER_SIZE);
    char type;
    long addr;
    int size;
    while(fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        type = strtok(buffer, " ")[0];
        if ('I' != type){
            addr = strtol(strtok(NULL, ","), NULL, 16);
            size = atoi(strtok(NULL, ","));
            if (v){
                printf("%c %lx,%i", type, addr, size);
            }
            fetch(cache, addr);
            if (type == 'M'){
                fetch(cache, addr);
            }
            if(v){
                printf("\n");
            }
        }
    }

    // 关闭文件
    fclose(fp);

    // 释放内存
    freeCache(cache);
    free(buffer);

    // 展示结果
    printSummary(hit_count, miss_count, eviction_count);    
}

int initOptions(int argc, char **argv){
    int opt;
    if (argc == 1){
        return 0;
    }
    while ((opt = getopt(argc, argv, ":hvs:E:b:t:")) != -1){
        switch (opt){
            case 'h':
            case '?':
            case ':':
                return 0;
                break;
            case 'v':
                v = 1;
                break;
            case 's':
                s = atoi(optarg);
                S = 1 << s;
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                B = 1 << b;
                break;
            case 't':
                t = optarg;
                break;  
        }
    }
    if (!s || !E || !b || !t){
        return 0;
    }
    mask_t = (-1L << (s + b)) ^ (-1L << 48);
    mask_s = (-1L << (s + b)) ^ (-1L << b);
    return 1;
}

void printHelpInfo(void){
    printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n\n");
    printf("\t-h: Optional help flag that prints usage info\n\n");
    printf("\t-v: Optional verbose flag that displays trace info\n\n");
    printf("\t-s <s>: Number of set index bits\n\n");
    printf("\t-E <E>: Associativity\n\n");
    printf("\t-b <b>: Number of block bits\n\n");
    printf("\t-t <tracefile>: Name of the valgrind trace to replay\n");
}

vcache initCache(void){
    vcache out = calloc(sizeof(set_lru), S);
    if (!out){
        return NULL;
    }
    for (int i = 0; i < S; i++){
        out[i].table = calloc(sizeof(node*), TABLE_SIZE);
        if (!out[i].table){
            for (int j = 0; j < i; j++){
                free(out[j].table);
            }
            return NULL;
        }
        out[i].size = 0;
        out[i].head = NULL;
        out[i].tail = NULL;
    }
    return out;
}

void fetch(vcache cache, long addr){
    set_lru *set = &cache[(mask_s & addr) >> b];
    node *found = getNode(set, addr);
    if (found){
        if (v){
            printf(" hit");
        }
        hit_count++;
        updateNode(set, found);
    } else {
        if (v){
            printf(" miss");
        }
        miss_count++;
        putNode(set, addr);
    }
    return;
}

node* getNode(set_lru* set, long addr){
    long tag = addr & mask_t;
    int hash_v = hash(tag);
    node* tmp = set -> table[hash_v];
    while(tmp){
        if (tmp -> line == tag){
            return tmp;
        }
        tmp = tmp -> next_table;
    }
    return NULL;
}

void updateNode(set_lru* set, node *found){
    if (set -> size == 1 || ! (found -> pre_list)){
        return;
    }
    found -> pre_list -> next_list = found -> next_list;
    if (found -> next_list){
        found -> next_list -> pre_list = found -> pre_list;
    } else {
        set -> tail = found -> pre_list;
    }
    found -> next_list = set -> head;
    found -> pre_list = NULL;
    set -> head -> pre_list = found;
    set -> head = found;
}

void putNode(set_lru* set, long addr){
    // Create a new node
    long tag = addr & mask_t;
    node *new = malloc(sizeof(node));
    if (!new){
        return;
    }
    new -> line = tag;

    // Insert node into the set
    node* set_head = set -> head;
    new -> next_list = set_head;
    new -> pre_list = NULL;
    set -> head = new;
    if (set_head){
        set_head -> pre_list = new;
    }
    if (set -> size == 0){
        set -> tail = new;
    }

    // Insert node into the hashtable
    int hash_v = hash(tag);
    node* table_head = set -> table[hash_v];
    new -> next_table = table_head;
    new -> pre_table = NULL;
    set -> table[hash_v] = new;
    if (table_head){
        table_head -> pre_table = new;
    }
    
    // Remove the oldest node
    if (set -> size < E){
        set -> size++;
    } else {
        deleteNode(set);
    }
}

void deleteNode(set_lru *set){
    // Remove from set
    node *remove = set -> tail;
    remove -> pre_list -> next_list = NULL;
    set -> tail = remove -> pre_list;

    // Remove from hash table
    int hash_v = hash(remove -> line);
    if (remove -> pre_table){
        remove -> pre_table -> next_table = remove -> next_table;
    } else {
        set -> table[hash_v] = remove -> next_table;
    }
    if (remove -> next_table){
        remove -> next_table -> pre_table = remove -> pre_table;
    }
    
    // Free memory
    free(remove);

    // Update
    if(v){
        printf(" eviction");
    }
    eviction_count++;
}

int hash(long tag){
    return tag % TABLE_SIZE;
}

void freeCache(vcache cache){
    for (int i = 0; i < S; i++){
        freeSet(&cache[i]);
    }
    free(cache);
}

void freeSet(set_lru *set){
    node *head = set -> head;
    node *tmp;
    while(head){
        tmp = head;
        head = head -> next_list;
        free(tmp);
    }
    free(set -> table);
    return;
}