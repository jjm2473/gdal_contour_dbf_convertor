/*
* author:jjm2473
* date:2016-4-18
*/
#include <stdio.h>
#include <stdlib.h>
#include "Convertor.h"
void usage(char* prog){
    printf("Convert gdal_contour generated dbf to JOSM import ready dbf.by jjm2473\n\n");
    printf("Usage: %s [<options>] <srcfile> <destfile>\n", prog);
    printf("The options are:\n"
           "\t-l: print log\n"
           "\t-i <Major> <Medium>: Contour level interval,default value is 100 50\n"
           );
}
int main(int argc,char* argv[])
{
    int i;
    int Major_interval=100,Medium_interval=50;
    int print_log=0;
    int print_help=0;
    char *srcfile = NULL;
    char *destfile = NULL;

    if(argc<3){
        usage(argv[0]);
        return -1;
    }

    for(i=1;i<argc;++i){
        if(argv[i][0]=='-'){
            switch(argv[i][1]){
            case 'l':
                print_log=1;
                break;
            case '-':
                if(argv[i][2]!='h')
                    break;
            case 'h':
                print_help=1;
                break;
            case 'i':
                Major_interval = atoi(argv[++i]);
                Medium_interval = atoi(argv[++i]);
                break;
            }
        }else{
            if(srcfile==NULL){
                srcfile=argv[i];
            }else{
                destfile=argv[i];
            }
        }
    }
    if(print_help){
        usage(argv[0]);
    }
    if(destfile==NULL){
        if(print_help){
            return 0;
        }else{
            usage(argv[0]);
            return -1;
        }
    }

    FILE* src = fopen(srcfile,"rb");
    FILE* dest = fopen(destfile,"wb");

    convert(src,dest,Major_interval,Medium_interval,print_log);

    fclose(dest);
    fclose(src);

    return 0;
}
