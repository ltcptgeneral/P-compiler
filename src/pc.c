#include <stdio.h>

void usage(){
	printf("Usage: pc <inputfile> <outputfile> <>\n");
}

int main(int argc, char ** argv){

	if(argc != 3){
		usage();
		return 2;
	}

	FILE * infile = fopen(argv[1], "r");
	FILE * outfile = fopen(argv[2], "w");
	if(infile == NULL){
		printf("cannot find %s: No such file or directory or lacks read permission", argv[1]);
	}
	if(outfile == NULL){
		printf("cannot open %s: No such file or directory or lacks write permission", argv[2]);
	}
	if(infile == NULL || outfile == NULL) return 1;

	char c = (char) fgetc(infile);
	unsigned long int open_brackets = 0;
	unsigned long int closed_brackets = 0;
	while(c != EOF){

		switch (c){
			case '>':
				fprintf(outfile, "++ptr;\n");
				break;
			case '<':
				fprintf(outfile, "--ptr;\n");
				break;
			case '+':
				fprintf(outfile, "++*ptr;\n");
				break;
			case '-':
				fprintf(outfile, "--*ptr;\n");
				break;
			case '[':
				fprintf(outfile, "while(*ptr){\n");
				open_brackets++;
				break;
			case ']':
				fprintf(outfile, "}\n");
				closed_brackets++;
				break;
			default:
				break;
		}

		c = fgetc(infile);
	}

	return 0;
}