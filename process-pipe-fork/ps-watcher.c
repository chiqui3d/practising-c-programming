#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/socket.h>
#include "./ps-watcher.h"

// https://bewareofgeek.livejournal.com/2945.html
// https://gist.github.com/iNarcissuss/4809c725eb04b070f439d9c6df28cdd7
// https://stackoverflow.com/questions/6075013/how-to-detect-the-launching-of-programs-on-linux
// https://stackoverflow.com/questions/26852228/detect-new-process-creation-instantly-in-linux
// https://serverfault.com/questions/966709/how-to-identify-the-source-of-growing-blocked-processes-on-a-system-container-l

#define CONFIG_PROC_EVENTS "y"

static volatile bool need_exit = false;
static void on_sigint(int unused) {
    need_exit = true;
}

static int proc_handle(int nl_sock, struct cn_msg *nlcn_msg) {
    int rc;
    struct proc_event *ev = (struct proc_event *)nlcn_msg->data;

    while (!need_exit) {
        rc = recv(nl_sock, &nlcn_msg, sizeof(nlcn_msg), 0);
        if (rc == 0) {
            /* shutdown? */
            return 0;
        } else if (rc == -1) {
            if (errno == EINTR)
                continue;
            perror("netlink recv");
            return -1;
        }
        switch (ev->what) {
            case PROC_EVENT_NONE:
                printf("set mcast listen ok\n");
                break;
            case PROC_EVENT_FORK:
                printf("fork: parent tid=%d pid=%d -> child tid=%d pid=%d\n",
                       ev->event_data.fork.parent_pid,
                       ev->event_data.fork.parent_tgid,
                       ev->event_data.fork.child_pid,
                       ev->event_data.fork.child_tgid);
                break;
            case PROC_EVENT_EXEC:
                printf("exec: tid=%d pid=%d\n",
                       ev->event_data.exec.process_pid,
                       ev->event_data.exec.process_tgid);
                break;
            case PROC_EVENT_UID:
                printf("uid change: tid=%d pid=%d from %d to %d\n",
                       ev->event_data.id.process_pid,
                       ev->event_data.id.process_tgid,
                       ev->event_data.id.r.ruid,
                       ev->event_data.id.e.euid);
                break;
            case PROC_EVENT_GID:
                printf("gid change: tid=%d pid=%d from %d to %d\n",
                       ev->event_data.id.process_pid,
                       ev->event_data.id.process_tgid,
                       ev->event_data.id.r.rgid,
                       ev->event_data.id.e.egid);
                break;
            case PROC_EVENT_EXIT:
                printf("exit: tid=%d pid=%d exit_code=%d\n",
                       ev->event_data.exit.process_pid,
                       ev->event_data.exit.process_tgid,
                       ev->event_data.exit.exit_code);
                break;
            default:
                printf("unhandled proc event\n");
                break;
        }
    }

    return 0;
}
struct cn_msg proc_listen(int nl_sock) {

    char buff[BUFF_SIZE];
    struct nlmsghdr *nl_hdr = (struct nlmsghdr *)buff;
    struct cn_msg *cn_hdr = (struct cn_msg *)NLMSG_DATA(nl_hdr);
    enum proc_cn_mcast_op *mcop_msg = (enum proc_cn_mcast_op *)&cn_hdr->data[0];

    printf("sending proc connector: PROC_CN_MCAST_LISTEN... ");
    memset(buff, 0, sizeof(buff));

    /* fill the netlink header */
    nl_hdr->nlmsg_len = SEND_MESSAGE_LEN;
    nl_hdr->nlmsg_type = NLMSG_DONE;
    nl_hdr->nlmsg_pid = getpid();
    /* fill the connector header */
    cn_hdr->id.idx = CN_IDX_PROC;
    cn_hdr->id.val = CN_VAL_PROC;
    cn_hdr->len = sizeof(enum proc_cn_mcast_op);
    if (send(nl_sock, nl_hdr, nl_hdr->nlmsg_len, 0) != nl_hdr->nlmsg_len) {
        perror("failed to send proc connector mcast ctl op!\n");
        exit(EXIT_FAILURE);
    }

    return *cn_hdr;
}

static int proc_connect() {
    int rc;
    int nl_sock;
    struct sockaddr_nl sa_nl;

    nl_sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
    if (nl_sock == -1) {
        perror("socket");
        return -1;
    }

    sa_nl.nl_family = AF_NETLINK;
    sa_nl.nl_groups = CN_IDX_PROC;
    sa_nl.nl_pid = getpid();

    rc = bind(nl_sock, (struct sockaddr *)&sa_nl, sizeof(sa_nl));
    if (rc == -1) {
        perror("bind");
        close(nl_sock);
        return -1;
    }

    return nl_sock;
}

int main() {

    signal(SIGINT, &on_sigint);
    siginterrupt(SIGINT, true);

    /* The argument list to pass to the "ls" command. */
    char *arg_list[] = {
        "ps", /* argv[0], the name of the program. */
        "-eo",
        "stime,pid,ppid,tty,user,comm,%mem,%cpu,wchan:14,stat,cmd",
        "--sort=-%mem",
        "--forest",
        NULL /* The argument list must end with a NULL. */
    };

    int nl_sock = proc_connect();
    if (nl_sock == -1) {
        perror("Socket\n");
        exit(EXIT_FAILURE);
    }

    setvbuf(stdout, NULL, _IONBF, 0);

    struct cn_msg cn_hdr = proc_listen(nl_sock);

    proc_handle(nl_sock,&cn_hdr);

    /*  pid_t child_pid;
     child_pid = fork();
     if (child_pid != 0) {
         waitpid(child_pid, NULL, 0);
     } else {
         execvp("ps", arg_list);
         exit(EXIT_SUCCESS);
     }
  */

    printf("ps done\n");

    close(nl_sock);
    return EXIT_SUCCESS;
}