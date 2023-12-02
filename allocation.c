#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000000

typedef struct {
    int size;
    int start;
    char process[50];
} Block;

Block memory[MAX];
int numBlocks = 0;

void allocate(char process[], int size) {
    int i, bestIndex = -1, bestSize = MAX+1;
    for (i = 0; i < numBlocks; i++) {
        if (strcmp(memory[i].process, "FREE") == 0 && memory[i].size >= size && memory[i].size < bestSize) {
            bestSize = memory[i].size;
            bestIndex = i;
        }
    }
    if (bestIndex != -1) {
        strcpy(memory[bestIndex].process, process);
        if (memory[bestIndex].size > size) {
            memmove(&memory[bestIndex+1], &memory[bestIndex], (numBlocks - bestIndex) * sizeof(Block));
            strcpy(memory[bestIndex].process, process);
            memory[bestIndex].size = size;
            memory[bestIndex+1].size -= size;
            memory[bestIndex+1].start += size;
            numBlocks++;
        }
        printf("Successfully allocated %d to process %s\n", size, process);
    } else {
        printf("Error: Not enough memory to allocate to process %s\n", process);
    }
}

void release(char process[]) {
    int i;
    for (i = 0; i < numBlocks; i++) {
        if (strcmp(memory[i].process, process) == 0) {
            strcpy(memory[i].process, "FREE");
            if (i > 0 && strcmp(memory[i-1].process, "FREE") == 0) {
                memory[i-1].size += memory[i].size;
                memmove(&memory[i], &memory[i+1], (numBlocks - i - 1) * sizeof(Block));
                numBlocks--;
                i--;
            }
            if (i < numBlocks - 1 && strcmp(memory[i+1].process, "FREE") == 0) {
                memory[i].size += memory[i+1].size;
                memmove(&memory[i+1], &memory[i+2], (numBlocks - i - 2) * sizeof(Block));
                numBlocks--;
            }
            printf("Successfully released memory for process %s\n", process);
            return;
        }
    }
    printf("Error: No memory allocated to process %s\n", process);
}

void compact() {
    int i, j;
    for (i = 0; i < numBlocks; i++) {
        if (strcmp(memory[i].process, "FREE") == 0) {
            for (j = i+1; j < numBlocks; j++) {
                if (strcmp(memory[j].process, "FREE") != 0) {
                    strcpy(memory[i].process, memory[j].process);
                    memory[i].size = memory[j].size;
                    strcpy(memory[j].process, "FREE");
                    break;
                }
            }
        }
    }
    printf("Compaction process is successful\n");
}

void status() {
    int i;
    printf("Partitions [Allocated memory= %d]:\n", MAX - memory[numBlocks-1].size);
    for (i = 0; i < numBlocks; i++) {
        if (strcmp(memory[i].process, "FREE") != 0) {
            printf("Address [%d:%d] Process %s\n", memory[i].start, memory[i].start + memory[i].size - 1, memory[i].process);
        }
    }
    printf("Holes [Free memory= %d]:\n", memory[numBlocks-1].size);
    for (i = 0; i < numBlocks; i++) {
        if (strcmp(memory[i].process, "FREE") == 0) {
            printf("Address [%d:%d] len = %d\n", memory[i].start, memory[i].start + memory[i].size - 1, memory[i].size);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Please provide the initial amount of memory as a command line argument\n");
        return 1;
    }
    int size = atoi(argv[1]);
    if (size <= 0 || size > MAX) {
        printf("Error: Invalid size of memory\n");
        return 1;
    }
    strcpy(memory[0].process, "FREE");
    memory[0].size = size;
    memory[0].start = 0;
    numBlocks = 1;
    printf("Here, the Best Fit approach has been implemented and the allocated %d bytes of memory.\n", size);
    char command[50], process[50], algorithm;
    int processSize;
    while (1) {
        printf("allocator>");
        scanf("%s", command);
        if (strcmp(command, "RQ") == 0) {
            scanf("%s %d %c", process, &processSize, &algorithm);
            if (algorithm == 'B') {
                allocate(process, processSize);
            } else {
                printf("Error: Invalid algorithm\n");
            }
        } else if (strcmp(command, "RL") == 0) {
            scanf("%s", process);
            release(process);
        } else if (strcmp(command, "C") == 0) {
            compact();
        } else if (strcmp(command, "Status") == 0) {
            status();
        } else if (strcmp(command, "Exit") == 0) {
            break;
        } else {
            printf("Error: Invalid command\n");
        }
    }
    return 0;
}
