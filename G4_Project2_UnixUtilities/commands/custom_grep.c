#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// function for case-insensitive search
int match_ignore_case(char *line,char *pattern)
{
    char l[1024],p[1024];

    int i;
    for(i=0;line[i];i++) l[i]=tolower(line[i]);
    l[i]='\0';

    for(i=0;pattern[i];i++) p[i]=tolower(pattern[i]);
    p[i]='\0';

    return strstr(l,p)!=NULL;
}

int main(int argc,char *argv[])
{
    int ignore_case=0;
    int line_number=0;

    char *pattern,*filename;

    // CASE 1: normal grep
    if(argc==3)
    {
        pattern=argv[1];
        filename=argv[2];
    }
    // CASE 2: with flag
    else if(argc==4)
    {
        if(strcmp(argv[1],"-i")==0)
            ignore_case=1;
        else if(strcmp(argv[1],"-n")==0)
            line_number=1;
        else
        {
            printf("Invalid option\n");
            return 1;
        }

        pattern=argv[2];
        filename=argv[3];
    }
    else
    {
        printf("Usage:\n");
        printf("./custom_grep pattern file\n");
        printf("./custom_grep -i pattern file\n");
        printf("./custom_grep -n pattern file\n");
        return 1;
    }

    FILE *fp=fopen(filename,"r");
    if(fp==NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    char line[1024];
    int line_no=1;

    while(fgets(line,sizeof(line),fp))
    {
        int match=0;

        if(ignore_case)
            match=match_ignore_case(line,pattern);
        else
            match=(strstr(line,pattern)!=NULL);

        if(match)
        {
            if(line_number)
                printf("%d: %s",line_no,line);
            else
                printf("%s",line);
        }

        line_no++;
    }

    fclose(fp);
    return 0;
}