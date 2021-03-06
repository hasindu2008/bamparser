#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include "htslib/sam.h"
#include "common.h"


    
int main(int argc,char** argv){
    
    //check args
    if(argc!=2){
        fprintf(stderr, "Usage %s file.bam \n",argv[0]);
        exit(EXIT_FAILURE);
    }
    
    //these come from htslib/sam.h
	samFile *in = NULL;
	bam1_t *b= NULL;
    bam_hdr_t *header = NULL;

    //open the BAM file (though called sam_open is opens bam files too :P)
    in = sam_open(argv[1], "r");
    errorCheckNULL(in);
    
    //get the sam header. 
    if ((header = sam_hdr_read(in)) == 0){
        fprintf(stderr,"No sam header?\n");
        exit(EXIT_FAILURE);
    }
    //print the chromosome names in the header
    //see the bam_hdr_t struct in htslib/sam.h for parsing the rest of the stuff in header
    int i;
    for(i=0; i< (header->n_targets); i++){
        printf("Chromosome ID %d = %s\n",i,(header->target_name[i]));
    }     
    
    //this must be the initialisation for the structure that stores a read (need to verify)
	b = bam_init1();
    
    //my structure for a read (see common.h)
    struct alignedRead* myread = (struct alignedRead*)malloc(sizeof(struct alignedRead));
    
    //repeat until all reads in the file are retrieved
	while ( sam_read1(in, header, b) >= 0){
        getRead(myread, b);         //copy the current read to the myread structure. See common.c for information
        printRead(myread,header);   //print data in  myread structure. See common.c for information
	}

    
    //wrap up
    free(myread);
	bam_destroy1(b);
	bam_hdr_destroy(header);
	sam_close(in);
    
    return 0;
}