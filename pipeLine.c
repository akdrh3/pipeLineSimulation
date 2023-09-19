#include <stdio.h>
#include <stdlib.h>

#define MAX_INSTRUCTIONS 32

enum PipelineStage
{
    FETCH_STAGE,
    DECODE_STAGE,
    EXECUTE_STAGE,
    MEM_STAGE,
    WRITE_BACK_STAGE,
    DONE
};

typedef struct
{
    char opcode;
    int rd;
    int rs;
    int rt;
    int Ft; // fetch time
    int Dt; // decode time
    int Et; // execute time
    int Mt; // memory time
    int Wt; // write back time
    enum PipelineStage stage;
} Instruction;

int initStructuresAndCounts(Instruction *instructions)
{
    FILE *fptr; // read test.in and parse the instruction
    fptr = fopen("test.in", "r");
    if (fptr == NULL) // check if the file exits
    {
        printf("Not able to open the file.");
    }

    // loop through each line of input and parse the data into instruction
    int i = 0;
    while (i < MAX_INSTRUCTIONS && fscanf(fptr, "%c,%d,%d,%d", &instructions[i].opcode, &instructions[i].rd, &instructions[i].rs, &instructions[i].rt) == 4)
    {
        i += 1;
    }

    fclose(fptr); // close file
    return i;
}

void fetch_stage(Instruction *instructions, int cycle, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (instructions[i].stage == FETCH_STAGE)
        {
            instructions[i].stage = DECODE_STAGE;
            instructions[i].Ft = cycle;
            break;
        }
    }
}

void decode_stage(Instruction *instructions, int cycle, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (instructions[i].stage == DECODE_STAGE)
        {
            instructions[i].stage = EXECUTE_STAGE;
            instructions[i].Dt = cycle;
            break;
        }
    }
}

void execute_stage(Instruction *instructions, int cycle, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (instructions[i].stage == EXECUTE_STAGE)
        {
            instructions[i].stage = MEM_STAGE;
            instructions[i].Et = cycle;
            break;
        }
    }
}

void memory_stage(Instruction *instructions, int cycle, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (instructions[i].stage == MEM_STAGE)
        {
            instructions[i].stage = WRITE_BACK_STAGE;
            instructions[i].Mt = cycle;
            break;
        }
    }
}

void write_back_stage(Instruction *instructions, int cycle, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (instructions[i].stage == WRITE_BACK_STAGE)
        {
            instructions[i].stage = DONE;
            instructions[i].Wt = cycle;
            break;
        }
    }
}

void emitOutput(Instruction *instructions, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%02d, %02d, %02d, %02d, %02d\n", instructions[i].Ft, instructions[i].Dt, instructions[i].Et, instructions[i].Mt, instructions[i].Wt);
    }
}

int main()
{
    Instruction instructions[MAX_INSTRUCTIONS] = {0}; // initialize all structures up to MAX_INSTRUCTIONS
    int icount = initStructuresAndCounts(&instructions);

    int completedInst, cycle = 0;
    while (completedInst < icount)
    {
        fetch_stage(&instructions, cycle, icount);
        decode_stage(&instructions, cycle, icount);
        execute_stage(&instructions, cycle, icount);
        memory_stage(&instructions, cycle, icount);
        write_back_stage(&instructions, cycle, icount);
        cycle += 1;
        completedInst += 1;
    }
    for (int i = 0; i < icount; i++)
    { // check if there is any instruction not done
        if (instructions[i].stage != DONE)
        {
            printf("something went wrong");
            break;
        }
    }

    emitOutput(instructions, icount);
    return 0;
}