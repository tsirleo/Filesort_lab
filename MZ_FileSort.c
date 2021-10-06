#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>

struct list
{ 
  char *data;
  struct list *next; 
  struct list *prev; 
};

void Open_file_error(int fd)
{
    if (fd==-1)
    {
        fprintf(stderr, "File opening failure \n");
        close(fd);
        exit(1);
    }
    return;
}

char* Read_string(int fd)
{
	unsigned basis=4;
	unsigned size=0;
	unsigned i=0;
	char ch;
	char *str=(char*)malloc(size+=basis);
	while ((read(fd, &ch, sizeof(ch)))&&(ch!='\n'))
	{
		str[i++]=ch;
		if (i==size)
			str=(char*)realloc(str, size+=basis);
	}
	str[i]='\0';
	if (!strcmp(str, "\0"))
	{
		str=(char*)realloc(str, 2*sizeof(ch));
		strcpy(str, "\n");
		return str;		
	}
	else
	{
		str=(char*)realloc(str, (i+1)*sizeof(ch));
		return str;
	}
}

int Number_of_strings_in_file(char *file_name)
{
	int fd, number = 0;
    char ch;
	fd=open(file_name, O_RDONLY, 0660);
	Open_file_error(fd);
	while (read(fd, &ch, sizeof(ch)))
	{
		if (ch=='\n')
			number+=1;
	}
	close(fd);
	return number;
}

struct list* Make_list_add_element(struct list *Lst, char *str)
{
	struct list *x;
	struct list *head=Lst;
	if ((Lst)==NULL)
	{
		Lst = (struct list*)malloc(sizeof(struct list));
		Lst->data = malloc(strlen(str)+1);
		strcpy(Lst->data, str);
		Lst->next = NULL;
		Lst->prev = NULL;
		return(Lst);
	}
	else
	{
		while (Lst->next!=NULL)
		{
			Lst=Lst->next;
		}
		x = (struct list*)malloc(sizeof(struct list));
		x->data = malloc(strlen(str)+1);
		strcpy(x->data, str);
		x->prev = Lst;
		x->next = Lst->next;
		Lst->next = x;
		if (x->next!= NULL)
			x->next->prev = x;
		return(head);
	}
}

void Delete_list(struct list *Lst)
{
	struct list *x;
	while (Lst!=NULL)
		{
			x=Lst;
			Lst=Lst->next;
			free(x->data);
			free(x);
		}
} 

int Number_of_spaces(char *str)
{
	int k=0;
	int i=0;
	if (str[i++]==' ')
	{
		k+=1;
		while (str[i++]==' ')
			k+=1;
		return k;
	}
	else
		return 0;
}
void Sort_list(struct list **Lst, int flag_r, int flag_b)
{
    if (*Lst == NULL)
        return;	    
    struct list *Lst_copy1 = *Lst, *Lst_copy2, *minElem, *maxElem;
    char *min, *max, *str;
    while ((*Lst)->prev!=NULL)
        (*Lst) = (*Lst)->prev;
    if (!flag_r)
    {
		while (Lst_copy1->next!=NULL)
		{
			Lst_copy2 = Lst_copy1->next;
			min = Lst_copy1->data;
			minElem = Lst_copy1;
			while (Lst_copy2!=NULL)
			{
				if (flag_b)
				{
					if (strcmp(min+Number_of_spaces(min), Lst_copy2->data+Number_of_spaces(Lst_copy2->data)) > 0)
					{
						min = Lst_copy2->data;
						minElem = Lst_copy2;
					}
					Lst_copy2 = Lst_copy2->next;
				}
				else
				{
					if (strcmp(min, Lst_copy2->data) > 0)
					{
						min = Lst_copy2->data;
						minElem = Lst_copy2;
					}
					Lst_copy2 = Lst_copy2->next;					
				}
			}
			str = Lst_copy1->data;
			Lst_copy1->data = minElem->data;
			minElem->data = str;
			Lst_copy1 = Lst_copy1->next;
		}
	}
	else
	{
		while (Lst_copy1->next!=NULL)
		{
			Lst_copy2 = Lst_copy1->next;
			max = Lst_copy1->data;
			maxElem = Lst_copy1;
			while (Lst_copy2!=NULL)
			{
				if (flag_b)
				{
					if (strcmp(max+Number_of_spaces(max), Lst_copy2->data+Number_of_spaces(Lst_copy2->data)) < 0)
					{
						max = Lst_copy2->data;
						maxElem = Lst_copy2;
					}
					Lst_copy2 = Lst_copy2->next;
				}
				else
				{
					if (strcmp(max, Lst_copy2->data) < 0)
					{
						max = Lst_copy2->data;
						maxElem = Lst_copy2;
					}
					Lst_copy2 = Lst_copy2->next;					
				}
			}
			str = Lst_copy1->data;
			Lst_copy1->data = maxElem->data;
			maxElem->data = str;
			Lst_copy1 = Lst_copy1->next;
		}		
	}
}

void Print_list(struct list *Lst, int flag_o, char* file_res_name)
{
	int fd;
	char ch;
	if (flag_o)
	{
		fd=open(file_res_name, O_WRONLY, 0660);
		Open_file_error(fd);
		lseek(fd, 0, SEEK_END);
		while (Lst!=NULL)
		{
			write(fd, Lst->data, (strlen(Lst->data))*sizeof(ch));
			if (!strcmp(Lst->data, "\n"))
			{
				Lst=Lst->next;
			}
			else
			{
				ch='\n';
				write(fd, &ch, sizeof(ch));	
				Lst=Lst->next;
			}
		}
		close(fd);
	}
	else
	{
		printf("------------------------------------------------------------------\n");
		printf("Sorted file: \n");
		while (Lst!=NULL)
		{
			write(1, Lst->data, (strlen(Lst->data))*sizeof(ch));
			if (!strcmp(Lst->data, "\n"))
			{
				Lst=Lst->next;
			}
			else
			{
				ch='\n';
				write(1, &ch, sizeof(ch));	
				Lst=Lst->next;
			}
		}
	}
}

int main(int argc, char **argv)
{
	int fd, help, flag_r=0, flag_b=0, flag_m=0, flag_o=0;
	struct list *Lst=NULL, *Lst_res=NULL, *Lst_head;
	char *file_res_name, *str, ch;
    if (argc<2)
    {
        fprintf(stderr, "Error: Unsufficient number of arguments \n");
        exit(2);
    }
	for(int i=1;i<argc;i++)
	{
		if (!strcmp(argv[i], "-r"))
			flag_r=1;
		else if (!strcmp(argv[i], "-b"))
			flag_b=1;
		else if (!strcmp(argv[i], "-m"))
		{
			flag_m=1;
			help=open("help", O_CREAT|O_TRUNC, 0660);
			Open_file_error(help);
			close(help);
		}
		else if (!strcmp(argv[i], "-o"))
		{
			int flag=0;
			flag_o=1;
			file_res_name=argv[i+=1];
			fd=open(argv[i], O_RDONLY|O_CREAT, 0660);
			Open_file_error(fd);
			for(int j=0;j<Number_of_strings_in_file(argv[i]);j++)
			{
				str=Read_string(fd);
				Lst_res=Make_list_add_element(Lst_res, str);
				free(str);
			}
			close(fd);
			fd=open(file_res_name, O_CREAT|O_TRUNC, 0660);
			Open_file_error(fd);
			close(fd);
			for(int j=1;j<argc;j++)
			{
				if (j!=i)
					if (!strcmp(argv[j], argv[i]))
					{
						flag=1;
						break;
					}		
			}
			if (flag==0)
				Delete_list(Lst_res);
		}
		else 
		{
			if ((flag_o)&&(!strcmp(argv[i], file_res_name)))
			{
				Sort_list(&Lst_res, flag_r, flag_b);
				if (!flag_m)
				{
					Print_list(Lst_res, flag_o, file_res_name);
					Delete_list(Lst_res);
					Lst_res=NULL;
				}
				else
				{
					help=open("help", O_WRONLY, 0660);
					Open_file_error(help);
					lseek(help, 0, SEEK_END);
					Lst_head=Lst_res;
					while (Lst_res!=NULL)
					{
						write(help, Lst_res->data, (strlen(Lst_res->data))*sizeof(ch));
						if (!strcmp(Lst_res->data, "\n"))
						{
							Lst_res=Lst_res->next;
						}
						else
						{
							ch='\n';
							write(help, &ch, sizeof(ch));	
							Lst_res=Lst_res->next;
						}
					}	
					close(help);	
					Delete_list(Lst_head);
					Lst_res=NULL;
				}				
			}
			else
			{
				fd=open(argv[i], O_RDONLY, 0660);
				Open_file_error(fd);
				for(int j=0;j<Number_of_strings_in_file(argv[i]);j++)
				{
					str=Read_string(fd);
					Lst=Make_list_add_element(Lst, str);
					free(str);
				}
				close(fd);
				Sort_list(&Lst, flag_r, flag_b);
				if (!flag_m)
				{
					Print_list(Lst, flag_o, file_res_name);
					Delete_list(Lst);
					Lst=NULL;
				}
				else
				{
					help=open("help", O_WRONLY, 0660);
					Open_file_error(help);
					lseek(help, 0, SEEK_END);
					Lst_head=Lst;
					while (Lst!=NULL)
					{
						write(help, Lst->data, (strlen(Lst->data))*sizeof(ch));
						if (!strcmp(Lst->data, "\n"))
						{
							Lst=Lst->next;
						}
						else
						{
							ch='\n';
							write(help, &ch, sizeof(ch));	
							Lst=Lst->next;
						}
					}	
					close(help);	
					Delete_list(Lst_head);
					Lst=NULL;
				}
			}
		}
	}
	if (flag_m)
	{
		help=open("help", O_RDONLY, 0660);
		Open_file_error(help);
		for(int i=0;i<Number_of_strings_in_file("help");i++)
		{
			str=Read_string(help);
			Lst=Make_list_add_element(Lst, str);
			free(str);
		}
		close(help);
		Sort_list(&Lst, flag_r, flag_b);	
		Print_list(Lst, flag_o, file_res_name);
		Delete_list(Lst);	
		Lst=NULL;
	}
	unlink("help");
	return 0;
}

