/*
* author:jjm2473
* date:2016-4-18
*/

#ifndef DBF_H_INCLUDED
#define DBF_H_INCLUDED
#include <stdio.h>
#include <stdint.h>

typedef struct YYMMDD{
    int8_t year;
    int8_t month;
    int8_t day;
}YYMMDD_t;//3

typedef struct DBF_META{
    int8_t valid;//0
    YYMMDD_t lastupdate;//1-3
    int32_t records;//4-7
    int16_t header_length;//8-9
    int16_t record_length;//10-11
    char keep[20];
}DBF_META_t;//32

typedef struct DBF_FIELD{
    char name[11];//0-10,NULL terminal string
    char type; //11,CDNLM
    int32_t nouse1; //12-15
    int8_t length; //16
    int8_t dec_count; //17
    char nouse2[14];//18-31
}DBF_FIELD_t;//32

typedef struct DBF_HEADER{
    int field_count;
    DBF_META_t meta;
    DBF_FIELD_t fields[0];//scale
}DBF_HEADER_t;

DBF_HEADER_t * readHeader(FILE*);
size_t writeHeader(FILE*, DBF_HEADER_t *);
char** readRow(FILE*, DBF_HEADER_t *);
void freeRow(char**, DBF_HEADER_t *);
DBF_HEADER_t * newHeader(int field_count);
void fillHeader(DBF_HEADER_t *);
size_t writeRow(FILE*, DBF_HEADER_t *, const char**);
#endif // DBF_H_INCLUDED
