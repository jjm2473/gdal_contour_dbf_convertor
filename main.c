/*
* author:jjm2473
* date:2016-4-18
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "DBF.h"

const char* CONTOUR="contour";
const char* CONTOUR_EXT="contour_ext";
const char* ELEVATION="elevation";
const char* ELEVATION_MAJOR="elevation_major";
const char* ELEVATION_MEDIUM="elevation_medium";
const char* ELEVATION_MINOR="elevation_minor";

int Major_duration=100;
int Medium_duration=50;

int main()
{
    int print_log=1;
    char** row;
    const char** crow;

    int i,j;
    float field_float;
    int field_int;
    int elefield,ele;

    DBF_FIELD_t *p;

    FILE* src = fopen("contour.dbf","rb");
    FILE* dest = fopen("test.dbf","wb");
    DBF_HEADER_t * header = readHeader(src);

    elefield = -1;
    for(i=0;i<header->field_count;++i){
        p=header->fields+i;
        if(!strncasecmp(p->name,"ELE",3)){
            elefield=i;
            break;
        }
    }
    DBF_HEADER_t * destHeader =
        newHeader(elefield==-1?header->field_count:header->field_count+2);

    if(print_log){
        for(i=0;i<header->field_count;++i){
            p=header->fields+i;
            printf("%s,%c,%d,%d\t",p->name,p->type,p->length,p->dec_count);
        }
        printf("\n");
    }

    for(i=0;i<header->meta.records;++i){
        row=readRow(src, header);
        for(j=0;j<header->field_count;++j){
            p=header->fields+j;

            if(print_log)
                printf("%s\t",row[j]);
            switch(p->type){
            case 'C':

                break;
            case 'N':
                if(p->dec_count)
                    field_float = atof(row[j]);
                else
                    field_int = atoi(row[j]);

                break;
            case 'F':
                field_float = atof(row[j]);

            }

        }
        if(print_log)
            printf("\n");
        freeRow(row, header);
    }

    free(header);
    fclose(dest);
    fclose(src);
    return 0;
}
