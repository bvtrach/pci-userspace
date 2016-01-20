#include <sys/stat.h>

/* now, obviously you need the right firmware file for _your_ device */
#define FIRMWARE "/lib/firmware/rtl_nic/rtl8168e-2.fw"

#define CTRLSOCK "/tmp/iwnsock"
#define CTRLURL "unix://" CTRLSOCK

#include "common.c"

#include <rump/netconfig.h>
#include <rump/rump_syscalls.h>
#include <arpa/inet.h>
#define DESTADDR 0x43b484d1
#define WANTHTML "GET / HTTP/1.0\n\n"
#define BUFSIZE (1024*64)

static struct rump_boot_etfs eb = {
    .eb_key = "/libdata/firmware/if_re_pci/" FIRMWARE,
    .eb_hostpath = FIRMWARE,
    .eb_type = RUMP_ETFS_REG,
    .eb_begin = 0,
    .eb_size = RUMP_ETFS_SIZE_ENDOFF,
};

int netconnect()
{
    struct sockaddr_in sin;
    char *buf;
    ssize_t nn, off;
    int error;
    int s;
    s = rump_sys_socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        printf("you're mean! no sucket for you!\n");
        return -1;
    }
    printf("!!!\nTHE FD value: %d\n!!!\n", s);
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80);
    sin.sin_addr.s_addr = DESTADDR;
    if (rump_sys_connect(s, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
        printf("could not connect\n");
        return -1;
    }

    nn = rump_sys_write(s, WANTHTML, sizeof(WANTHTML)-1);
    printf("wrote http request, rv %zd\n", nn);

    buf = calloc(1, BUFSIZE);
    off = 0;
    do {
        nn = rump_sys_read(s, buf+off, (BUFSIZE-off)-1);
        off += nn;
        if (off >= BUFSIZE) exit(-1);
    } while (nn > 0);
    if (nn == -1) {
        printf("read failed: %zd\n", nn);
        return -1;
    }

    printf("read %zd bytes\n", off);

    /* display last 500 bytes of delicious info */
    buf[off] = '\0';
    off -= 500;
    if (off < 0)
        off = 0;
    printf("that was an educational experience.  "
           "we learned:\n");
    printf("%s", &buf[off]);

}

int
main()
{
    struct stat sb;
    int rv;

    if (stat(FIRMWARE, &sb) == -1)
        err(1, "need firmware file %s", FIRMWARE);
    rump_boot_etfs_register(&eb);

    common_bootstrap();
    rump_pub_netconfig_dhcp_ipv4_oneshot("re0");
    netconnect();
        
    common_listen();
}
