#include <stdio.h>
#define bool unsigned char
#define true 1
#define false 0

void usage(){
	printf("Usage: pc <inputfile> <outputfile>\n");
}

int main(int argc, char ** argv){

	if(argc != 3){
		usage();
		return 2;
	}

	FILE * infile = fopen(argv[1], "r");
	FILE * outfile = fopen(argv[2], "w");
	if(infile == NULL){
		printf("cannot find %s: No such file or directory or lacks read permission\n", argv[1]);
	}
	if(outfile == NULL){
		printf("cannot open %s: No such file or directory or lacks write permission\n", argv[2]);
	}
	if(infile == NULL || outfile == NULL) return 1;

	bool success = true;
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
			
			case '!':
				fprintf(outfile, "putchar(*ptr);\n");
				break;
			case '?':
				fprintf(outfile, "*ptr = getchar();\n");
				break;

			case '\n':
				break;
			case '\r':
				break;
			case '\t':
				break;
			case ' ':
				break;

			default:
				printf("unknown character: %c\n", c);
				success = false;
				break;
		}

		c = fgetc(infile);
	}

	if(open_brackets != closed_brackets){
		success = false;
		printf("unbalanced brackets, found %lu \"[\" but %lu \"]\"\n", open_brackets, closed_brackets);
	}

	if(success == false){
		fclose(infile);
		fclose(outfile);
		return 1;
	}

	fclose(infile);
	fclose(outfile);
	return 0;
}