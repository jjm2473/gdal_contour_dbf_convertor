/*
* author:jjm2473
* date:2016-4-18
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "DBF.h"
#include "Convertor.h"

const char* CONTOUR="contour";
const char* CONTOUR_EXT="contour_ex";
const char* ELEVATION="elevation";
const char* ELEVATION_MAJOR="elevation_major";
const char* ELEVATION_MEDIUM="elevation_medium";
const char* ELEVATION_MINOR="elevation_minor";

int convert(FILE *src,FILE *dest,int Major_interval,int Medium_interval,int log){
    int i,j;

    char** row;
    const char** crow;

    float field_float;
    int field_int;
    int elefield;
    int ele;
    char elestr[12];

    DBF_HEADER_t * header;
    DBF_FIELD_t *p;

    header = readHeader(src);

    elefield = -1;
    for(i=0;i<header->field_count;++i){
        p=header->fields+i;
        if(!strncasecmp(p->name,"ELE",4)){
            elefield=i;
            break;
        }
    }

    if(log){
        for(i=0;i<header->field_count;++i){
            p=header->fields+i;
            printf("%s,%c,%d,%d\t",p->name,p->type,p->length,p->dec_count);
        }
        printf("\n");
    }

    DBF_HEADER_t * destHeader =
        newHeader(elefield==-1?header->field_count:header->field_count+2);

    for(i=0;i<header->field_count;++i){
        destHeader->fields[i]=header->fields[i];
    }
    if(elefield != -1){
        strncpy(destHeader->fields[elefield].name,"ele",3);
        destHeader->fields[elefield].dec_count=0;
        destHeader->fields[elefield].length=8;

        strcpy(destHeader->fields[header->field_count].name, CONTOUR);
        destHeader->fields[header->field_count].type='C';
        destHeader->fields[header->field_count].length=9;

        strcpy(destHeader->fields[header->field_count+1].name, CONTOUR_EXT);
        destHeader->fields[header->field_count+1].type='C';
        destHeader->fields[header->field_count+1].length=16;
    }
    destHeader->meta.records = header->meta.records;
    destHeader->meta.lastupdate = header->meta.lastupdate;
    fillHeader(destHeader);
    writeHeader(dest, destHeader);

    crow=(const char**)malloc(sizeof(const char*)*destHeader->field_count);

    for(i=0;i<header->meta.records;++i){
        row=readRow(src, header);

        for(j=0;j<header->field_count;++j){
            p=header->fields+j;

            if(log)
                printf("%s\t",row[j]);

            crow[j]=row[j];

            switch(p->type){
            case 'C':

                break;
            case 'N':
                if(p->dec_count)
                    field_float = atof(row[j]);
                else
                    field_int = atoi(row[j]);
                if(elefield==j){
                    if(p->dec_count)
                        ele=field_float;
                    else
                        ele=field_int;
                }
                break;
            case 'F':
                field_float = atof(row[j]);

            }

        }

        if(log)
            printf("\n");

        if(elefield!=-1){
            crow[elefield]=itoa(ele,elestr,10);
            crow[header->field_count]=ELEVATION;

            if(ele%Major_interval==0){
                crow[header->field_count+1]=ELEVATION_MAJOR;
            }else if(ele%Medium_interval==0){
                crow[header->field_count+1]=ELEVATION_MEDIUM;
            }else{
                crow[header->field_count+1]=ELEVATION_MINOR;
            }
        }
        writeRow(dest,destHeader,crow);
        freeRow(row, header);
    }
    free(crow);
    free(destHeader);
    free(header);

    return 0;
}
