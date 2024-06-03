//
// Created by 86156 on 2024/5/1.
//
#include "score.h"
#include "stdio.h"

int* getInput()
{
    static int scores[NUM];
    printf("\r\n开始测试\n");
    for (int i = 0; i < NUM; i++) {
        printf("请输入第%i分数\r\n", i+1);
        scanf("%d", &scores[i]);
    }
    return scores;
}