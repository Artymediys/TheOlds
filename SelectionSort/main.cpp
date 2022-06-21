#include <iostream>

using std::cin,
      std::cout,
      std::cerr;

int main() {
    //Инициализация массива
    unsigned int size = 0;
    cout << "Input size of array: ";
    cin >> size;
    if (cin.fail()){
        cerr << "Invalid input data!\n";
        return 0;
    }
    int *arr = new int [size];
    for (int i = 0; i < size; i++){
        cout << "Input " << i << " element of array: ";
        cin >> arr[i];
        if (cin.fail()){
            cerr << "Invalid input data!\n";
            return 0;
        }
    }

    //Алгоритм
    unsigned int lastElem = 0;
    while ((size - lastElem) != 1) {
        int maxElem = arr[0];
        int maxElemID = 0;
        for (int i = 1; i < size - lastElem; i++) {
            if (maxElem < arr[i]) {
                maxElem = arr[i];
                maxElemID = i;
            }
            if (i == (size - lastElem - 1)) {
                arr[maxElemID] = arr[i];
                arr[i] = maxElem;
                lastElem++;
            }
        }
    }

    //Вывод
    cout << "Sorted Array: [ ";
    for (int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }
    cout << "]\n";

    delete [] arr;
    return 0;
}
