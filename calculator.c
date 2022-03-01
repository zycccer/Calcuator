////
//// Created by zyc on 2022/2/27.
////
//
//#include "calculator.h"
//#include "stdlib.h"
//#include <string.h>
//#include <stdio.h>
///**
// *
// * @param context
// * @param input_value
// * @return 判断输入值是否合法
// */
//int HandleInput(CalcContext *context,char input_value){
//    // 这中间调用了一个函数了  啦啦啦啦
//    if (context->display_text){
//        char  output[2] = {input_value};
//        context->display_text(output);
//    }
//    return 1;
//}
//
///**
// * 限制其作用域只在定义该变量的源文件内有效，而在同一源程序的其他源文件中不能使用。这时，就可以通过在全局变量之前加上关键字 static 来实现，使全局变量被定义成为一个静态全局变量
// */
//static void ClearBuffer(CalcContext *context) {
//    context->input_buffer[0] = 0;
//    context->input_buffer_position = 0;
//}
///** memset
// *  str -- 指向要填充的内存块。
//    c -- 要被设置的值。该值以 int 形式传递，但是函数在填充内存块时是使用该值的无符号字符形式。
//    n -- 要被设置为该值的字符数。
// * @param calcContext
// */
//static void ClearAll(CalcContext *calcContext){
//    ClearBuffer(calcContext);
//    calcContext->result = 0;
//    memset(&calcContext->previous_operation, 0, sizeof(Operation));
//    memset(&calcContext->current_operation, 0, sizeof(Operation));
//}
//CalcContext *CreateCalcContext(){
//    CalcContext *calcContext = malloc(sizeof (CalcContext));
//    ClearAll(calcContext);
//    calcContext->display_text = NULL;
//    return calcContext;
//
//}
//
//void DestroyCalcContext(CalcContext **p_Context){
//    free(*p_Context);
//    *p_Context = NULL;
//}

#include "calculator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void ClearBuffer(CalcContext *context) {
    context->input_buffer[0] = 0;
    context->input_buffer_position = 0;
}

static void ClearAll(CalcContext *context) {
    ClearBuffer(context);
    context->result = 0;
    memset(&context->previous_operation, 0, sizeof(Operation));
    memset(&context->current_operation, 0, sizeof(Operation));
}

static void FormatOutput(CalcContext *context) {

    if (context->result == -0.0) {
        context->result = 0.0;
    }
    sprintf(context->input_buffer, "%.6g", context->result);
}

static void DisplayOutput(CalcContext *context) {
    FormatOutput(context);
    context->display_text(context->input_buffer);
    ClearBuffer(context);
}

static void FormatInput(CalcContext *context) {
    //如果是什么都没输入 那就显示0 操作位置之后到第一位 偏移量1
    if (context->input_buffer_position == 0) {
        context->input_buffer[0] = '0';
        context->input_buffer[1] = 0;
        context->input_buffer_position = 1;
    } else if (context->input_buffer_position == 1) {
        //判断如果是小数点怎么搞 是小数点   0.
        if (context->input_buffer[0] == '.') {
            context->input_buffer[0] = '0';
            context->input_buffer[1] = '.';
            context->input_buffer[2] = 0;
            context->input_buffer_position = 2;
        }
    } else if (context->input_buffer_position > 1) {

        char *p = context->input_buffer;
        if (p[0] == '0' && p[1] != '.') {
            p[0] = p[1];
            p[1] = 0;
            //清除多余的 0
            context->input_buffer_position = 1;
        } else if(p[context->input_buffer_position - 1] == '.') {
            for (int i = 0; i < context->input_buffer_position - 1; ++i) {
                if (p[i] == '.') {
                    //清楚多余输入 的点
                    context->input_buffer_position--;
                    p[context->input_buffer_position] = 0;
                    break;
                }
            }
        }
    }
}

static void DisplayInput(CalcContext *context) {
    FormatInput(context);
    context->display_text(context->input_buffer);
}

static void ApplyInput(CalcContext *context, double *operand) {
    if (context->input_buffer_position > 0) {
        *operand = atof(context->input_buffer);
        ClearBuffer(context);
    } else {
        *operand = context->result;
    }
}

static void ApplyInputToLeft(CalcContext *context) {
    // 把前面输入的数字保存进左值
    ApplyInput(context, &context->current_operation.left);
}

static void ApplyInputToRight(CalcContext *context) {
    ApplyInput(context, &context->current_operation.right);
}

static void ApplyPreviousOperation(CalcContext *context) {
    context->result = OperationApply(&context->previous_operation);
}

static void ApplyCurrentOperation(CalcContext *context) {
    // 调用前面 operation 的通用构造方法  传入地址 就可以选择性的去 选择方法 而不是调用N个方法
    context->result = OperationApply(&context->current_operation);
    memcpy(&context->previous_operation, &context->current_operation, sizeof(Operation));
    memset(&context->current_operation, 0, sizeof(Operation));
}

int HandleInput(CalcContext *context, char input_value) {
    switch (input_value) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.': {
            if (context->input_buffer_position < MAX_CHAR) {
                context->input_buffer[context->input_buffer_position++] = input_value;
                context->input_buffer[context->input_buffer_position] = '\0';
            }
            //如果没有过操作数  就展示对应的数据
            DisplayInput(context);
            break;
        }
            //如果是加法  吧函数回调给 函数
        case '+': context->current_operation.operator = Plus;
            ApplyInputToLeft(context);
            break;
        case '-':context->current_operation.operator = Minus;
            ApplyInputToLeft(context);
            break;
        case '*':context->current_operation.operator = Multiply;
            ApplyInputToLeft(context);
            break;
        case '/':context->current_operation.operator = Divide;
            ApplyInputToLeft(context);
            break;
        case '=':
            //如果前面没有运算符 就输出No operation
            if (context->current_operation.operator) {
                ApplyInputToRight(context);
                ApplyCurrentOperation(context);
                DisplayOutput(context);
            } else if (context->previous_operation.operator) {
                context->previous_operation.left = context->result;

                ApplyPreviousOperation(context);
                DisplayOutput(context);
            } else {
                printf("No operation.\n");
            }
            break;
        case '%':
            ApplyInputToLeft(context);
            context->current_operation.operator = Multiply;
            context->current_operation.right = 0.01;

            ApplyCurrentOperation(context);
            DisplayOutput(context);
            break;
        case 'S':
            ApplyInputToLeft(context);
            context->current_operation.operator = Multiply;
            context->current_operation.right = -1;

            ApplyCurrentOperation(context);
            DisplayOutput(context);
            break;
        case 'C':
            ClearAll(context);
            DisplayInput(context);
            break;
    }
    return 1;
}

CalcContext *CreateCalcContext(){
    CalcContext *calc_context = malloc(sizeof(CalcContext));
    ClearAll(calc_context);
    calc_context->display_text = NULL;
    return calc_context;
}

void DestroyCalcContext(CalcContext **p_context){
    free(*p_context);
    *p_context = NULL;
}
