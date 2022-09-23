#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <glob.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>

#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
// https://github.com/rbtylee/tutorial-jsonc/blob/master/src/json-parse01.c
#include <json-c/json.h>

#include "../../include/h_error.h"
#include "../../include/h_prompt.h"

// https://docs.docker.com/engine/reference/commandline/logs/#options

// https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
// https://stackoverflow.com/a/23657072
// https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
// https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
// https://stackoverflow.com/a/3219459

#define UNDERLINE "\x1B[4;37m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define RESET "\x1B[0m"

#define LOG_LINE_MAX 2024
#define LOG_PATH_MAX 200

struct Container {
    char id[65];
    char name[65];
    char logPath[LOG_PATH_MAX];
};

struct Containers {
    struct Container *containers;
    size_t size;
};

void prompt();
struct Containers listContainers();
struct Container findContainer(char *name);
void showLog(struct Container container, bool showTimestamps, bool followLog, int sinceSeconds);

void printContainers();
void printContainer(struct Container container);
void printJsonLine(char *line, bool showTimestamps);

FILE *openFile(char *path);
void listLog(FILE *file, struct Container container, bool showTimestamps, bool followLog, int sinceSeconds);
void searchLog(struct Container container, char *string, bool showTimestamps, bool followLog, int sinceSeconds);
static const char *readableFileSize(double size);
void sizeLogFile(struct Container container);
int glob_warn(const char *path, int _);

int sinceToSeconds(char *since);
int findOptLongByVal(char value, struct option *optOptions, int sizeOptOptions);
int countLines(FILE *file);
time_t getJsonTime(char *line);
int binarySearchDate(FILE *file, int sinceSeconds, struct Container container);

// https://stackoverflow.com/questions/25580069/fseek-to-a-line-number-with-lines-of-variable-length
// https://stackoverflow.com/questions/28863676/how-to-find-in-a-sorted-list-of-dates-where-a-specific-date-lies-in-between-in
int main(int argc, char *argv[]) {

    if (argc <= 1) {
        prompt();
        return EXIT_SUCCESS;
    }

    bool showFullLog = 0;
    bool followLog = 0;
    bool showTimestamps = 0;
    char *containerName = NULL;
    char *since = NULL;
    char *find = NULL;
    int command;
    opterr = 0;
    int option_index = 0;

    struct option longOptions[] = {
        {"container-name", required_argument, NULL, 'c'},
        {"help", no_argument, NULL, 'h'},
        {"search", required_argument, NULL, 's'},
        {"since", required_argument, NULL, 'n'},
        {"show", no_argument, NULL, 'v'},
        {"follow", no_argument, NULL, 'f'},
        {"timestamps", no_argument, NULL, 't'},
        {NULL, 0, NULL, 0},
    };

    while ((command = getopt_long(argc, argv, "fthc:s:", longOptions, &option_index)) != -1) {
        switch (command) {
            case 0: {
                /* If this option set a flag, do nothing else now. */
                if (longOptions[option_index].flag != 0) {
                    break;
                }
                printf("option %s", longOptions[option_index].name);
                if (optarg) {
                    printf(" with arg %s", optarg);
                }
                printf("\n");
                break;
            }
            case 'n':
                since = optarg;
                break;
            case 'c':
                containerName = optarg;
                break;
            case 's':
                find = optarg;
                break;
            case 'v':
                showFullLog = 1;
                break;
            case 'f':
                followLog = 1;
                break;
            case 't':
                showTimestamps = 1;
                break;
            case 'h':
                printf("Usage: %s [OPTIONS]\n", basename(argv[0]));
                printf("Options:\n");
                printf("  -c, --container-name <container-name>\n");
                printf("  -s, --search <search-string>\n");
                printf("  -v, --show Show full log\n");
                printf("      --since <search-string-time> Show logs since timestamp (e.g. 2022-06-18 17:51:37) or "
                       "relative (e.g. 42m for 42 minutes)\n");
                printf("  -f, --follow Follow log output\n");
                printf("  -t, --timestamps Show timestamps\n");
                printf("  -h, --help\n");
                return EXIT_SUCCESS;
            case '?':
                if (optopt == 'c' || optopt == 's' || optopt == 'n')
                    fprintf(stderr, "Option --%s| requires an argument.\n", longOptions[findOptLongByVal(optopt, longOptions, 7)].name);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option|argument '%d'.\n", optopt);
                return EXIT_FAILURE;
            default:
                abort();
        }
    }

    if (containerName == NULL) {
        die("container name option '--container-name' is required");
    }

    int sinceSeconds = -1;
    if (since != NULL) {
        sinceSeconds = sinceToSeconds(since);
        if (sinceSeconds == -1) {
            die("invalid time format for option '--since': %s", since);
        }
    }

    struct Container container = findContainer(containerName);

    if (showFullLog == TRUE) {
        showLog(container, showTimestamps, followLog, sinceSeconds);
    }

    if (find != NULL) {
        printf("find\n");
        searchLog(container, find, showTimestamps, followLog, sinceSeconds);
    }

    printContainer(container);
    return EXIT_SUCCESS;
}

void prompt() {

    printf(UNDERLINE "List Containers:\n" RESET);
    printf(BLUE "Note: Only the containers with the generated log file are showed:\n\n" RESET);
    struct Containers containers = listContainers();
    int i;
    for (i = 0; i < containers.size; i++) {
        printf(RED "%i)" RESET " %s\n", i + 1, containers.containers[i].name);
    }
    printf(BLUE);
    int containerNumber = getInt("Enter the container number:");
    printf(RESET);
    struct Container container = containers.containers[containerNumber - 1];
    free(containers.containers);

    printf("List action for container '%s':\n", container.name);
    printf("1) Info\n");
    printf("2) Show full log\n");
    printf("3) Search\n");
    printf(BLUE);
    int action = getInt("Enter the action number:");
    printf(RESET);

    switch (action) {
        case 1:
            printContainer(container);
            break;
        case 2:
            showLog(container, FALSE, FALSE, -1);
            break;
        case 3: {
            char *search = getString("Enter the string for search:");
            searchLog(container, search, FALSE, FALSE, -1);
            free(search);
            break;
        }
        default:
            die("Invalid action");
    }
}
void showLog(struct Container container, bool showTimestamps, bool followLog, int sinceSeconds) {

    FILE *logFile = openFile(container.logPath);
    listLog(logFile, container, showTimestamps, followLog, sinceSeconds);
}

void printContainers() {

    struct Containers containers = listContainers();
    int i;

    for (i = 0; i < containers.size; i++) {
        printf("ID: %s\n", containers.containers[i].id);
        printf("Name: %s\n", containers.containers[i].name);
        printf("LogPath: %s\n\n", containers.containers[i].logPath);
    }

    printf("N.º containers: %lu\n\n", containers.size);

    free(containers.containers);
}

void printJsonLine(char *line, bool showTimestamps) {
    json_object *jsonLine = json_tokener_parse(line);
    /* json_object_object_foreach(jsonLine, key, val)
    printf("%s ", json_object_get_string(val)); */
    if (jsonLine != NULL) {
        json_object *logLine;
        json_object_object_get_ex(jsonLine, "log", &logLine);
        if (showTimestamps == TRUE && logLine != NULL) {
            json_object *time;
            json_object_object_get_ex(jsonLine, "time", &time);
            // printf("%s - ", json_object_get_string(time));

            struct tm logTime;
            strptime(json_object_get_string(time), "%Y-%m-%dT%H:%M:%S", &logTime);

            char datetime[200];
            strftime(datetime, sizeof(datetime) + 1, "%Y-%m-%d %T", &logTime);
            printf(RED "%s" RESET " - ", datetime);
        }
        if (logLine != NULL) {
            printf(GREEN "%s\n" RESET, json_object_get_string(logLine));
        }
    } else {
        printf(GREEN "%s\n" RESET, line);
    }
    json_object_put(jsonLine);
}

void printContainer(struct Container container) {

    printf("\n");
    printf(MAGENTA "ID:" GREEN " %s\n" RESET, container.id);
    printf(MAGENTA "Name:" GREEN " %s\n" RESET, container.name);
    printf(MAGENTA "LogPath:" GREEN " %s\n" RESET, container.logPath);
    sizeLogFile(container);
}

void sizeLogFile(struct Container container) {

    FILE *logFile = openFile(container.logPath);
    fseek(logFile, 0, SEEK_END);
    double sizeBytes = ftell(logFile);
    fseek(logFile, 0, SEEK_SET);
    fclose(logFile);
    printf(MAGENTA "Log Size:" GREEN " %s\n" RESET, readableFileSize(sizeBytes));
}

static const char *readableFileSize(double size) {
    int i = 0;
    const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }
    static char output[200];
    sprintf(output, "%.2f %s", size, units[i]);
    return output;
}

struct Container findContainer(char *name) {

    struct Containers containers = listContainers();
    struct Container container = {};
    bool find = false;
    int i;

    for (i = 0; i < containers.size; i++) {
        if (strcmp(name, containers.containers[i].name) == 0) {
            find = true;
            container = containers.containers[i];
            break;
        }
    }
    free(containers.containers);
    if (!find) {
        die("Container not found for %s\n", name);
    }

    return container;
}

struct Containers listContainers() {

    glob_t containersDirectories;
    size_t i, count = 1;
    struct Container *containers = malloc(sizeof(struct Container));

    // sudo journalctl -u docker.service
    // sudo cat /var/log/syslog | grep dockerd
    // /var/log/daemon.log || /var/log/docker.log
    // syslog
    // https://www.wolfe.id.au/2015/05/03/syslog-logging-driver-for-docker/
    containers[0] = (struct Container){"docker", "daemon", "/var/log/docker.log"};

    glob("/var/lib/docker/containers/*", GLOB_TILDE, glob_warn, &containersDirectories);

    for (i = 0; i < containersDirectories.gl_pathc; i++) {

        char configFilePath[CHAR_MAX];

        strcpy(configFilePath, containersDirectories.gl_pathv[i]);
        strcat(configFilePath, "/config.v2.json");

        // printf("%s\n", configFilePath);

        if (access(configFilePath, R_OK) != -1) {

            json_object *root_obj = json_object_from_file(configFilePath);

            json_object *jID;
            json_object *jContainerName;
            json_object *jLogPath;

            if (json_object_object_get_ex(root_obj, "Name", &jContainerName) == TRUE &&
                json_object_object_get_ex(root_obj, "LogPath", &jLogPath) == TRUE &&
                json_object_object_get_ex(root_obj, "ID", &jID) == TRUE) {

                struct Container container;
                // printf("%d\n", json_object_get_string_len(jID));
                strcpy(container.id, json_object_get_string(jID));
                strcpy(container.name, json_object_get_string(jContainerName) + 1);
                strcpy(container.logPath, json_object_get_string(jLogPath));
                json_object_put(root_obj);

                if (strlen(container.logPath) <= 0) {
                    continue;
                }

                count++;
                struct Container *tmpContainers = realloc(containers, count * sizeof(struct Container));
                if (tmpContainers == NULL) {
                    if (containers) {
                        free(containers);
                    }
                    die("Realloc");
                }

                containers = tmpContainers;
                containers[count - 1] = container;
            }
        }
    }

    struct Containers containersList = {containers, count};

    globfree(&containersDirectories);

    return containersList;
}

FILE *openFile(char *path) {

    char *dirCopy, *baseCopy, *directory, *fileName;

    dirCopy = strdup(path);
    baseCopy = strdup(path);
    directory = dirname(dirCopy);
    fileName = basename(baseCopy);

    if (mkdir(directory, 755) == -1 && errno != EEXIST) {
        die("mkdir");
    }

    free(dirCopy);
    free(baseCopy);

    FILE *pr_file = fopen(path, "a+");
    if (NULL == pr_file) {
        die("fopen");
    }

    return pr_file;
}

/*
 * volatile might be necessary depending on the system/implementation in use.
 * (see "C11 draft standard  n1570: 5.1.2.3")
 *
 */
volatile sig_atomic_t sigintReceived = 0;

void sigint_handler(int s) { sigintReceived = 1; }

void followWithLoop(FILE *file, bool showTimestamps, int *count) {
    char line[LOG_LINE_MAX];
    fseek(file, 0, SEEK_END);
    while (!sigintReceived) {
        if (fgets(line, LOG_LINE_MAX, file)) {
            (*count)++;
            printJsonLine(line, showTimestamps);
        }
        sleep(1);
    }
}

void printJsonLineToDescriptor(char *line, bool showTimestamps) {
    json_object *jsonLine = json_tokener_parse(line);
    /* json_object_object_foreach(jsonLine, key, val)
    printf("%s ", json_object_get_string(val)); */
    if (jsonLine != NULL) {
        json_object *logLine;
        json_object_object_get_ex(jsonLine, "log", &logLine);
        if (showTimestamps == TRUE && logLine != NULL) {
            json_object *time;
            json_object_object_get_ex(jsonLine, "time", &time);
            // printf("%s - ", json_object_get_string(time));

            struct tm logTime;
            strptime(json_object_get_string(time), "%Y-%m-%dT%H:%M:%S", &logTime);

            char datetime[200];
            strftime(datetime, sizeof(datetime) + 1, "%Y-%m-%d %T", &logTime);
            dprintf(STDOUT_FILENO, RED "%s" RESET " - ", datetime);
            // write(STDOUT_FILENO, line, n_read)
        }
        if (logLine != NULL) {
            dprintf(STDOUT_FILENO, GREEN "%s\n" RESET, json_object_get_string(logLine));
        }
    } else {
        dprintf(STDOUT_FILENO, GREEN "%s\n" RESET, line);
    }
    json_object_put(jsonLine);
}

void followWithInotify(FILE *file, struct Container container, bool showTimestamps, int sinceSeconds, int *count) {
    char line[LOG_LINE_MAX];
    fseek(file, 0, SEEK_END);

    int fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }
    int wd = inotify_add_watch(fd, container.logPath, IN_MODIFY);
    if (wd == -1 && errno != EINTR) {
        fprintf(stderr, "Cannot watch '%s': %s\n", container.logPath, strerror(errno));
        exit(EXIT_FAILURE);
    }

    int fdLog = open(container.logPath, O_RDONLY);
    // int fdLog = fileno(file);
    if (fdLog == -1 && errno != EINTR) {
        fprintf(stderr, "open '%s': %s\n", container.logPath, strerror(errno));
        exit(EXIT_FAILURE);
    }
    while (!sigintReceived) {
        struct inotify_event event = {};
        /*
            read to determine the event change happens on “container.logPath” file with inotify_add_watch
            Actually this read blocks until the change event occurs
        */
        ssize_t len = read(fd, &event, sizeof(event));
        if (len == -1 && errno != EAGAIN && errno != EINTR) {
            perror("read event");
            exit(EXIT_FAILURE);
        }

        // if event
        if ((event.mask & IN_MODIFY) && !sigintReceived) {
            (*count)++;
            while (!sigintReceived) {
                ssize_t n_read = read(fdLog, line, LOG_LINE_MAX);
                if (n_read == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                } else if (n_read == 0) {
                    break;
                }

                printJsonLine(line, showTimestamps);
                // fflush(stdout);
                //  write(STDOUT_FILENO, line, n_read);
            }
        }
    }
    inotify_rm_watch(fd, wd);
    close(fdLog);
}

void listLog(FILE *file, struct Container container, bool showTimestamps, bool followLog, int sinceSeconds) {

    fseek(file, 0, SEEK_SET);

    char line[LOG_LINE_MAX];
    int count = 0;

    // signal(SIGINT, sigint_handler);
    struct sigaction action = {};
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &action, NULL) == -1) {
        fprintf(stderr, "sigaction");
    }

    while (fgets(line, LOG_LINE_MAX, file) && !sigintReceived) {
        printJsonLine(line, showTimestamps);
        count++;
    }

    if (followLog && !sigintReceived) {
        /*  printf(RED "Follow: \n" RESET);
         sleep(0.5); */
        followWithInotify(file, container, showTimestamps, sinceSeconds, &count);
    }

    printf(UNDERLINE "\nNº of lines read: %i\n" RESET, count);
    fclose(file);
}

void searchLog(struct Container container, char *string, bool showTimestamps, bool followLog, int sinceSeconds) {

    FILE *logFile = openFile(container.logPath);

    fseek(logFile, 0, SEEK_SET);

    int count = 0;
    char line[LOG_LINE_MAX];

    while (fgets(line, LOG_LINE_MAX, logFile) && !sigintReceived) {
        char *found = strcasestr(line, string);
        if (found != NULL) {
            printJsonLine(line, showTimestamps);
            count++;
        }
    }

    printf(UNDERLINE "Results found for \"%s\": %i\n" RESET, string, count);

    fclose(logFile);
}
int glob_warn(const char *path, int _) {
    printf("Trouble with glob: %s", path);
    return 0;
}
// get time in seconds since the Epoch (January 1, 1970)
int sinceToSeconds(char *since) {

    time_t currentTime = time(NULL);
    time_t sinceSeconds = 0;
    struct tm sinceTm;

    if (strptime(since, "%F%t%T", &sinceTm)) {
        sinceSeconds = mktime(&sinceTm);
        return sinceSeconds;
    }

    char relativeTime;
    int relativeTimeValue;

    if (sscanf(since, "%i%c", &relativeTimeValue, &relativeTime) != 2) {
        return -1;
    }

    sinceSeconds = currentTime;

    switch (relativeTime) {
        case 's':
            sinceSeconds -= relativeTimeValue;
            break;
        case 'm':
            sinceSeconds -= relativeTimeValue * 60;
            break;
        case 'h':
            sinceSeconds -= relativeTimeValue * 60 * 60;
            break;
        default:
            return -1;
    }

    return sinceSeconds;
}

int findOptLongByVal(char value, struct option *optOptions, int sizeOptOptions) {

    for (int i = 0; i < sizeOptOptions; i++) {
        if (optOptions[i].val == value) {
            return i;
        }
    }
    return -1;
}

int countLines(FILE *file) {
    fseek(file, 0, SEEK_SET);
    int count = 0;
    char line[LOG_LINE_MAX];
    while (fgets(line, LOG_LINE_MAX, file)) {
        count++;
    }
    fseek(file, 0, SEEK_SET);
    return count;
}

time_t getJsonTime(char *line) {

    json_object *jsonLine = json_tokener_parse(line);
    time_t date = -1;

    if (jsonLine != NULL) {
        json_object *time;
        json_object_object_get_ex(jsonLine, "time", &time);
        struct tm logTime;
        strptime(json_object_get_string(time), "%Y-%m-%dT%H:%M:%S", &logTime);
        date = mktime(&logTime);
    }

    json_object_put(jsonLine);
    return date;
}

time_t getTimeOnText(char *line) {
    time_t date = -1;

    struct tm logTime;
    if (strptime(line, "%Y-%m-%dT%H:%M:%S", &logTime)) {
        date = mktime(&logTime);
        return date;
    }

    return date;
}

int binarySearchDate(FILE *file, int sinceSeconds, struct Container container) {
    char line[LOG_LINE_MAX];
    int count = countLines(file);
    printf("count: %i\n", count);
    int low = 0;
    int high = count - 1;
    int mid = 0;
    time_t date = 0;

    return -1;
}

int binarySearch(FILE *file, int lineNumber) {

    int count = countLines(file);
    int start = 0;
    int end = count - 1;
    int mid = 0;

    while (start <= end) {
        mid = (start + end) / 2;
        fseek(file, mid * LOG_LINE_MAX, SEEK_SET);
        char line[LOG_LINE_MAX];
        fgets(line, LOG_LINE_MAX, file);
        int lineNumberFromFile = atoi(line);
        if (lineNumberFromFile == lineNumber) {
            return mid;
        } else if (lineNumberFromFile < lineNumber) {
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return -1;
}