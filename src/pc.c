#include <stdio.h>
#include <stdlib.h>
#define bool unsigned char
#define true 1
#define false 0
#define DEFAULT_SIZE 1

struct Symbol {
	char symbol;
	unsigned long int count;
};

struct Stack {
	unsigned long int top;
	unsigned long int size; // capacity
	struct Symbol * arr;
};

void delete(struct Stack * s){
	s->size = 0;
	s->top = 0;
	free(s->arr);
	s->arr = NULL;
}

void push(struct Stack * s, char symbol, bool combine){
	if(s->size == 0){
		s->arr = calloc(DEFAULT_SIZE, sizeof(struct Symbol));
		s->top = 0;
		s->size = 1;
		struct Symbol new;
		new.symbol = symbol;
		new.count = 1;
		s->arr[s->top] = new;
		s->top++;
	}
	else if(s->top == s->size){
		struct Symbol * temp = s->arr;
		unsigned long int old_size = s->size; 
		s->size *= 2;
		s->arr = calloc(s->size, sizeof(struct Symbol));
		for(unsigned long int i = 0; i < old_size; i++){
			s->arr[i] = temp[i];
		}
		free(temp);
		if((s->arr[s->top - 1].symbol == symbol) && combine){
			s->arr[s->top - 1].count++;
		}
		else{
			struct Symbol new;
			new.symbol = symbol;
			new.count = 1;
			s->arr[s->top] = new;
			s->top++;
		}
	}
	else{
		if((s->arr[s->top - 1].symbol == symbol) && combine){
			s->arr[s->top - 1].count++;
		}
		else{
			struct Symbol new;
			new.symbol = symbol;
			new.count = 1;
			s->arr[s->top] = new;
			s->top++;
		}
	}
}

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

	unsigned long int open_brackets = 0;
	unsigned long int closed_brackets = 0;

	char c = (char) fgetc(infile);

	struct Stack s;
	s.top = 0;
	s.size = 0;

	while(c != EOF){
		switch(c){
			case '>':
				push(&s, c, true);
				break;
			case '<':
				push(&s, c, true);
				break;
			case '+':
				push(&s, c, true);
				break;
			case '-':
				push(&s, c, true);
				break;
			case '[':
				push(&s, c, false);
				open_brackets++;
				break;
			case ']':
				push(&s, c, false);
				closed_brackets++;
				break;
			
			case '!':
				push(&s, c, false);
				break;
			case '?':
				push(&s, c, false);
				break;

			case 'e':
				push(&s, c, false);
				break;
			case 'r':
				push(&s, c, false);
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
				fclose(infile);
				fclose(outfile);
				delete(&s);
				return 1;
		}
		
		c = (char) fgetc(infile);
	}

	if(open_brackets != closed_brackets){
		printf("unbalanced brackets, found %lu \"[\" but %lu \"]\"\n", open_brackets, closed_brackets);
		fclose(infile);
		fclose(outfile);
		delete(&s);
		return 1;
	}

	fprintf(outfile, "#include <stdlib.h>\n\
#include <stdio.h>\n\
int main(){\n\
unsigned char * ptr = (unsigned char *) calloc(1, sizeof(unsigned char));\n\
long unsigned int size = 1;\n\
long unsigned int acc = 0;\n");

	c = 0;
	unsigned long int count = 0;

	for(int i = 0; i < s.top; i++){
		c = s.arr[i].symbol;
		count = s.arr[i].count;
		switch (c){
			case '>':
				fprintf(outfile, "acc += %lu;\n", count);
				break;
			case '<':
				fprintf(outfile, "acc -= %lu;\n", count);
				break;
			case '+':
				fprintf(outfile, "*(ptr + acc) += %lu;\n", count);
				break;
			case '-':
				fprintf(outfile, "*(ptr + acc) -= %lu;\n", count);
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
				fprintf(outfile, "unsigned long int new_size = *(ptr + acc) + size;\n\
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
		}
	}

	fprintf(outfile, "unsigned char r = *ptr;\n\
free(ptr);\n\
return r;\n\
}");

	fclose(infile);
	fclose(outfile);
	delete(&s);
	return 0;
}