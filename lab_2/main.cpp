#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

/*
 * Функция: generateArr
 * Назначение: заполняет двумерный массив случайными числами от -30.0 до 40.0.
 * @param arr - указатель на массив
 * @param n   - размерность квадратного массива
 * @return    - ничего не возвращает (void)
 */
void generateArr(double **arr, int n)
{
    double low = -30.00, up = 40.00;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            arr[i][j] = low + (up - low) * rand() / (double)RAND_MAX;
        }
    }
}

/*
 * Функция: findMax
 * Назначение: находит самую горячую точку массива (максимальную температуру).
 * @param arr - указатель на массив
 * @param n   - размерность квадратного массива
 * @return    - максимальное значение температуры (double)
 */
double findMax(double** arr, int n)
{
    double max = arr[0][0];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (max < arr[i][j]){
                max = arr[i][j];
            }
        }
    }
    return max;
}

/*
 * Функция: findMin
 * Назначение: находит самую холодную точку массива (минимальную температуру).
 * @param arr - указатель на массив
 * @param n   - размерность квадратного массива
 * @return    - минимальное значение температуры (double)
 */
double findMin(double** arr, int n)
{
    double min = arr[0][0];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (min > arr[i][j]){
                min = arr[i][j];
            }
        }
    }
    return min;   
}

/*
 * Функция: normalizeArr
 * Назначение: нормализует значения температур к диапазону [0; 1]
 *             по формуле (x - min) / (max - min), где min и max —
 *             минимальное и максимальное значения массива.
 * @param arr - указатель на массив
 * @param n   - размерность квадратного массива
 * @return    - ничего не возвращает (void)
 * @note      - если все элементы равны (max == min), деления на 0 не произойдёт,
 *              функция просто выйдет без изменений.
 */
void normalizeArr(double** arr, int n)
{
    double minV = findMin(arr, n);
    double maxV = findMax(arr, n);

    if (maxV == minV) return;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            arr[i][j] = (arr[i][j] - minV)/(maxV - minV);
        }
    }
}

/*
 * Функция: passByRef
 * Назначение: демонстрирует передачу двумерного массива в функцию
 *             через ссылку на указатель (double **&arr). Показывает,
 *             что функция работает с тем же массивом, а не с его копией,
 *             и выводит адрес массива в консоль.
 * @param arr - ссылка на указатель на массив
 * @param n   - размерность квадратного массива
 * @return    - ничего не возвращает (void)
 */
void passByRef(double **&arr, int n)
{
    cout << "Array passed by reference. Address: " << arr << endl;
    (void)n;
}

/*
 * Функция: printArr
 * Назначение: выводит все элементы массива в консоль построчно.
 * @param arr - указатель на массив
 * @param n   - размерность квадратного массива
 * @return    - ничего не возвращает (void)
 */
void printArr(double** arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int num;
    cin >> num;
    if (num <= 0) {
        cout << "N must be positive" << endl;
        return 1;
    }

    double **arr = new double*[num];
    for (int i = 0; i < num; i++)
       arr[i] = new double[num];

    srand(time(nullptr));
    generateArr(arr, num);
    printArr(arr, num);
    
    cout << "Print the number of your choose: \n" << "1. normalize array of tempeeratures\n" 
    << "2. find the hottest point\n" << "3. find the coldest point\n" 
    << "4. send the link of the massive in function" << endl;
    int user_choice;
    cin >> user_choice;
    switch (user_choice) {
    case 1:
        normalizeArr(arr, num);
        printArr(arr, num);
        break;
    case 2:
        cout << "Hottest: " << findMax(arr, num) << endl;
        break;
    case 3:
        cout << "Coldest: " << findMin(arr, num) << endl;
        break;
    case 4:
        passByRef(arr, num);
        break;
    default:
        cout << "Wrong choice" << endl;
        break;
}

    for (int i = 0; i < num; i++)
        delete[] arr[i];
    delete[] arr;

    return 0;
}