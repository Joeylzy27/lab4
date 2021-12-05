#ifndef VM_H
#define VM_H
#define uint64_t unsigned long long
#define offset (0xffffffe000000000-0x80000000)
#define PAGE_SIZE 0x1000
void create_mapping(uint64_t* pgtbl,uint64_t va,uint64_t pa,uint64_t sz,int perm);
uint64_t cur = 0;
#define page_count cur
void paging_init();
#endif