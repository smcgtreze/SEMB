#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{

char *values;

system("touch info.csv");

values = "Responder_id,LanguagesWorkedWith\n1,HTML/CSS;Java;JavaScript;Python\n2,C++;HTML/CSS;Python\n3,HTML/CSS\n4,C;C++;C#;Python;SQL\n5,C++;HTML/CSS;Java;JavaScript;Python;SQL;VBA\n6,Java;R;SQL\n7,HTML/CSS;JavaScript\n8,Bash/Shell/PowerShell;C;C++;HTML/CSS;Java;JavaScript;Python;SQL\n9,Bash/Shell/PowerShell;C#;HTML/CSS;JavaScript;Python;Ruby;Rust;SQL;TypeScript;WebAssembly;Other(s):\n10,C#;Go;JavaScript;Python;R;SQL\n11,Other(s):";

FILE *ficheiro;

ficheiro = fopen("info.csv","w");


if(ficheiro==NULL) 
{
    printf("Erro ficheiro csv não existe");

    return -1;
}

fprintf(ficheiro,"%s",values);
  
fclose(ficheiro);

system("python plot.py");
//system("python3 plot.py");  
}