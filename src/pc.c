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

	fprintf(outfile, "#include <stdlib.h>\n\
#include <stdio.h>\n\
int main(){\n\
unsigned char * ptr = (unsigned char *) calloc(1, sizeof(unsigned char));\n\
long unsigned int size = 1;\n\
long unsigned int acc = 0;\n");

	bool success = true;
	char c = (char) fgetc(infile);
	unsigned long int open_brackets = 0;
	unsigned long int closed_brackets = 0;
	while(c != EOF){

		switch (c){
			case '>':
				fprintf(outfile, "++acc;\n");
				break;
			case '<':
				fprintf(outfile, "--acc;\n");
				break;
			case '+':
				fprintf(outfile, "++*(ptr + acc);\n");
				break;
			case '-':
				fprintf(outfile, "--*(ptr + acc);\n");
				break;
			case '[':
				fprintf(outfile, "while(*(ptr + acc)){\n");
				open_brackets++;
				break;
			case ']':
				fprintf(outfile, "}\n");
				closed_brackets++;
				break;
			
			case '!':
				fprintf(outfile, "putchar(*(ptr + acc));\n");
				break;
			case '?':
				fprintf(outfile, "*(ptr + acc) = getchar();\n");
				break;

			case 'e':
				fprintf(outfile, "unsigned long int new_size= *(ptr + acc);\n\
unsigned char * new = (unsigned char *) calloc(new_size, sizeof(unsigned char));\n\
for(int i = 0; i < size; i++){\n\
*(new + i) = *(ptr + i);\n\
}\n\
free(ptr);\n\
ptr = new;\n\
size = new_size;");
				break;

			case 'r':
				fprintf(outfile, "unsigned char r = *(ptr + acc);\n\
free(ptr);\n\
return r;\n");
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

	fprintf(outfile, "unsigned char r = *ptr;\n\
free(ptr);\n\
return r;\n\
}");

	fclose(infile);
	fclose(outfile);
	return 0;
}