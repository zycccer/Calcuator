//
// Created by zyc on 2022/2/26.
//
#include "operation.h"
double OperationApply(Operation *operation){
    if(operation->operator){
        return operation->operator(operation->left,operation->left);
    } else{
        return 0.0;
    }
}

double Plus(double left, double right) {
    return left + right;
}

double Minus(double left, double right) {
    return left - right;
}

double Multiply(double left, double right) {
    return left * right;
}

double Divide(double left, double right) {
    if (right == 0.0) {
        return 0;
    } else {
        return left / right;
    }
}
