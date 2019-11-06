#include<iostream>
#include<string>
#include <unistd.h>
#include <sstream>
using namespace std;

string to_str(int number){
    stringstream ss;
    ss << number;
    
    return ss.str();
}

string char_to_str(char character){
    stringstream ss;
    ss << character;
    
    return ss.str();
}

string float_to_str(float number){
    stringstream ss;
    ss << number;
    
    return ss.str();
}

int str_to_int(string number){
    int value;
    stringstream ss(number);
    ss >> value;

    return value;
}

float str_to_float(string number){
    float value;
    stringstream ss(number);
    ss >> value;

    return value;
}


int pot(int b, int n){
    int result, i;

    if (n > 0){
        result = b;
        for (i = 1; i < n; i++){
            result = result * b;
        }
    } else {
        result = 1;
    }

    return result;
}

float negativePot(int b, int n){
    int result, i;

    if (n > 0){
        result = 1;
        for (i = 0; i < n; i++){
            result = result * b;
        }
    } else {
        result = 1;
    }

    return 1/float(result);
}

void waitKeypress(){
    cin.ignore();
    cin.ignore();
}

int asciiToInt(int value){
    if(value > 96){
        value = value - 87;
    } else if (value > 64){
        value = value - 55;
    } else {
        value = value - 48;
    }

    return value;
}

bool checkCorrectBaseInput(int base, string num){
    int i, valueToCheck;
    bool status = true;

    for(i = 0; i < num.length(); i++){

        valueToCheck = asciiToInt(int(num.at(i)));

        if (valueToCheck >= base){
            status = false;
        }
    }

    return status;
}

string calcDecimalNTo10(int n, string decimal){
    int i, result, positionalNumber;
    float aux = 0.0;

    for(i = 0; i < decimal.length(); i++){
        positionalNumber = asciiToInt(int((decimal).at(i)));
        aux += positionalNumber * negativePot(n, i+1);
    }

    result = pot(10, int((float_to_str(aux)).length()) - 2) * aux;

    return to_str(result);
}

string calcDecimal10ToN(int n, string decimal, int precission){

    int i;
    float aux = str_to_float(decimal) / float(pot(10, (decimal.length())));
    string result;

    for(i = 0; i < precission; i++){

        if (int(float(n) * aux) > 9){
            result += char(int(float(n) * aux) + 55);
        } else {
            result += to_str(int(float(n) * aux));
        }
        if (float(n) * aux >= 1.0){
            aux = (float(n) * aux) - int(float(n) * aux);
        } else {
            aux = float(n) * aux;
        }
    }

    return result;
}

string calcBase10(int n, string num){

    int result, i, n1, n2;
    string aux;

    if(n >= 2 && checkCorrectBaseInput(n, num)){

        result = 0;
        for(i = 0; i < num.length(); i++){
            n1 = pot(n, i);
            n2 = asciiToInt(int(num.at(num.length() - i - 1)));
            
            result += n1 * n2;
        }
    } else {
        cout << "Valores fuera de rango." << endl;
    }

    return to_str(result);
}

string calcBaseN(int n, string base10){
    
    string result, aux;
    int num = str_to_int(base10);

    if (num >= 0 && n >= 2){
        if ((num % n) > 9){
            result = char(55 + num % n);
        } else {
            result = to_str(num % n);
        }

        while(num>=n){

            if(num % n == 1){
                num = (num - (n - 1)) / n;
            } else {
                num = num / n;
            }
            if ((num % n) > 9){
                aux = char(55 + num % n);
            } else {
                aux = to_str(num % n);
            }
            result = aux + result;
        }
    } else {
        cout << "Valores fuera de rango..." << endl;
    }

    return result;
}

string calcRestrictedToBase(int n, string num){

    string result;
    int length;
    string cleanNum;
    bool isNegative = false;

    if (num.at(0) == '-'){
        isNegative = true;
    }
    cleanNum = to_str(str_to_int(num));

    if(isNegative){
        length = (calcBaseN(n, to_str(str_to_int(num) * -1))).length();
        cleanNum = to_str(str_to_int(cleanNum) * -1);
        result =  calcBaseN(n, to_str(pot(n, length) - str_to_int(cleanNum) - 1));
    } else {
        result = calcBaseN(n, cleanNum);
    }    

    return result;
}

string calcBaseComplement(int n, string num){
    
        string result;
        int length;
        string cleanNum;
        bool isNegative = false;
    
        if (num.at(0) == '-'){
            isNegative = true;
        }
        cleanNum = to_str(str_to_int(num));
    
        if(isNegative){
            length = (calcBaseN(n, to_str(str_to_int(num) * -1))).length();
            cleanNum = to_str(str_to_int(cleanNum) * -1);
            result =  calcBaseN(n, to_str(pot(n, length) - str_to_int(cleanNum)));
        } else {
            result = calcBaseN(n, cleanNum);
        }    
    
        return result;
}

string calcSlant(int n, string num){
    
    string result, cleanNum;

    if (str_to_int(num) < 0){
        cleanNum = to_str(str_to_int(num) * -1);
        result = "1";
    } else {
        cleanNum = to_str(str_to_int(num));
        result = "0";
    }

    result += calcBaseComplement(n, cleanNum);

    return result;
}

string calcSubstract(int n, string num1, string num2){
    
    string calc = calcBaseN(n, (to_str(str_to_int(calcBase10(n, num1)) - str_to_int(calcBase10(n, num2)))));

    
    return calc;
}

string calcSum(int n, string num1, string num2){
    
    string calc = calcBaseN(n, (to_str(str_to_int(calcBase10(n, num1)) + str_to_int(calcBase10(n, num2)))));

    return calc;
}

string calcIEEE754To10(string num){

    int i;

    string result, aux;
    string sign = num.substr(0, 1);
    string exponent = num.substr(1, 8);
    string mantisa1 = "1";
    string mantisa2 = num.substr(9, 32);

    aux = exponent;

    if(sign == "1"){
        result = "-";
    } else if(sign == "0"){
        result = "+";
    }
    
    exponent = to_str(str_to_int(calcBase10(2, exponent)) - 127);
    
    if (str_to_int(exponent) < 0){
        for(i = 0; i < (str_to_int(exponent) * -1); i++){
            mantisa2 = mantisa1.substr(mantisa1.length() - 1, mantisa1.length()) + mantisa2;
            mantisa1 = mantisa1.substr(0, mantisa1.length());
        }
        if(mantisa1 == ""){
            mantisa1 = "0";
        }
    } else {
        for(i = 0; i < str_to_int(exponent); i++){
            mantisa1 += mantisa2.substr(0, 1);
            mantisa2 = mantisa2.substr(1, mantisa2.length() + 1);
            cout << "sign: " << sign << "\nexponent: " << exponent << "\nmantisa1: " << mantisa1 << "\nmantisa2: " << mantisa2 << endl;
            
        }
    }

    result += calcBase10(2, mantisa1) + "." + calcDecimalNTo10(2, mantisa2);
    
    return result;
}

string calcIEEE754(char sign, string num, string decimal){

    int i;
    string result, binNum, binDec, mantissa, exponent, aux;
    bool canContinue = true;

    if(sign == '-'){
        result = "1";
    } else { //Positivo por defecto
        result = "0";
    }

    binNum = calcBaseN(2, num);
    binDec = calcDecimal10ToN(2, decimal, 23);

    if(str_to_int(num) > 0){

        i = 0;

        while (binNum.length() > 1){
            binDec = binNum.substr(binNum.length() - 1) + binDec;
            binNum = binNum.substr(0, binNum.length() - 1);
            i++;
        }

    } else {
        i = 0;
        
        while (binDec.at(0) == '0'){
            binDec = binDec.substr(1, binDec.length() - 1);
            i--;
        }

        binDec = binDec.substr(1, binDec.length() - 1);
        i--;
    }

    exponent = calcBaseN(2, to_str(127 + i));

    

    if (exponent.length() < calcBaseN(2, to_str(127)).length()){
        
        aux = exponent;
        
        if(i < 0){
            exponent = "1";
        } else {
            exponent = "0";
        }
        while((exponent.length() + aux.length()) < calcBaseN(2, to_str(127)).length()){
            exponent += "0";
        }

        exponent += aux;
    }

    //cout << "\n\nSigno: " << result << "\nExponente: " << exponent.substr(0, 8) << "\nMantisa: " << binDec.substr(0, 23) << endl;

    result += " " + exponent.substr(0, 8) + " " + binDec.substr(0, 23);

    return result;
}

int main (){
    //TODO: (h) para mostrar los comandos disponibles--> Modo bucle

    int n, decimalPrecission, i;
    string num, num2, decimalNum, result;
    char option, suboption, sign;

    while(option != 'e' && option != 'E'){

        cout << 
" ____                         _____      _      \n"
"|  _ \\                       / ____|    | |     \n"
"| |_) | __ _ ___  ___ ______| |     __ _| | ___ \n"
"|  _ < / _` / __|/ _ \\______| |    / _` | |/ __|\n"
"| |_) | (_| \\__ \\  __/      | |___| (_| | | (__ \n"
"|____/ \\__,_|___/\\___|       \\_____\\__,_|_|\\___| \n"
        "\n\n\n"
        "SELECCIONA UNA OPCIÓN:\n\n"
        "1. Conversión de base 10 a base n\n"
        "2. Conversión de base n a base 10\n"
        "3. Conversión decimal\n"
        "4. Suma en base n\n"
        "5. Resta en base n\n"
        "6. Complemento restringido a la base\n"
        "7. Complemento a la base\n"
        "8. Sesgo\n"
        "9. Representación IEEE-754 (32 bits)\n\n"
        "(e) para salir.\n\n"
        "Opción: ";

        cin >> option;

        cout << endl;

        switch (option){
            case '1':
                cout << "Introduce la base: ";
                cin >> n;
                cout << "Introduce el número en base 10: ";
                cin >> num;

                cout << "\nResultado: " << calcBaseN(n, num) << endl;

                waitKeypress();
            break;
            case '2':
                cout << "Introduce la base inicial: ";
                cin >> n;
                cout << "Introduce el numero en dicha base: ";
                cin >> num;

                cout << "\nResultado: " << calcBase10(n, num) << endl;

                waitKeypress();
            break;
            case '3':
                cout << 
                "Selecciona el tipo de conversión:\n"
                "1. De base 10 a base n\n"
                "2. De base n a base 10\n\n"
                "Opcion: ";
                cin >> suboption;
                if(suboption == '1'){
                    cout << "\nIntroduce la base: ";
                    cin >> n;
                    cout << "Introduce el valor entero (sin decimales): ";
                    cin >> num;
                    cout << "Introduce el valor decimal: ";
                    cin >> decimalNum;
                    cout << "Introduce la precisión decimal: ";
                    cin >> decimalPrecission;
                    
                    cout << "\nResultado: " << calcBaseN(n, num) << ".";
                    cout << calcDecimal10ToN(n, decimalNum, decimalPrecission) << endl;

                    waitKeypress();
                } else if (suboption == '2'){
                    cout << "\nIntroduce la base: ";
                    cin >> n;
                    cout << "Introduce el valor entero (sin decimales): ";
                    cin >> num;
                    cout << "Introduce el valor decimal: ";
                    cin >> decimalNum;
                    
                    cout << "\nResultado: " << calcBase10(n, num) << ".";
                    cout << calcDecimalNTo10(n, decimalNum) << endl;

                    waitKeypress();
                } else {
                    cout << "\nNo has seleccionado ninguna opción";
                }
            break;
            case '4':
                cout << "Introduce la base de la operación: ";
                cin >> n;
                cout << "Introduce el primer valor: ";
                cin >> num;
                cout << "Introduce el segundo valor: ";
                cin >> num2;

                if (checkCorrectBaseInput(n, num) && checkCorrectBaseInput(n, num2)){
                    cout << "\n\n";
                    if(num.length() >= num2.length()){
                        cout << "   " << num << "\n + ";
                        for(i = 0; i < (num.length() - num2.length()); i++){
                            cout << " ";
                        }
                        cout << num2 << "\n ";
                        for(i = 0; i < (num.length() + 2); i++){
                            cout << "-";
                        }
                        cout << endl; 
                    } else {
                        cout << "   ";
                        for(i = 0; i < (num2.length() - num.length()); i++){
                            cout << " ";
                        }
                        cout << num << "\n + " << num2 << "\n ";
                        for(i = 0; i < (num2.length() + 2); i++){
                            cout << "-";
                        }
                        cout << endl;
                    }
    
                    cout << "   " << calcSum(n, num, num2);
                } else {
                    cout << "\nValores fuera de rango." << endl;
                }
                

                waitKeypress();
            break;
            case '5':
            cout << "Introduce la base de la operación: ";
            cin >> n;
            cout << "Introduce el primer valor: ";
            cin >> num;
            cout << "Introduce el segundo valor: ";
            cin >> num2;

            if (checkCorrectBaseInput(n, num) && checkCorrectBaseInput(n, num2)){
                cout << "\n\n";
                if(num.length() >= num2.length()){
                    cout << "   " << num << "\n - ";
                    for(i = 0; i < (num.length() - num2.length()); i++){
                        cout << " ";
                    }
                    cout << num2 << "\n ";
                    for(i = 0; i < (num.length() + 2); i++){
                        cout << "-";
                    }
                    result = calcSubstract(n, num, num2);
                    
                    cout << "\n   ";
                    for(i = 0; i < (num.length() - result.length()); i++){
                        cout << " ";
                    }

                    cout << result;
                } else {
                    cout << "   ";
                    for(i = 0; i < (num2.length() - num.length()); i++){
                        cout << " ";
                    }
                    cout << num << "\n - " << num2 << "\n ";
                    for(i = 0; i < (num2.length() + 2); i++){
                        cout << "-";
                    }
                    result = calcSubstract(n, num2, num);

                    cout << "\n  ";
                    for(i = 0; i < (num2.length() - result.length()); i++){
                        cout << " ";
                    }

                    cout << "-" << result;
                }
            } else {
                cout << "\nValores fuera de rango." << endl;
            }
            

            waitKeypress();
            break;
            case '6':
                cout << "Introduce la base: ";
                cin >> n;
                cout << "Introduce el número en base 10 con signo (+/-): ";
                cin >> num;

                cout << "\nResultado: " << calcRestrictedToBase(n, num) << "\nResultado en base 10: ";
                cout << calcBase10(n, calcRestrictedToBase(n, num)) << endl;

                waitKeypress();
            break;
            case '7':
                cout << "Introduce la base: ";
                cin >> n;
                cout << "Introduce el número en base 10 con signo (+/-): ";
                cin >> num;

                cout << "\nResultado: " << calcBaseComplement(n, num) << "\nResultado en base 10: ";
                cout << calcBase10(n, calcBaseComplement(n, num)) << endl;

                waitKeypress();
            break;
            case '8':
                cout << "Introduce la base: ";
                cin >> n;
                cout << "Introduce el número al que aplicar el sesgo: ";
                cin >> num;

                cout << "\nResultado: " << calcSlant(n, num) << endl;

                waitKeypress();
            break;
            case '9':

            cout << "1. Conversión de base 10 a IEEE-754.\n2. Conversión de IEEE-754 a base 10\n\nOpcion: ";
            cin >> suboption;

            cout << endl;

            if(suboption == '1'){
                cout << "Introduce el signo: ";
                cin >> sign;
                cout << "Introduce el número entero en valor absoluto (sin decimales): ";
                cin >> num;
                cout << "Introduce el número decimal: ";
                cin >> decimalNum;

                if(sign == '-' || sign == '+'){
                    cout << "\nResultado: " << calcIEEE754(sign, num, decimalNum) << endl;
                } else {
                    cout << "El signo no es válido.";
                }
            } else if(suboption == '2'){
                cout << "Introduce el numero en IEEE-754 sin espacios: ";
                cin >> num;

                cout << "\nResultado: " << calcIEEE754To10(num) << endl;

            } else {
                cout << "Opción inválida.";
            }


                waitKeypress();
            break;
            default:
                option = 'e';
            break;
        }
    }

    return 0;
}