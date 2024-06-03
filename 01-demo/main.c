#include <stdio.h>
#include "test.h"
#include "my_dir.h"
#include "my_code.h"
#include "score.h"
int main(void) {
    sayhi();
    printdir();
    generator_code();
    int* arr = getInput();
    double sum = 0;
    for (int i = 0; i < NUM; ++i) {
        printf("输入第%i个分数是：%i\r\n", (i+1), *(arr++));
        printf("数组值是：%i\r\n", arr[i]);
        sum += arr[i];
    }
    printf("合计：%f\n", sum);
    printf("平均值%.2f\n", (double )(sum / NUM));
    return 0;
}
