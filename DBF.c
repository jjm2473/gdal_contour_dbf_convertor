/*
* author:jjm2473
* date:2016-4-18
*/

#include "DBF.h"
#include <malloc.h>
#include <strings.h>

#define SIZEOFHEADER(field_count) (sizeof(DBF_META_t)+sizeof(DBF_FIELD_t)*(field_count))

/**
* must free return value
*/
DBF_HEADER_t * readHeader(FILE* src){
    int field_count;
    DBF_META_t meta;
    fread(&meta,sizeof(meta),1,src);
    field_count=(meta.header_length-33)/32;
    DBF_HEADER_t * header = (DBF_HEADER_t *)
        malloc(sizeof(int)+SIZEOFHEADER(field_count));
    header->meta = meta;
    header->field_count = field_count;
    fread(&header->fields,sizeof(DBF_FIELD_t),field_count,src);
    fgetc(src);
    return header;
}

size_t writeHeader(FILE* dest, DBF_HEADER_t * header){
    size_t size=fwrite(&header->meta, SIZEOFHEADER(header->field_count), 1, dest);
    fputc(0x0D, dest);
    return size+1;
}

/**
* must free return value
*/
char** readRow(FILE* src, DBF_HEADER_t * header){
    int i,preced;
    char fieldbuffer[255];
    char field_chars[255];
    char** row = (char**)malloc(sizeof(char*)*header->field_count);

    preced=fgetc(src);
    DBF_FIELD_t *p;
    for(i=0;i<header->field_count;++i){
        p=header->fields+i;
        fread(fieldbuffer,p->length,1,src);
        fieldbuffer[p->length]='\0';
        sscanf(fieldbuffer,"%s",field_chars);
        row[i]=strdup(field_chars);
    }
    return row;
}

void freeRow(char** row, DBF_HEADER_t *header){
    int i;
    for(i=0;i<header->field_count;++i){
        free(row[i]);
    }
    free(row);
}

DBF_HEADER_t * newHeader(int field_count){
    DBF_HEADER_t * header = (DBF_HEADER_t *)
        calloc(1, sizeof(int)+SIZEOFHEADER(field_count));
    header->field_count = field_count;
    header->meta.valid = 3;
    header->meta.header_length = SIZEOFHEADER(field_count)+1;
    return header;
}

void fillHeader(DBF_HEADER_t *header){
    int i,record_length=1;
    for(i=0;i<header->field_count;++i){
        record_length += header->fields[i].length;
    }
    header->meta.record_length = record_length;
}

size_t writeRow(FILE* dest, DBF_HEADER_t *header, const char** row){
    int i;
    size_t size=1;
    fputc(0x20, dest);//preced
    for(i=0;i<header->field_count;++i){
        size+=fprintf(dest, "%*s", header->fields[i].length, row[i]);
    }
    fflush(dest);
    return size;
}
