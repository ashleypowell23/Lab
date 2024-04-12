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
    struct RecordType* record;
    int filled;
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
    printf("Records in Hash Table:\n");
    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].filled)
        {
            printf("Index %d -> %d %c %d\n", i, pHashArray[i].record->id,
                   pHashArray[i].record->name, pHashArray[i].record->order);
        }
    }
}

// Initialize the hash table
void initializeHash(struct HashType *pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
        pHashArray[i].record = NULL;
        pHashArray[i].filled = 0;
    }
}

// Insert a record into the hash table
void insertRecord(struct HashType *pHashArray, int hashSz, struct RecordType record)
{
    int index = hash(record.id, hashSz);
    
    // Linear probing to handle collisions
    while (pHashArray[index].filled)
    {
        index = (index + 1) % hashSz;
    }
    
    // Allocate memory for the record
    pHashArray[index].record = malloc(sizeof(struct RecordType));
    
    // Copy values from the passed record
    pHashArray[index].record->id = record.id;
    pHashArray[index].record->name = record.name;
    pHashArray[index].record->order = record.order;
    
    pHashArray[index].filled = 1;
}


int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    int hashSize = 100; // Size of the hash table
    
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    
    struct HashType *hashTable = malloc(sizeof(struct HashType) * hashSize);
    initializeHash(hashTable, hashSize);
    
    for(int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, hashSize, pRecords[i]);
    }
    
    displayRecordsInHash(hashTable, hashSize);
    
    free(pRecords);
    free(hashTable);
    
    return 0;
}

