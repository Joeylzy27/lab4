#include "vm.h"
#include "put.h"

extern unsigned long long text_start;
extern unsigned long long rodata_start;
extern unsigned long long data_start;
extern unsigned long long _end;

#pragma GCC push_options
#pragma GCC optimize("O1")
void create_mapping(uint64_t *pgtbl, uint64_t va, uint64_t pa, uint64_t sz, int perm)
{

    // close_virtual_mapping();

    // static int last_allocated_page_addr=null; // root page
    // if(last_allocated_page_addr==null) last_allocated_page_addr=pgtbl;
    // root_page = pgtbl

    // static int page_count = 0;
    #define page_count cur

    //提取各级虚拟页号
    int VPN_2 = (va >> 30) & 0x1FF;
    int VPN_1 = (va >> 21) & 0x1FF;
    int VPN_0 = (va >> 12) & 0x1FF;

    uint64_t *second_pgtbl; //二级页表的基地址
    if ( (pgtbl[VPN_2] & 0x1) == 0){
        page_count++;   //分配新的物理页
        second_pgtbl = (void*)((uint64_t)(&_end) + 0x1000 * page_count); //获取基地址
        for (int i = 0; i < 512; i++)
            second_pgtbl[i] = 0;
        pgtbl[VPN_2] |= (((uint64_t)second_pgtbl >> 12) << 10); //存储二级页表的物理基页
        pgtbl[VPN_2] |= 0x1;    //对valid位置位
    }
    second_pgtbl = (void*)((pgtbl[VPN_2] >> 10) << 12);

    uint64_t *third_pgtbl; //三级页表的基地址
    if ( (second_pgtbl[VPN_1] & 0x1) == 0)
    {
        page_count++;
        third_pgtbl = (void*)((uint64_t)(&_end) + 0x1000 * page_count); //获取基地址
        for (int i = 0; i < 512; i++)
            third_pgtbl[i] = 0;
        second_pgtbl[VPN_1] |= (((uint64_t)third_pgtbl >> 12) << 10); //存储三级页表的物理基页
        second_pgtbl[VPN_1] |= 0x1;
    }
    third_pgtbl = (void*)((second_pgtbl[VPN_1] >> 10) << 12);

    if ( (third_pgtbl[VPN_0] & 0x1) == 0)
    {
        third_pgtbl[VPN_0] |= (pa >> 12) << 10; //存储实际的物理页
        third_pgtbl[VPN_0] |= 0x1;          //valid置位
        third_pgtbl[VPN_0] |= perm << 1;    //权限置位

    }
    
    // second_level_page_addr = root_page[va.VPN[2]].PPN
    // if second_level_page_addr == null :
    //     last_allocated_page_addr += 0x1000
    //     root_page[va.VPN[2]].init()
    //     root_page[va.VPN[2]].PPN = last_allocated_page_addr
    //     second_level_page_addr = last_allocated_page_addr
    //     second_level_page_addr.clear()

    // third_level_page_addr = second_level_page_addr[va.VPN[1]].PPN
    // if third_level_page_addr == null :
    //     last_allocated_page_addr += 0x1000
    //     second_level_page_addr[va.VPN[1]].init()
    //     second_level_page_addr[va.VPN[1]].PPN = last_allocated_page_addr
    //     third_level_page_addr = last_allocated_page_addr
    //     third_level_page_addr.clear()

    // third_level_page_addr[va.VPN[0]].init()
    // third_level_page_addr[va.VPN[0]].PPN = pa
    // third_level_page_addr[va.VPN[0]].RSW = perm

    // open_virtual_mapping();
}
#pragma GCC pop_options


#pragma GCC push_options
#pragma GCC optimize("O1")
void paging_init()
{
    uint64_t *pgtbl = &_end;
    // for (int i = 0; i < 512; i+=4){
    //     pgtbl[i] = 0;
    //     pgtbl[i+3] = 0;
    //     pgtbl[i+1] = 0;
    //     pgtbl[i+2] = 0;
    // }
    for(int i=0;i<512;i++){
        pgtbl[i]=0;
    }
    // void create_mapping(uint64_t *pgtbl, uint64_t va, uint64_t pa, uint64_t sz, int perm)

    for (uint64_t va = 0xffffffe000000000; va < 0xffffffe001000000; va = va + 0x1000)
    {
        create_mapping(pgtbl, va, va - offset, 0x1000, 7);
        create_mapping(pgtbl, va - offset, va - offset, 0x1000, 7);
    }

    for (uint64_t va = 0x10000000; va < 0x10001000; va += 0x1000)
        create_mapping(pgtbl, va, va, 0x1000, 7);
    
}
#pragma GCC pop_options