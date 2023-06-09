//https://school.programmers.co.kr/learn/courses/30/lessons/42840

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> answers) {
    
    vector<int> answer;

    vector<int> person1; // 1�� ������
    vector<int> person2; // 2�� ������
    vector<int> person3; // 3�� ������

    int p1 = 0; // 1�� �����ڿ� ����
    int p2 = 0; // 2�� �����ڿ� ����
    int p3 = 0; // 3�� �����ڿ� ����

    int cnt1 = 0;
    int cnt2 = 0;
    int cnt3 = 0;

    //1�� ������ ��� ���
    for (int i = 0; i < answers.size(); i++)
    {
        p1 = (i % 5) + 1;
        person1.push_back(p1);
    }


    //2�� ������ ��� ���
    for (int i = 1; i <= answers.size(); i++)
    {
        if (i % 8 == 2)
        {
            person2.push_back(1);
        }
        else if(i % 8 == 4)
        {
            person2.push_back(3);
        }
        else if (i % 8 == 6)
        {
            person2.push_back(4);
        }
        else if (i % 8 == 0)
        {
            person2.push_back(5);
        }
        else
        {
            person2.push_back(2);
        }
    }

    //3�� ������ ��� ���
    for (int i = 1; i <= answers.size(); i++)
    {
        if (i % 10 == 1 || i % 10 == 2)
        {
            person3.push_back(3);
        }
        else if (i % 10 == 3 || i % 10 == 4)
        {
            person3.push_back(1);
        }
        else if (i % 10 == 5 || i % 10 == 6)
        {
            person3.push_back(2);
        }
        else if (i % 10 == 7 || i % 10 == 8)
        {
            person3.push_back(4);
        }
        else if (i % 10 == 9 || i % 10 == 0)
        {
            person3.push_back(5);
        }
    }


    //answers �迭�� ������ 1,2,3 �迭 ��
    for (int i = 0; i < answers.size(); i++)
    {
        if (answers[i] == person1[i])
        {
            cnt1++;
        }

        if (answers[i] == person2[i])
        {
            cnt2++;
        }

        if (answers[i] == person3[i])
        {
            cnt3++;
        }

    }

    // ���� ���� ���� ���
    int maxScore = max(cnt1, max(cnt2, cnt3)); //max �Լ� : �� ���� �� �߿��� �� ū ���� ��ȯ

    // ���� ���� ������ ���� ������ ã��
    if (cnt1 == maxScore)
    {
        answer.push_back(1);
    }
    if (cnt2 == maxScore)
    {
        answer.push_back(2);
    }
    if (cnt3 == maxScore)
    {
        answer.push_back(3);
    }

    return answer;
}

int main(void)
{
    vector<int> answers;
    answers = { 1,3,2,4,2 };

    solution(answers);

    return 0;
}