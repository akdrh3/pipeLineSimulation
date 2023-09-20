#include <stdio.h>
#include <stdlib.h>

// build binary and give .o file and test.in

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

int initStructuresAndCounts(Instruction instructions[], FILE *fptr)
{
    char *line = NULL;
    size_t len = 0;
    __ssize_t read;

    // loop through each line of input and parse the data into instruction
    int i = 0;
    while (i < MAX_INSTRUCTIONS && (read = getline(&line, &len, fptr)) != -1)
    {
        if (sscanf(line, "%c,%d,%d,%d", &instructions[i].opcode, &instructions[i].rd, &instructions[i].rs, &instructions[i].rt) == 4)
        {
            instructions[i].Ft = 0;
            instructions[i].Dt = 0;
            instructions[i].Et = 0;
            instructions[i].Mt = 0;
            instructions[i].Wt = 0;
            instructions[i].stage = FETCH_STAGE;

            i += 1;
        }
    }

    if (line)
    {
        free(line);
    }

    fclose(fptr); // close file
    return i;
}

void fetch_stage(Instruction instructions[], int cycle, int len)
{
    int i = 0;
    // depends on decode stage, it should stall as well
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

void decode_stage(Instruction instructions[], int cycle, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    // if previous inst is L and the its rd is
    // same as next inst's rs or rt it will stall one cycle on decode and fetch stage
    {
        if (instructions[i].stage == DECODE_STAGE)
        {

            instructions[i].stage = EXECUTE_STAGE;
            instructions[i].Dt = cycle;
            break;
        }
    }
}

void execute_stage(Instruction instructions[], int cycle, int len)
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

void memory_stage(Instruction instructions[], int cycle, int len)
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

void write_back_stage(Instruction instructions[], int cycle, int len)
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

void emitOutput(Instruction instructions[], int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%02d, %02d, %02d, %02d, %02d\n", instructions[i].Ft, instructions[i].Dt, instructions[i].Et, instructions[i].Mt, instructions[i].Wt);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("%s\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("something went wrong while opening the file");
        return 1;
    }

    Instruction instructions[MAX_INSTRUCTIONS] = {0}; // initialize all structures up to MAX_INSTRUCTIONS
    int icount = initStructuresAndCounts(instructions, fptr);

    int cycle = 0;
    while (1)
    {
        write_back_stage(instructions, cycle, icount);
        memory_stage(instructions, cycle, icount);
        execute_stage(instructions, cycle, icount);
        decode_stage(instructions, cycle, icount);
        fetch_stage(instructions, cycle, icount);

        int all_completed = 1;
        for (int i = 0; i < icount; i++)
        {
            // check if there is any instruction not done
            if (instructions[i].stage < DONE)
            {
                all_completed = 0;
                break;
            }
        }

        if (all_completed)
        {
            break;
        }

        cycle += 1;
    }

    emitOutput(instructions, icount);
    return 0;
}

// if previous inst is L and the its rd is
// same as next inst's rs or rt it will stall one cycle on decode and fetch stage
