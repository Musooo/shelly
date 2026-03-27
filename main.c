#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int append_string(char ***argv, char *string, int *size)
{
	*argv = realloc(*argv, (*size+1)*sizeof(char *));
	(*argv)[*size] = string;
	(*size)++;
	return 0;
}

int create_args_from_line(char *string, char ***argv, char split)
{
	int count = 0;
	//pointer to the start of the word
	char *start = string;
	while (*string!='\0')
        {
                if (*string == split || *string == '\n')
                {
                        *string='\0';
			append_string(argv, start, &count);
			start=string+1;
                }

                string++;
        }
	append_string(argv, NULL, &count);

	//remember to see if the last arg is appended

	return count;
}

char * get_line(void) 
{
	char *line = malloc(100);
	char *start_line = line;
	size_t lenmax = 100;
	size_t len = lenmax;

	int curr;

	if (line==NULL)
		return NULL;

	for(;;)
	{
		curr = fgetc(stdin);
		if (curr == EOF)
			break;
		if(--len==0)
		{
			len = lenmax;
			char * linen = realloc(start_line, lenmax *= 2);
			if (linen==NULL)
			{
				free(start_line);
				return NULL;
			}
			line = linen + (line - start_line);
			start_line = linen;
		}
		if((*line++ = curr) == '\n')
			break;
	}

	*line = '\0';
	return start_line;
}

int main(void)
{
	for (;;) 
	{
		//scan input
		printf("shell>");
		char* line = get_line();

		// creazione del processo figlio
		int rc = fork();
		if (rc<0) 
		{
			fprintf(stderr, "failed\n");
			exit(1);
		}
		else if (rc == 0)
		{
			//processo figlio
			//printf("%s\n", line);
			char **argv = NULL;
			create_args_from_line(line, &argv, ' ');
			execvp(argv[0], argv);
		}
		else
		{
			int rc_wait = wait(NULL);
		}


		//temp way to exit
		if (line == " ")
			break;
		
		free(line);
	}

	return 0;
}

