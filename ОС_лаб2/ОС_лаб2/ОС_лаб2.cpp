#include <windows.h>
#include <iostream>
#include <clocale>

using namespace std;

int* arr = nullptr;
int n = 0;

int minIndex = 0, maxIndex = 0;   
double averageValue = 0.0;        

DWORD WINAPI min_max(LPVOID)
{
    minIndex = 0;
    maxIndex = 0;

    for (int i = 1; i < n; i++)
    {
        if (arr[i] < arr[minIndex])
            minIndex = i;
        Sleep(7);                 

        if (arr[i] > arr[maxIndex])
            maxIndex = i;
        Sleep(7);                 
    }

    cout << "[min_max] Минимальный элемент: " << arr[minIndex] << endl;
    cout << "[min_max] Максимальный элемент: " << arr[maxIndex] << endl;
    return 0;
}

DWORD WINAPI average(LPVOID)
{
    double sum = 0;

    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
        Sleep(12);               
    }

    averageValue = sum / n;
    cout << "[average] Среднее значение: " << averageValue << endl;
    return 0;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "Введите размерность массива: ";
    cin >> n;
    if (n <= 0)
    {
        cout << "Размерность должна быть положительной." << endl;
        return 1;
    }

    arr = new int[n];
    cout << "Введите элементы массива:" << endl;
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    HANDLE hMinMax = CreateThread(NULL, 0, min_max, NULL, 0, NULL);
    if (hMinMax == NULL)
    {
        cout << "Не удалось создать поток min_max." << endl;
        delete[] arr;
        return 1;
    }

    HANDLE hAverage = CreateThread(NULL, 0, average, NULL, 0, NULL);
    if (hAverage == NULL)
    {
        cout << "Не удалось создать поток average." << endl;
        WaitForSingleObject(hMinMax, INFINITE);
        CloseHandle(hMinMax);
        delete[] arr;
        return 1;
    }

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    arr[minIndex] = (int)averageValue;
    arr[maxIndex] = (int)averageValue;

    cout << "Результирующий массив:" << endl;
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;

    delete[] arr;
    return 0;
}
