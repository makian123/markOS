#include "lib/char.h"
#include "lib/malloc.h"
#include "lib/terminal.h"

uint8_t get_ascii_char(uint8_t key_code, bool caps)
{
    uint8_t output;
    switch(key_code){
        case KEY_A :
		output = 'a';
		break;
        case KEY_B :
		output = 'b';
		break;
        case KEY_C :
		output = 'c';
		break;
        case KEY_D :
		output = 'd';
		break;
        case KEY_E :
		output = 'e';
		break;
        case KEY_F :
		output = 'f';
		break;
        case KEY_G :
		output = 'g';
		break;
        case KEY_H :
		output = 'h';
		break;
        case KEY_I :
		output = 'i';
		break;
        case KEY_J :
		output = 'j';
		break;
        case KEY_K :
		output = 'k';
		break;
        case KEY_L :
		output = 'l';
		break;
        case KEY_M :
		output = 'm';
		break;
        case KEY_N :
		output = 'n';
		break;
        case KEY_O :
		output = 'o';
		break;
        case KEY_P :
		output = 'p';
		break;
        case KEY_Q :
		output = 'q';
		break;
        case KEY_R :
		output = 'r';
		break;
        case KEY_S :
		output = 's';
		break;
        case KEY_T :
		output = 't';
		break;
        case KEY_U :
		output = 'u';
		break;
        case KEY_V :
		output = 'v';
		break;
        case KEY_W :
		output = 'w';
		break;
        case KEY_X :
		output = 'x';
		break;
        case KEY_Y :
		output = 'y';
		break;
        case KEY_Z :
		output = 'z';
		break;
        case KEY_1 :
		output = '1';
		break;
        case KEY_2 :
		output = '2';
		break;
        case KEY_3 :
		output = '3';
		break;
        case KEY_4 :
		output = '4';
		break;
        case KEY_5 :
		output = '5';
		break;
        case KEY_6 :
		output = '6';
		break;
        case KEY_7 :
		output = '7';
		break;
        case KEY_8 :
		output = '8';
		break;
        case KEY_9 :
		output = '9';
		break;
        case KEY_0 :
		output = '0';
		break;
        case KEY_MINUS :
		output = '-';
		break;
        case KEY_EQUAL :
		output = '=';
		break;
        case KEY_SQUARE_OPEN_BRACKET :
		output = '[';
		break;
        case KEY_SQUARE_CLOSE_BRACKET :
		output = ']';
		break;
        case KEY_SEMICOLON :
		output = ';';
		break;
        case KEY_BACKSLASH :
		output = '\\';
		break;
        case KEY_COMMA :
		output = ',';
		break;
        case KEY_DOT :
		output = '.';
		break;
        case KEY_FORESLHASH :
		output = '/';
		break;
        case KEY_SPACE :
		output = ' ';
		break;
        default :
		output = null;
		break;
    }

    if(caps && output >= 'a' && output <= 'z'){
        output -= 32;
    }

    return output;
}