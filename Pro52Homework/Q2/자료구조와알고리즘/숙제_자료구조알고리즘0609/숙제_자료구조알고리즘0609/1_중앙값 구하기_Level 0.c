//https://school.programmers.co.kr/learn/courses/30/lessons/120811

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// array_len은 배열 array의 길이입니다.
int solution(int array[], size_t array_len) {
    int answer = 0;
    int temp = 0;
    int i, j = 0;

    for (i = 0; i < array_len - 1; i++)
    {
        for (j = 0; j < (array_len - 1) - i; j++)
        {
            if (array[j]>array[j+1])
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    int n = array_len / 2;
    answer = array[n];
    
    printf("%d", answer);

    return answer;
}

int main(void)
{
    int array_len = 3;
    int array[3] = { 9, -1, 0 };

    solution(array, array_len);

    return 0;
}