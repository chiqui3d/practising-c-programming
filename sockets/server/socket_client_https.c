#include "../../include/h_error.h"
#include "../../include/h_prompt.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h> // for getaddrinfo() and getservbyname
#include <netinet/in.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>      // for fprintf()
#include <stdlib.h>     // for exit()
#include <string.h>     // for strlen()
#include <sys/socket.h> // for socket()
#include <sys/types.h>  // for pid_t
#include <unistd.h>     // for close()

#define BUF_SIZE 10000

#define SEND_SIZE 1024

#define DOMAIN "www.reddit.com:443"

#define PATH "/"

#define HOST_NAME "www.reddit.com"

#define HOST_PORT "443"

#define HEADER "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n"

// https://developer.ibm.com/tutorials/l-openssl/#
// https://wiki.openssl.org/index.php/Simple_TLS_Server
// https://fm4dd.com/openssl/sslconnect.shtm
// https://aticleworld.com/ssl-server-client-using-openssl-in-c/

void InitializeSSL() {
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
}
void ShowCerts(SSL *ssl) {
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if (cert != NULL) {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line); /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);      /* free the malloc'ed string */
        X509_free(cert); /* free the malloc'ed certificate copy */
    } else {
        printf("Info: No client certificates configured.\n");
    }
}

int main() {

    InitializeSSL();

    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    SSL *ssl;

    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

    if (!SSL_CTX_load_verify_locations(ctx, NULL, "/usr/lib/ssl/certs")) {
        die("SSL_CTX_load_verify_locations");
    }

    BIO *bio = BIO_new_ssl_connect(ctx);
    if (bio == NULL) {
        die("BIO_new_ssl_connect");
    }

    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    BIO_set_conn_hostname(bio, DOMAIN);

    if (BIO_do_connect(bio) <= 0) {
        die("BIO_new_connect");
    }

    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        die("SSL_get_verify_result");
    }

    ShowCerts(ssl);

    char send_data[SEND_SIZE];
    snprintf(send_data, sizeof(send_data), HEADER, "/r/PHP/", HOST_NAME);
    BIO_puts(bio, send_data);
    // BIO_write(bio, send_data, strlen(send_data) + 1);

    char buffer[BUF_SIZE];
    int n = 0;
    char *HTML = malloc(sizeof(char));

    printf("HTML:\n");
    do {
        n = BIO_read(bio, buffer, BUF_SIZE);
        if (n <= 0) {
            break;
        }
        char *tmp = realloc(HTML, strlen(HTML) + n + 1);
        if (tmp == NULL) {
            free(HTML);
            die("realloc");
        }
        HTML = tmp;
        strncat(HTML, buffer, n);
    } while (n > 0);
    /*
        if (n <= 0){
            if (BIO_should_retry(bio->io)){
                printf("BIO_should_retry\n");
            }else{
                printf("BIO_should_not_retry\n");
            }
        } */

    int len = strlen(HTML);
    HTML[len] = '\0';
    printf("%s\n", HTML);
    printf("Len: %i\n", len);

    free(HTML);
    SSL_CTX_free(ctx);
    BIO_free_all(bio);

    return EXIT_SUCCESS;
}