#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Fill out this structure
struct HashType
{
    struct RecordType* data;
    int size;
};

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    for (i=0;i<hashSz;++i)
    {
        // if index is occupied with any records, print all
        if (pHashArray[i].data != NULL)
        {
            printf("Index %d -> ", i);
            struct RecordType* record = pHashArray[i].data;
            printf("%d, %c, %d", record->id, record->name, record->order);
            while (record->next != NULL)
            {
                record = record->next;
                printf(" -> %d, %c, %d", record->id, record->name, record->order);
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Define and initialize hash table
    int hashSize = 10; // You can adjust the size as needed
    struct HashType *hashTable = (struct HashType*)malloc(hashSize * sizeof(struct HashType));
    for (int i = 0; i < hashSize; ++i)
    {
        hashTable[i].data = NULL;
        hashTable[i].size = 0;
    }

    // Hash records and store them in the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        int h = hash(pRecords[i].id, hashSize);
        // Add record to the hash table
        if (hashTable[h].data == NULL)
        {
            hashTable[h].data = &pRecords[i];
        }
        else
        {
            // Handle collisions by chaining
            struct RecordType* temp = hashTable[h].data;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = &pRecords[i];
        }
        hashTable[h].size++;
    }

    // Display records in the hash structure
    displayRecordsInHash(hashTable, hashSize);

    // Free allocated memory
    free(pRecords);
    free(hashTable);

    return 0;
}
