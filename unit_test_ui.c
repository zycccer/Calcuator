//
// Created by zyc on 2022/2/27.
//

#include "unit_test_ui.h"
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

UiContext *ui_context;
static void DisplayTextToBuffer(char *text){
    //text 写入数组
    //rintf指的是字符串格式化命令，
    // 函数声明为 int sprintf(char *string, char *format [,argument,...]);，
    // 主要功能是把格式化的数据写入某个字符串中
    sprintf(ui_context->text_displayer, text);
}

static void UnitTestMain() {
    FILE *case_file = fopen("cases.txt", "r");
    if (case_file) {
        char key;
        char output_result[MAX_CHAR + 1];
        while (fscanf(case_file, " %c,%s", &key, output_result) != EOF) {
            if (HandleInput(ui_context->context, key)) {
                if (strcmp(output_result, ui_context->text_displayer) == 0) {
                    printf("Input [%c]: Output: %s, OK\n", key, output_result);
                } else {
                    printf("Input [%c]: Output: %s, Expect: %s, ERROR\n", key, ui_context->text_displayer, output_result);

                }
            }
        }

        fclose(case_file);
    } else {
        perror("open case file");
    }
}
static void InitUnitTest() {
    char *const KOutputBuffer = malloc(MAX_CHAR+1);
    ui_context = malloc(sizeof (UiContext));
    ui_context->text_displayer = KOutputBuffer;
    ui_context->context = CreateCalcContext();
    ui_context->context->display_text = DisplayTextToBuffer;

}

static void DestroyUnitTest(){
    DestroyCalcContext(&ui_context->context);
    free(ui_context->text_displayer);
    free(ui_context);
    ui_context = NULL;
}
int RunUnitTest(int argc, char *argv[]){
    InitUnitTest();
    UnitTestMain();
    DestroyUnitTest();
    return 0;
}
