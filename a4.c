/* 
 * CP386 Assignment 4
 * Virtual memory simulator using:
 *   - page table (frame, present, use bits)
 *   - clock page replacement
 *   - random logical address generator
 *   - logical → physical translation
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define PAGE_SIZE 4096
#define NUM_PAGES 64
#define NUM_FRAMES 16
#define NUM_ACCESSES 100

typedef struct {
    int frame_number;
    bool present_bit;
    bool use_bit;
} PageTableEntry;

PageTableEntry page_table[NUM_PAGES];
int clock_hand = 0;
int page_faults = 0;
int page_replacements = 0;


// Initializes the page table
void initialize_page_table() {
    printf("\n--- init page table ---\n");

    for (int i = 0; i < NUM_PAGES; i++) {
        if (i < NUM_FRAMES) {
            // first frames start loaded for simplicity
            page_table[i].frame_number = i;
            page_table[i].present_bit = true;
            page_table[i].use_bit = false;

            printf("page %d -> frame %d (in mem)\n", i, i);
        } else {
            page_table[i].frame_number = -1;
            page_table[i].present_bit = false;
            page_table[i].use_bit = false;

            printf("page %d -> not loaded\n", i);
        }
    }
    printf("\n");
}


// random virtual address
unsigned int generate_random_address() {
    return rand() % (NUM_PAGES * PAGE_SIZE);
}


// get page num
unsigned int get_page_number(unsigned int va) {
    return va / PAGE_SIZE;
}

// get offset
unsigned int get_offset(unsigned int va) {
    return va % PAGE_SIZE;
}


// clock algo to find victim frame
int find_victim_page() {
    printf("   [clock] searching victim..\n");

    while (true) {
        int current_page = -1;

        // find which page is in this frame
        for (int i = 0; i < NUM_PAGES; i++) {
            if (page_table[i].present_bit && page_table[i].frame_number == clock_hand) {
                current_page = i;
                break;
            }
        }

        if (current_page == -1) {
            // empty frame case (rare)
            int vf = clock_hand;
            clock_hand = (clock_hand + 1) % NUM_FRAMES;
            return vf;
        }

        if (!page_table[current_page].use_bit) {
            // use bit is 0 -> replace it
            printf("   [clock] victim pg %d (frame %d)\n", current_page, clock_hand);

            page_table[current_page].present_bit = false;
            page_table[current_page].frame_number = -1;

            int vf = clock_hand;

            clock_hand = (clock_hand + 1) % NUM_FRAMES;
            return vf;
        } else {
            // use bit is 1, give second chance
            page_table[current_page].use_bit = false;
            clock_hand = (clock_hand + 1) % NUM_FRAMES;
        }
    }
}


// page fault handler
void handle_page_fault(unsigned int page_number) {
    printf("   [fault] page %d not in mem\n", page_number);
    page_faults++;

    int victim = find_victim_page();

    printf("   [rep] load page %d into frame %d\n", page_number, victim);

    page_table[page_number].frame_number = victim;
    page_table[page_number].present_bit = true;
    page_table[page_number].use_bit = true;

    page_replacements++;
}


// translate VA -> PA
unsigned int translate_address(unsigned int va) {
    unsigned int page = get_page_number(va);
    unsigned int off = get_offset(va);

    printf("\n--- translating ---\n");
    printf("VA: %u -> page %u off %u\n", va, page, off);

    if (!page_table[page].present_bit) {
        handle_page_fault(page);
    } else {
        printf("   [hit] page %u in mem (frame %d)\n",
               page, page_table[page].frame_number);
        page_table[page].use_bit = true;
    }

    unsigned int frame = page_table[page].frame_number;
    unsigned int pa = (frame * PAGE_SIZE) + off;

    printf("PA: %u\n", pa);
    return pa;
}


// main
int main() {
    srand(time(NULL));

    printf("--- virtual mem sim (cp386 a4) ---\n\n");

    initialize_page_table();

    for (int i = 0; i < NUM_ACCESSES; i++) {
        printf("\n== access %d ==\n", i + 1);

        unsigned int va = generate_random_address();
        translate_address(va);
    }

    printf("\n--- stats ---\n");
    printf("accesses: %d\n", NUM_ACCESSES);
    printf("faults: %d\n", page_faults);
    printf("replacements: %d\n", page_replacements);
    printf("fault rate: %.2f%%\n", (page_faults * 100.0) / NUM_ACCESSES);

    printf("\n--- final page table ---\n");
    printf("page | frame | present | use\n");
    printf("-----------------------------\n");

    for (int i = 0; i < NUM_PAGES; i++) {
        printf("%4d | ", i);

        if (page_table[i].present_bit)
            printf("%5d | ", page_table[i].frame_number);
        else
            printf(" N/A  | ");

        printf("%7s | %d\n",
               page_table[i].present_bit ? "yes" : "no",
               page_table[i].use_bit ? 1 : 0);
    }

    return 0;
}
