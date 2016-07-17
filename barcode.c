#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BARCODE_LENGTH 16 //default barcode length

#ifdef USE_MALLOC_WRAPPERS
#  include "malloc_wrap.h"
#endif

int main_barcode(int argc, char *argv[]){
	//assume lines are never longer than 512 char
	char line1[512],line2[512];
	char *inputfiles[2]={0,0},*outputfile=0;
	FILE *inputfp1=0,*inputfp2=0,*outputfp=stdout;
	if(argc !=3 && argc !=5){
		fprintf(stderr,"The command is bwa barcode <-o outputFile> R1file R2file\n");
		return 1;
	}
	
	//***********************************************************************
	//parse the command line
	//
	//check for case when there are 3 arguments (i.e. no -o flag)
	//
	//check the case when there are 5 arguments
	//check that there actually is a -o flag and an argument following it
	//********************************************************************
if(argc==3){
	 //argv[0]= barcode
	 //argv[1]= <filename1>
	 //argv[2]= <filename2>
		inputfiles[0]=argv[1];
		inputfiles[1]=argv[2];
	}
	if(argc ==5){
		//argv[0]=barcode		
		//check for -
		int numberofInputfiles=0;
		int i=1;
		while(i<5){
		 if(strcmp(argv[i],"-o") ==0){
				//-o can be anywhere i.e. at end of expression
		  //when I encounter it I will look at the next argument by incrementing by 2
		  //argv[i] is "-o"
		  //argv[i+1] is <outputfile> 
		  //argv[i+2] is <next argument> if i=2 is less than argc which will be checked by while loop
				outputfile=argv[i+1];
				fprintf(stderr,"The output file is %s\n",outputfile);
				i+=2;
			}
			else{
			 //otherwise I assume it is an inputfilename
	   //I keep track of whether it is the first inputfile or second inputfile using an index numberofInputfiles
	   
	   //argv[i] is <inputfile>
	   //argv[i+1] is <next argument> if i+1 is less than argc which will be checked by while loop		
				inputfiles[numberofInputfiles++]=argv[i++];	
			}
	 }
	 if (!inputfiles[0] ||!inputfiles[1] ){
		 fprintf(stderr,"The command is bwa barcode <-o outputFile> R1file R2file\n");
		 return 1;
	 }
	 else{
			fprintf(stderr,"The two input files are %s and %s\n",inputfiles[0],inputfiles[1]);
		}	 
	}
 
	//**********************************************************************
	//open the files
	//use fopen to open streams to the two input files and the output file if one is given
	//check that the open is successful	(i.e. that the file pointers are not 0)			
 //**********************************************************************
 
	inputfp1=fopen(inputfiles[0],"r");
	if(!inputfp1){
	 fprintf(stderr,"unable to open %s\n",inputfiles[0]);
	 exit(0);
	}	
	inputfp2=fopen(inputfiles[1],"r");
	if(!inputfp2){
		fprintf(stderr,"unable to open %s\n",inputfiles[1]);
		exit(0);
	}
	outputfp=(outputfile)? fopen(outputfile,"w") : stdout;
	if(!outputfp){
		exit(0);
	}	
 
 
 //**********************************************************************
 //now print out a new file which is the same as file2 with the exception that
 //the header lines have a colon and the first BARCODE_LENGTH characters of the 
 //sequence line from file 1
 //
 //HINT: this can be done with one while loop using fgets, strlen and fprintf
 //HINT:  use the modulo % operator to figure out which lines are sequence lines
 //**********************************************************************
	int nLines=0;
	while(fgets(line1,sizeof(line1),inputfp1)){
		//look for the second line in set of 4 in the first file
		if(nLines %4 == 1){
			//found it!
			//line1 now contains:<barcode><extra letters>\n\0<junk>
			line1[BARCODE_LENGTH]='\0';
			//line 1 now contains <barcode>\0<xtra letters>\n\0<junk>
			//the string commands will see <barcode>
			
			//now let's print out 4 lines from the second file with the extra barcode I have in line1
   //get the first line - the line we need to add the barcode to
		 fgets(line2,sizeof(line2),inputfp2);
		 //replace the '\n' with ':'
		 line2[strlen(line2)-1]=':';
		 fprintf(outputfp,"%s%s\n",line2,line1);//added line feed using the format from fprintf
		 fprintf(outputfp,"%s",fgets(line2,sizeof(line2),inputfp2)); //fgets return value is line2
		 fprintf(outputfp,"%s",fgets(line2,sizeof(line2),inputfp2));
		 fprintf(outputfp,"%s",fgets(line2,sizeof(line2),inputfp2));
		 //can also do this with a fprintf and 3 fputs which is slightly faster
		 //i.e. fputs(line2,outputfp,fgets(line2,inputfp));
		}
		nLines++;
	}
	fclose(inputfp1);
	fclose(inputfp2);
	if(outputfp !=stdout)fclose(outputfp);
	return 0;
}
