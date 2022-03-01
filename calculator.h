//
// Created by zyc on 2022/2/27.
//

#ifndef UNTITLED5_CALCULATOR_H
#define UNTITLED5_CALCULATOR_H
#include "operation.h"
#define  MAX_CHAR 13
typedef struct {
 char input_buffer[MAX_CHAR +1];
 int  input_buffer_position;
 double result;

 Operation previous_operation;
 Operation current_operation;
 //输出一个字符串
 void (*display_text)(char *);
}CalcContext;

//处理输入
int HandleInput(CalcContext *context,char input_value);

CalcContext *CreateCalcContext();

void DestroyCalcContext(CalcContext **p_Context);

#endif //UNTITLED5_CALCULATOR_H
