enum LogActions { LIST, SEARCH, WRITE };
void writeLog(FILE *file, char *line);
void listLog(FILE *file);
void searchLog(FILE *file, char *string);
FILE *openFile(char *path);