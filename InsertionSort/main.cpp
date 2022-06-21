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
    int temp = 0;
    for (int i = 1; i < size; i++){
        for(int j = i - 1; j >= 0 && arr[j + 1] < arr[j]; j--){
            temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
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
