//
// Created by zyc on 2022/2/26.
//

#ifndef UNTITLED5_OPERATION_H
#define UNTITLED5_OPERATION_H
typedef struct Operation{
    double left;
    double right;
    double (*operator)(double, double);
} Operation;
/**
 *  执行操作 Operation 中的指针函数
 * @param operation
 * @return
 */
double OperationApply(Operation *operation);
/**
 * 真正指针函数有哪几种类型
 */
double Plus(double left, double right);

double Minus(double left, double right);

double Multiply(double left, double right);

double Divide(double left, double right);



#endif //UNTITLED5_OPERATION_H
