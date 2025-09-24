#include <iostream>
#include "src/parameters/Parameters.h"
using namespace std;

void message(const char * ag) {
    cout << ag << " não será executado." << endl;
}

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Quantidades de argumentos errada" << endl;
        cerr << "Usage: " << argv[0] << " <integer_parameter>" << endl;
        return 1;
    }
    if (string(argv[1]) == "1") {
        cout << "Iniciando execução do AG: " << argv[1] << endl;
        main_AG();
        cout << "Finalizando execução do AG: " << argv[1] << endl;
    }else {
        message("AG");
    }
    if (string(argv[2]) == "1") {
        cout << "Iniciando execução do DE: " << argv[1] << endl;
        main_DE();
        cout << "Finalizando execução do AG: " << argv[1] << endl;
    }else {
        message("DE");
    }
    return 0;
}