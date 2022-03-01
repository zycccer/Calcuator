//
// Created by zyc on 2022/2/27.
//

#ifndef UNTITLED5_UI_H
#define UNTITLED5_UI_H
#include "calculator.h"
/**
 * void *text_displayer;  指定往哪里输出！
 */
typedef struct {
    void *text_displayer;
    CalcContext *context;
} UiContext;

#endif //UNTITLED5_UI_H
