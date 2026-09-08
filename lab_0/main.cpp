#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

/*
 * Функция: generateArr
 * Назначение: заполняет массив случайными числами от 1 до 10.
 * @param arr - указатель на массив
 * @param n   - количество элементов в массиве
 * @return    - ничего не возвращает (void)
 */
void generateArr(int *arr, int n)
{
    int low = 1, up = 10;
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % up + low;
    }
}

/*
 * Функция: quickSort
 * Назначение: сортирует часть массива методом быстрой сортировки (рекурсия).
 * @param arr   - указатель на массив
 * @param start - индекс первого элемента в сортируемой части
 * @param end   - индекс последнего элемента в сортируемой части
 * @return      - ничего не возвращает (void)
 */
void quickSort(int *arr, int start, int end)
{
    if (start > end)
    {
        return;
    }
    int pivot = arr[rand() % (end - start + 1) + start];
    int l = start, r = end;
    while (l <= r)
    {
        while (arr[l] < pivot)
        {
            l++;
        }
        while (arr[r] > pivot)
        {
            r--;
        }
        if (l <= r)
        {
            int temp = arr[l];
            arr[l] = arr[r];
            arr[r] = temp;
            l++;
            r--;
        }
    }
    quickSort(arr, start, r);
    quickSort(arr, l, end);
}

/*
 * Функция: averageArr
 * Назначение: вычисляет среднее арифметическое всех элементов массива.
 * @param arr - указатель на массив
 * @param n   - количество элементов
 * @return    - среднее значение (тип double)
 */
double averageArr(int *arr, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += arr[i];
    }
    if (n == 0)
    {
        return 0;
    }
    else
    {
        return (double)sum / n;
    }
}

/*
 * Функция: printArr
 * Назначение: выводит все элементы массива в столбик.
 * @param arr - указатель на массив
 * @param n   - количество элементов
 * @return    - ничего не возвращает (void)
 */
void printArr(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << '\n'
         << endl;
}

int main()
{
    int num;
    std::cin >> num;
    int *arr = new int[num];
    srand(time(nullptr));
    generateArr(arr, num);
    printArr(arr, num);
    quickSort(arr, 0, num - 1);
    printArr(arr, num);
    cout << "Average: " << averageArr(arr, num) << endl;
    delete[] arr;
    return 0;
}