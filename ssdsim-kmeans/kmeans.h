#ifndef KMEANS_H
#define KMEANS_H
//#define N total_logical_addr_size/logical_addr_unit 
//N is for the number of the logical addr slice which could be configured
#define N 16
// K is the number of clusters which could be equal to the paralism  of SSD. 
//such as channel num,plane number,etc.
#define K 3

//calc the size of the logical addr size
//#define LOGICAL_ADDR_NUMBER //equal to largest lsn num
//#define SLICE_SIZE LOGICAL_ADDR_NUMBER / N

//Sample time window, unit is second
#define TIME_WINDOW 3315750000 //get from tracefile
#define trainingCount 10000
//hotness related structure
typedef struct
{
    float updateFreq;
    float updateInterval;
}hotnessMeta;

extern long trainingTriggerTime[N];

extern hotnessMeta clusterCenter[K];
//to store clusterCenter

extern int sliceCenter[N];
//lookup table
//if sliceCenter[0] = 1
//slice 0 belongs to cluster 2
//0<=sliceCenter[i]<=k-1

//to store metadata of each slice for training 
extern hotnessMeta hotnessData[N];
extern float endTime;


/*
hotnessMeta hotnessData[N] = {
    {2.0, 10.0},
    {2.0, 5.0},
    {8.0, 4.0},
    {5.0, 8.0},
    {7.0, 5.0},
    {6.0, 4.0},
    {1.0, 2.0},
    {4.0, 9.0},
    {7.0, 3.0},
    {1.0, 3.0},
    {3.0, 9.0}
};
*/

void kmeansTblInit(hotnessMeta *hotnessData);
void kmeansPretreat();
void kmeansCol();

float getDistance(hotnessMeta point1, hotnessMeta point2);
void getMean(int sliceCenter[N]);
float getE();
void cluster();
int kmeansTraining();

int transferLPN2Slice(int lpn,struct ssd_info * ssd);
void startKmeansTraining();
int kmeansMonitoring(int lpn,struct ssd_info * ssd,int count);
void kmeansCore();
void kmeansDispatch();
int processKmeans(int *sliceNumberPerCluster);
//reserved interface
void writeKmeans();
#endif
