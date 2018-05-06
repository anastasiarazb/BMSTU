%option noyywrap bison-bridge bison-locations

%{

#include <stdio.h>
#include <stdlib.h>

#define TAG_NUMBER 1
#define TAG_REGULAR_STRING 2
#define TAG_LITERAL_STRING 3


char *tag_names[] =
{
    "END_OF_PROGRAM",
    "NUMBER",
    "REGULAR_STRING",
    "LITERAL_STRING"
};

struct Position
{
    int line, pos, index;
};

void print_pos(struct Position *p)
{
    printf("(%d, %d)", p->line, p->pos);
}

struct Fragment
{
    struct Position starting, following;
};

typedef struct Fragment YYLTYPE; //Тип коордиат лексемы yylloc

void print_frag(struct Fragment *f)
{
    print_pos(&(f->starting));
    printf(" - ");
    print_pos(&(f->following));
}

union Token
{
    char *number;
    char *regular_string;
    char *literal_string;
};

typedef union Token YYSTYPE; //Тип атрибутов токена yylval

int continued;
struct Position cur;

//Действие, совершаемое после каждого вызова
//yylloc - координаты лексемы, тип YYLTYPE  
#define YY_USER_ACTION                  \
{                                       \
    int i;                              \
    if (!continued)                     \
        yylloc->starting = cur;         \
    continued = 0;                      \
    for (i = 0; i < yyleng; i++) {      \
        if ('\n' == yytext[i]) {        \
            cur.line++;                 \
            cur.pos = 1;                \
        }                               \
        else                            \
            cur.pos++;                  \
        cur.index++;                    \
    }                                   \
    yylloc->following = cur;            \
}                                       \

void init_scanner(char *program)
{
    continued = 0;
    cur.line = 1;
    cur.pos = 1;
    cur.index = 0;
    yy_scan_string(program);
}

void err(char *msg)
{
/* Бандитизм : ошибки нужно класть в список ошибок . */
    printf("Error ");
    print_pos(&cur);
    printf(": %s\n", msg);
}

%}

/*NUMBER          (0|1+)/([\n\t ]+|<<EOF>>) */
NUMBER          (0|1+)
REGULAR_STRING \"([^\"\n\\]|\\\"|\\t|\\n)*\"
LITERAL_STRING @\"([^\"]|\"\")*\"

%%

[\n\t ]+

{NUMBER}      { //yytext - [in] Строка, представляющая лексему.
                //yylval - [out] Атрибуты возвращаемого токена.

                  yylval->number = yytext;
                  return TAG_NUMBER;

              }

{REGULAR_STRING}      {
                        yylval->regular_string = yytext;
                        return TAG_REGULAR_STRING;
                      }

{LITERAL_STRING}  {
                    yylval->literal_string = yytext;
                    return TAG_LITERAL_STRING;
                  }
<<EOF>>       return 0;

.             err("Unexpected character");

%%

int main(int argc, char* argv[])
{
    int tag;
    YYSTYPE value;
    YYLTYPE coords;

    FILE *f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END); // seek to end of file http://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    int length = ftell (f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file
    char * buffer = malloc(length);
    fread(buffer, 1, length, f);
    fclose(f);

    init_scanner(buffer);

    do {
        tag = yylex(&value, &coords);
        if (tag != 0)
        {
            printf("%s ", tag_names[tag]);
            print_frag(&coords);

            switch (tag)
            {
            case TAG_NUMBER:
                printf(" %s\n", value.number);
                break;
            case TAG_REGULAR_STRING:
                printf(" %s\n", value.regular_string);
                break;
            case TAG_LITERAL_STRING:
                printf(" %s\n", value.literal_string);
                break;
            }
        }
    } while (tag != 0);

    free(buffer);
    return 0;
}
