#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "initialize.h"

#include "kmeans.h"

hotnessMeta hotnessData[N];
long trainingTriggerTime[N];
hotnessMeta clusterCenter[K];
int sliceCenter[N];
float endTime;

void kmeansTblInit(hotnessMeta *hotnessData)
{
    for (int i = 0; i < N; i++)
    {
        //if the slice is never written in, freq shall set to 0 and interval should be TIME_WINDOW
        (hotnessData+i)->updateFreq = 0;
        //hotnessData[i].updateInterval = TIME_WINDOW;  
        (hotnessData+i)->updateInterval = 0;    
    }
}
void kmeansPretreat()
{
    printf("zheng:  into kmeansPretreat\n ");
    float maxUpdateF,maxUpdateT;
    maxUpdateF = hotnessData[0].updateFreq;
    maxUpdateT = hotnessData[0].updateInterval;
    //Find Max slice in UpdateF TBL    
    //Find Max access in UpdateT TBL
    for (int i = 1; i < N; i++)
    {
        if(hotnessData[i].updateFreq > maxUpdateF)
            maxUpdateF = hotnessData[i].updateFreq;
        if (hotnessData[i].updateInterval > maxUpdateT)
            maxUpdateT = hotnessData[i].updateInterval;    
    }    
    //Scale UpdateF TBL and UpdateT TBL to the range [0, 512]
    //printf("MAX updatefreq:%f\tMAX updateInter%f\n",maxUpdateF,maxUpdateT);
    for (int i = 0; i < N; i++)
    {
        hotnessData[i].updateFreq = (hotnessData[i].updateFreq / maxUpdateF) * 512;
        hotnessData[i].updateInterval = (hotnessData[i].updateInterval / maxUpdateT) * 512;
    } 
}

float getDistance(hotnessMeta point1,  hotnessMeta point2)
{
    float d;
    d = sqrt((point1.updateFreq - point2.updateFreq) * (point1.updateFreq - point2.updateFreq) + (point1.updateInterval - point2.updateInterval) * (point1.updateInterval - point2.updateInterval));
    return d;
}

/// 计算每个簇的中心点
void getMean(int sliceCenter[N])
{
    hotnessMeta tep;
    int i, j, count = 0;
    for(i = 0; i < K; ++i)
    {
        count = 0;
        tep.updateFreq = 0.0;   /// 每算出一个簇的中心点值后清0
        tep.updateInterval = 0.0;
        for(j = 0; j < N; ++j)
        {
            if(i == sliceCenter[j])
            {
                count++;
                tep.updateFreq += hotnessData[j].updateFreq;
                tep.updateInterval += hotnessData[j].updateInterval;
            }
        }
        tep.updateFreq /= count;
        tep.updateInterval /= count;
        clusterCenter[i] = tep;
    }
    for(i = 0; i < K; ++i)
    {
        //printf("The new center point of %d is : \t( %f, %f )\n", i+1, clusterCenter[i].updateFreq, clusterCenter[i].updateInterval);
    }
}

/// 计算平方误差函数
float getE()
{
    int i, j;
    float cnt = 0.0, sum = 0.0;
    for(i = 0; i < K; ++i)
    {
        for(j = 0; j < N; ++j)
        {
            if(i == sliceCenter[j])
            {
                cnt = (hotnessData[j].updateFreq - clusterCenter[i].updateFreq) * (hotnessData[j].updateFreq - clusterCenter[i].updateFreq) + (hotnessData[j].updateInterval - clusterCenter[i].updateInterval) * (hotnessData[j].updateInterval - clusterCenter[i].updateInterval);
                sum += cnt;
            }
        }
    }
    return sum;
}

/// 把N个点聚类
void cluster()
{
    int i, j, q;
    float min;
    float distance[N][K];
    for(i = 0; i < N; ++i)
    {
        min = 999999.0;
        for(j = 0; j < K; ++j)
        {
            distance[i][j] = getDistance(hotnessData[i],clusterCenter[j]);
            /// //printf("%f\n", distance[i][j]);  /// 可以用来测试对于每个点与3个中心点之间的距离
        }
        for(q = 0; q < K; ++q)
        {
            if(distance[i][q] < min)
            {
                min = distance[i][q];
                sliceCenter[i] = q;
            }
        }
        //printf("( %.0f, %.0f )\t in cluster-%d\n", hotnessData[i].updateFreq, hotnessData[i].updateInterval, sliceCenter[i] + 1);
    }
    //printf("-----------------------------\n");
}

int kmeansTraining()
{
    //FILE *f1;
    //f1 = fopen(".\log.txt","w+");
    int i, j, n = 0;
    float temp1;
    float temp2, t;
    //printf("----------Data sets----------\n");
    for(i = 0; i < N; ++i)
    {
        //printf("\t( %.0f, %.0f )\n", hotnessData[i].updateFreq, hotnessData[i].updateInterval);
    }
    //printf("-----------------------------\n");

/*
    可以选择当前时间为随机数
    srand((unsigned int)time(NULL));
    for(i = 0; i < K; ++i)
    {
        j = rand() % K;
        mean[i].x = point[j].x;
        mean[i].y = point[j].y;
    }
*/
    //set intitial clusterCenter
    for (int i = 0; i < K; i++)
    {
        clusterCenter[i].updateFreq = hotnessData[i].updateFreq;      /// 初始化k个中心点
        clusterCenter[i].updateInterval = hotnessData[i].updateInterval;
    }

    
    cluster();          /// 第一次根据预设的k个点进行聚类
    temp1 = getE();        ///  第一次平方误差
    n++;                   ///  n计算形成最终的簇用了多少次

    //printf("The E1 is: %f\n\n", temp1);

    getMean(sliceCenter);
    cluster();
    temp2 = getE();        ///  根据簇形成新的中心点，并计算出平方误差
    n++;

    //printf("The E2 is: %f\n\n", temp2);

    while(fabs(temp2 - temp1) != 0)   ///  比较两次平方误差 判断是否相等，不相等继续迭代
    {
        if(n==10)
            break;
        temp1 = temp2;
        getMean(sliceCenter);
        cluster();
        temp2 = getE();
        n++;
        //printf("The E%d is: %f\n", n, temp2);
    }

    printf("The total number of cluster is: %d\n\n", n);  /// 统计出迭代次数
    for (int i = 0; i < N; i++)
    {
        printf("sliceCenter i :%d\n",sliceCenter[i]);
    } 
    return 0;    
}

int transferLPN2Slice(int lpn,struct ssd_info * ssd)
{
    int lsn = lpn * (ssd->parameter->subpage_page); 
    int largest_lsn=(unsigned int )((ssd->parameter->chip_num*ssd->parameter->die_chip*ssd->parameter->plane_die*ssd->parameter->block_plane*ssd->parameter->page_block*ssd->parameter->subpage_page)*(1-ssd->parameter->overprovide));
    ////printf("largest_lsn : %d\n",largest_lsn);
    int slice_lsn = largest_lsn / N;
    return lsn / slice_lsn;//slice 0 - slice 15
}

void startKmeansTraining()
{
    printf("startKmeansTraining\n");
    //trainingTriggerTime = {0};
    for(int i = 0;i < N;i++)
        trainingTriggerTime[i] = 0;
}

int kmeansMonitoring(int lpn,struct ssd_info * ssd,int count)
{   
    float currentTime;
    int sliceNum;
    currentTime = (ssd->request_queue->time);
    endTime = currentTime;
    int diffTime;
    printf("count = %d\n",count);
    //int currentTime = time(NULL);
    //printf("1:current time : %f\n",currentTime);
    ////printf("currentTime: %lld\t",currentTime);
    sliceNum = (int)transferLPN2Slice(lpn,ssd);
        if(trainingTriggerTime[sliceNum] == 0)
            trainingTriggerTime[sliceNum] = currentTime;
        //printf("2:sliceNUm:%d,trainingTriggerTime : %lld\n",sliceNum,trainingTriggerTime[sliceNum]);
        diffTime = currentTime - trainingTriggerTime[sliceNum];
        //printf("3:difftime : %lld\n",diffTime);
        ////printf("sliceNum: %d\n",sliceNum);
        
            //printf("4:origin updatefreq: %lld\n",hotnessData[sliceNum].updateFreq);
            hotnessData[sliceNum].updateFreq = hotnessData[sliceNum].updateFreq + 1;
            //printf("5:origin updatefreq + 1: %lld\n",hotnessData[sliceNum].updateFreq);
            hotnessData[sliceNum].updateInterval = diffTime;
    printf("after count record\n");
}

void kmeansCore()
{
    //printf("1.kmeansPretreat:\n");
    kmeansPretreat();
    printf("after kmeansPretreat:\n");
    kmeansTraining();
    for (int i = 0; i < N; i++)
    {
        hotnessData[i].updateFreq = 0;
        //hotnessData[i].updateInterval = TIME_WINDOW;  
        hotnessData[i].updateInterval = 0;
    }   
}

//for user write
void kmeansDispatch()
{
    
}

void writeKmeans()
{

}