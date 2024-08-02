#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_lsa(const char *input_filename, const char *output_filename);

int main()
{
    process_lsa("text1.txt", "lsa1.txt");
    process_lsa("text2.txt", "lsa2.txt");
    return 0;
}
void process_lsa(const char *input_filename, const char *output_filename)
{
    FILE *inputFile, *outputFile;

    char line[100];
    int lsa_values[5];

    inputFile = fopen(input_filename, "r");
    if (inputFile == NULL)
    {
        printf("Error opening input file!\n");
        return;
    }

    outputFile = fopen(output_filename, "w");
    if (outputFile == NULL)
    {
        printf("Error opening output file!\n");
        fclose(inputFile);
        return;
    }

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        if (strncmp(line, "I (", 3) != 0)
        {
            continue;
        }

        sscanf(line, "%*[^LSA_READINGS:]: LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d \t LSA_5: %d",
               &lsa_values[0], &lsa_values[1], &lsa_values[2], &lsa_values[3], &lsa_values[4]);

        for (int i = 0; i < 5; i++)
        {
            if (lsa_values[i] < 900)
            {
                fprintf(outputFile, (i == 4) ? "Black\n" : "Black-");
            }
            else
            {
                fprintf(outputFile, (i == 4) ? "White\n" : "White-");
            }
        }
    }
    printf("Completed Processing");
    fclose(inputFile);
    fclose(outputFile);
}