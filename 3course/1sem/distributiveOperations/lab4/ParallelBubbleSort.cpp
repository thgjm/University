#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <mpi.h>

using namespace std;

enum split_mode
{
    KeepFirstHalf,
    KeepSecondHalf
};

int ProcNum = 0;   // Number of available processes
int ProcRank = -1; // Rank of current process

const double RandomDataMultiplier = 100.0;

// Function for simple setting the initial data
void DummyDataInitialization(double *&pData, int &DataSize)
{
    for (int i = 0; i < DataSize; i++)
        pData[i] = DataSize - i;
}

// Function for initializing the data by the random generator
void RandomDataInitialization(double *&pData, int &DataSize)
{
    srand((unsigned)time(0));
    for (int i = 0; i < DataSize; i++)
        pData[i] = double(rand()) / RAND_MAX * RandomDataMultiplier;
}

// Function for formatted data output
void PrintData(double *pData, int DataSize)
{
    for (int i = 0; i < DataSize; i++)
        printf("%7.4f ", pData[i]);
    printf("\n");
}

// Function for allocating the memory and setting the initial values
void ProcessInitialization(double *&pData, int &DataSize,
                           double *&pProcData, int &BlockSize)
{
    setvbuf(stdout, 0, _IONBF, 0);
    if (ProcRank == 0)
    {
        do
        {
            printf("Enter the size of data to be sorted: ");
            scanf("%d", &DataSize);
            if (DataSize < ProcNum)
            {
                printf("Data size should be greater than number of processes\n");
            }
        } while ((DataSize < ProcNum));
        // printf("Sorting %d data items\n", DataSize);
    }
    // Broadcasting the data size
    MPI_Bcast(&DataSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int RestData = DataSize;
    for (int i = 0; i < ProcRank; i++)
        RestData -= RestData / (ProcNum - i);
    BlockSize = RestData / (ProcNum - ProcRank);
    pProcData = new double[BlockSize];
    if (ProcRank == 0)
    {
        pData = new double[DataSize];
        // Data initalization
        // DummyDataInitialization(pData, DataSize);
        RandomDataInitialization(pData, DataSize);
    }
}

// Function for computational process termination
void ProcessTermination(double *pData, double *pProcData)
{
    if (ProcRank == 0)
        delete[] pData;
    delete[] pProcData;
}

// Data distribution among the processes
void DataDistribution(double *pData, int DataSize, double *pProcData, int BlockSize)
{
    // Allocate memory for temporary objects
    int *pSendInd = new int[ProcNum];
    int *pSendNum = new int[ProcNum];
    int RestData = DataSize;
    int CurrentSize = DataSize / ProcNum;
    pSendNum[0] = CurrentSize;
    pSendInd[0] = 0;
    for (int i = 1; i < ProcNum; i++)
    {
        RestData -= CurrentSize;
        CurrentSize = RestData / (ProcNum - i);
        pSendNum[i] = CurrentSize;
        pSendInd[i] = pSendInd[i - 1] + pSendNum[i - 1];
    }
    MPI_Scatterv(pData, pSendNum, pSendInd, MPI_DOUBLE, pProcData,
                 pSendNum[ProcRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // Free the memory
    delete[] pSendNum;
    delete[] pSendInd;
}

/*
// Function for testing the data distribution
void TestDistribution(double *pData, int DataSize, double *pProcData,
                      int BlockSize)
{
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == 0)
    {
        printf("Initial data:\n");
        PrintData(pData, DataSize);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < ProcNum; i++)
    {
        if (ProcRank == i)
        {
            printf("ProcRank = %d\n", ProcRank);
            printf("Block:\n");
            PrintData(pProcData, BlockSize);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}
*/

// Function for testing the data distribution (root collects and prints) - fixed (uses Gatherv)
void TestDistribution(double *pData, int DataSize, double *pProcData,
                      int BlockSize)
{
    double *recvBuffer = nullptr;
    int *pReceiveNum = nullptr;
    int *pReceiveInd = nullptr;

    if (ProcRank == 0)
    {
        pReceiveNum = new int[ProcNum];
        pReceiveInd = new int[ProcNum];

        int RestData = DataSize;
        pReceiveInd[0] = 0;
        pReceiveNum[0] = DataSize / ProcNum;
        for (int i = 1; i < ProcNum; i++)
        {
            RestData -= pReceiveNum[i - 1];
            pReceiveNum[i] = RestData / (ProcNum - i);
            pReceiveInd[i] = pReceiveInd[i - 1] + pReceiveNum[i - 1];
        }

        int totalRecv = 0;
        for (int i = 0; i < ProcNum; i++)
            totalRecv += pReceiveNum[i];
        recvBuffer = new double[totalRecv];
    }

    MPI_Gatherv(pProcData, BlockSize, MPI_DOUBLE,
                recvBuffer, pReceiveNum, pReceiveInd, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    if (ProcRank == 0)
    {
        printf("\nInitial data:\n");
        PrintData(pData, DataSize);

        printf("\n=== Data distribution across processes ===\n");
        for (int i = 0; i < ProcNum; i++)
        {
            printf("\nProcRank = %d\n", i);
            printf("Block:\n");
            PrintData(recvBuffer + pReceiveInd[i], pReceiveNum[i]);
        }

        delete[] recvBuffer;
        delete[] pReceiveNum;
        delete[] pReceiveInd;
    }
}

// Function for parallel data output
/*void ParallelPrintData(double *pProcData, int BlockSize)
{
    // Print the sorted data
    for (int i = 0; i < ProcNum; i++)
    {
        if (ProcRank == i)
        {
            printf("ProcRank = %d\n", ProcRank);
            printf("Proc sorted data:\n");
            PrintData(pProcData, BlockSize);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}*/

// Function for parallel data output (collects everything to process 0)
void ParallelPrintData(double *pProcData, int BlockSize, int DataSize)
{
    double *pAllData = nullptr;
    int *pReceiveNum = nullptr;
    int *pReceiveInd = nullptr;

    if (ProcRank == 0)
    {
        pAllData = new double[DataSize];
        pReceiveNum = new int[ProcNum];
        pReceiveInd = new int[ProcNum];

        int RestData = DataSize;
        pReceiveInd[0] = 0;
        pReceiveNum[0] = DataSize / ProcNum;
        for (int i = 1; i < ProcNum; i++)
        {
            RestData -= pReceiveNum[i - 1];
            pReceiveNum[i] = RestData / (ProcNum - i);
            pReceiveInd[i] = pReceiveInd[i - 1] + pReceiveNum[i - 1];
        }
    }

    MPI_Gatherv(pProcData, BlockSize, MPI_DOUBLE,
                pAllData, pReceiveNum, pReceiveInd, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    if (ProcRank == 0)
    {
        printf("\n===== PARALLEL DATA OUTPUT =====\n");
        for (int i = 0; i < ProcNum; i++)
        {
            printf("ProcRank = %d\n", i);
            printf("Proc sorted data:\n");
            for (int j = 0; j < pReceiveNum[i]; j++)
                printf("%.2f ", pAllData[pReceiveInd[i] + j]);
            printf("\n-----------------------------\n");
        }
        printf("================================\n\n");

        delete[] pAllData;
        delete[] pReceiveNum;
        delete[] pReceiveInd;
    }
}

// Function for data exchange between the neighboring processes
void ExchangeData(double *pProcData, int BlockSize, int DualRank,
                  double *pDualData, int DualBlockSize)
{
    MPI_Status status;
    MPI_Sendrecv(pProcData, BlockSize, MPI_DOUBLE, DualRank, 0,
                 pDualData, DualBlockSize, MPI_DOUBLE, DualRank, 0,
                 MPI_COMM_WORLD, &status);
}

// Function for the serial bubble sort algorithm
void SerialBubbleSort(double *pData, int DataSize)
{
    double Tmp;
    for (int i = 1; i < DataSize; i++)
        for (int j = 0; j < DataSize - i; j++)
            if (pData[j] > pData[j + 1])
            {
                Tmp = pData[j];
                pData[j] = pData[j + 1];
                pData[j + 1] = Tmp;
            }
}

// Parallel bubble sort algorithm
void ParallelBubble(double *pProcData, int BlockSize)
{
    // Local sorting the process data
    SerialBubbleSort(pProcData, BlockSize);

    int Offset;
    split_mode SplitMode = KeepFirstHalf;

    for (int i = 0; i < 2 * ProcNum; i++)
    {
        if ((i % 2) == 1)
        {
            if ((ProcRank % 2) == 1)
            {
                Offset = 1;
                SplitMode = KeepFirstHalf;
            }
            else
            {
                Offset = -1;
                SplitMode = KeepSecondHalf;
            }
        }
        else
        {
            if ((ProcRank % 2) == 1)
            {
                Offset = -1;
                SplitMode = KeepSecondHalf;
            }
            else
            {
                Offset = 1;
                SplitMode = KeepFirstHalf;
            }
        }
        
        // Check the first and last processes
        if ((ProcRank == ProcNum - 1) && (Offset == 1))
            continue;
        if ((ProcRank == 0) && (Offset == -1))
            continue;

        MPI_Status status;
        int DualBlockSize;
        MPI_Sendrecv(&BlockSize, 1, MPI_INT, ProcRank + Offset, 0,
                     &DualBlockSize, 1, MPI_INT, ProcRank + Offset, 0,
                     MPI_COMM_WORLD, &status);
        double *pDualData = new double[DualBlockSize];
        double *pMergedData = new double[BlockSize + DualBlockSize];
        // Data exchange
        ExchangeData(pProcData, BlockSize, ProcRank + Offset, pDualData,
                     DualBlockSize);
        // Data merging
        merge(pProcData, pProcData + BlockSize,
              pDualData, pDualData + DualBlockSize, pMergedData);
        // Data splitting
        if (SplitMode == KeepFirstHalf)
            copy(pMergedData, pMergedData + BlockSize, pProcData);
        else
            copy(pMergedData + DualBlockSize, pMergedData + BlockSize + DualBlockSize, pProcData);
        delete[] pDualData;
        delete[] pMergedData;
    }
}

// Function for data collection
void DataCollection(double *pData, int DataSize, double *pProcData,
                    int BlockSize)
{
    // Allocate memory for temporary objects
    int *pReceiveNum = new int[ProcNum];
    int *pReceiveInd = new int[ProcNum];
    int RestData = DataSize;
    pReceiveInd[0] = 0;
    pReceiveNum[0] = DataSize / ProcNum;
    for (int i = 1; i < ProcNum; i++)
    {
        RestData -= pReceiveNum[i - 1];
        pReceiveNum[i] = RestData / (ProcNum - i);
        pReceiveInd[i] = pReceiveInd[i - 1] + pReceiveNum[i - 1];
    }
    MPI_Gatherv(pProcData, BlockSize, MPI_DOUBLE, pData,
                pReceiveNum, pReceiveInd, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // Free the memory
    delete[] pReceiveNum;
    delete[] pReceiveInd;
}

// Function for copying the sorted data
void CopyData(double *pData, int DataSize, double *pDataCopy)
{
    std::copy(pData, pData + DataSize, pDataCopy);
}

// Function for comparing the data
bool CompareData(double *pData1, double *pData2, int DataSize)
{
    return std::equal(pData1, pData1 + DataSize, pData2);
}

/*
// Function for testing the result of parallel bubble sort
void TestResult(double *pData, double *pSerialData, int DataSize)
{
    MPI_Barrier(MPI_COMM_WORLD);
    if (ProcRank == 0)
    {
        SerialBubbleSort(pSerialData, DataSize);
        if (!CompareData(pData, pSerialData, DataSize))
        {
            printf("The results of serial and parallel algorithms are "
                   "NOT identical. Check your code\n");
        }
        else
        {
            printf("The results of serial and parallel algorithms are "
                   "identical\n");
        }
    }
}*/

// Function for testing the result of parallel bubble sort (root collects all data)
void TestResult(double *pProcData, double *pSerialData, int DataSize, int BlockSize)
{
    double *pAllData = nullptr;
    int *pReceiveNum = nullptr;
    int *pReceiveInd = nullptr;

    if (ProcRank == 0)
    {
        pAllData = new double[DataSize];

        pReceiveNum = new int[ProcNum];
        pReceiveInd = new int[ProcNum];

        int RestData = DataSize;
        pReceiveInd[0] = 0;
        pReceiveNum[0] = DataSize / ProcNum;
        for (int i = 1; i < ProcNum; i++)
        {
            RestData -= pReceiveNum[i - 1];
            pReceiveNum[i] = RestData / (ProcNum - i);
            pReceiveInd[i] = pReceiveInd[i - 1] + pReceiveNum[i - 1];
        }
    }

    MPI_Gatherv(pProcData, BlockSize, MPI_DOUBLE,
                pAllData, pReceiveNum, pReceiveInd, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    if (ProcRank == 0)
    {
        SerialBubbleSort(pSerialData, DataSize);

        if (!CompareData(pAllData, pSerialData, DataSize))
            printf("\nThe results of serial and parallel algorithms are NOT identical. Check your code\n");
        else
            printf("\nThe results of serial and parallel algorithms are identical.\n");

        delete[] pAllData;
        delete[] pReceiveNum;
        delete[] pReceiveInd;
    }
}

int main(int argc, char *argv[])
{
    double *pData = 0;
    double *pProcData = 0;
    int DataSize = 0;
    int BlockSize = 0;
    double *pSerialData = 0;

    double start, finish, duration;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0)
        printf("Parallel bubble sort program\n");
    // Process initialization
    ProcessInitialization(pData, DataSize, pProcData, BlockSize);

    if (ProcRank == 0)
    {
        // Data copying
        pSerialData = new double[DataSize];
        CopyData(pData, DataSize, pSerialData);
    }

    start = MPI_Wtime();
    // Distributing the initial data among processes
    DataDistribution(pData, DataSize, pProcData, BlockSize);

    // Testing the data distribution
    //TestDistribution(pData, DataSize, pProcData, BlockSize);

    // Parallel bubble sort
    ParallelBubble(pProcData, BlockSize);

    //ParallelPrintData(pProcData, BlockSize, DataSize);

    // Process data collection
    DataCollection(pData, DataSize, pProcData, BlockSize);
    finish = MPI_Wtime();
    duration = finish - start;
    if (ProcRank == 0)
        printf("\nTime of execution: %f\n", duration);

    TestResult(pProcData, pSerialData, DataSize, BlockSize);

    // Process termination
    ProcessTermination(pData, pProcData);
    if (ProcRank == 0)
        delete[] pSerialData;
    MPI_Finalize();
}