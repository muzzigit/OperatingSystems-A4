# Virtual Memory Simulator

A CP386 (Operating Systems) assignment implementing a virtual memory management system with page table translation and clock page replacement algorithm.

## Features

- **Page Table Management**: Tracks virtual-to-physical address mappings with presence and use bits
- **Clock Page Replacement**: Implements the clock (second-chance) algorithm for victim page selection
- **Address Translation**: Converts logical addresses to physical addresses
- **Page Fault Handling**: Simulates page faults and memory load operations
- **Performance Statistics**: Reports page faults, replacements, and fault rate

## Configuration

- **Page Size**: 4,096 bytes
- **Virtual Pages**: 64
- **Physical Frames**: 16
- **Memory Accesses**: 100 (simulated)

## Building & Running

```bash
gcc -o a4 a4.c
./a4
```

## Output

The program displays:
- Initial page table state
- Per-access translation details (hits/faults)
- Clock algorithm victim selection
- Final statistics and page table contents

## Concepts

- Virtual memory and paging
- Page tables and frame allocation
- Clock page replacement algorithm
- Address translation (VA → PA)
- Page fault interrupts
