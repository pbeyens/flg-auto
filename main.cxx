#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>

#include "sgf/sgf.h"

static struct timespec ts;
static int playing;
static int s;
static char msg[10000];

static void mysend(void)
{
	size_t size;
	size = send(s,msg,strlen(msg),0);
	assert(size==strlen(msg));
	printf("%s\n",msg);
	msg[0] = '\0';
}

static void sgf_node_new(void)
{
	mysend();
	//nanosleep(&ts,0);
	//fflush(stdout);
	sleep(1);
	strcat(msg,";");
}

static void sgf_sz(int sz)
{
	char m[10];
	sprintf(m,"SZ[%d]",sz);
	strcat(msg,m);
}

static void sgf_b(char cx, char cy)
{
	char m[10];
	sprintf(m,"B[%c%c]",cx,cy);
	strcat(msg,m);
	if(!playing) { sleep(1); playing = 1; }
}

static void sgf_w(char cx, char cy)
{
	char m[10];
	sprintf(m,"W[%c%c]",cx,cy);
	strcat(msg,m);
	if(!playing) { sleep(1); playing = 1; }
}

static void sgf_add(char col, char cx, char cy)
{
	char m[10];
	sprintf(m,"A%c[%c%c]",col,cx,cy);
	strcat(msg,m);
}

static void sgf_ab(char cx, char cy)
{
	sgf_add('B',cx,cy);
}

static void sgf_aw(char cx, char cy)
{
	sgf_add('W',cx,cy);
}

static void sgf_ae(char cx, char cy)
{
}

static void sgf_pw(const char *prop, int size)
{
}

static void sgf_pb(const char *prop, int size)
{
}

static void sgf_cr(char cx, char cy)
{
}

static void sgf_prop_unknown(const char *prop, int size)
{
}


static const struct sgf_cb scb = { \
	sgf_node_new, \
	sgf_sz, \
	sgf_b, sgf_w, sgf_ab, sgf_aw, sgf_ae, \
	sgf_pw, sgf_pb, \
	sgf_cr, \
	sgf_prop_unknown };

static char* read_file(const char *sf)
{
	FILE *fp;
	long lSize;
	char *buffer;

	fp = fopen(sf,"rt");
	if(!fp) {
		perror("file open failed");
		exit(-1);
	}
	fseek(fp,0L,SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	buffer = (char*)calloc(1,lSize+1);
	if(!buffer){
		fclose(fp);
		fputs("mem alloc failed",stderr);
		exit(-1);
	}
	if(1!=fread(buffer,lSize,1,fp)) {
		fclose(fp);
		free(buffer);
		fputs("read failed",stderr);
		exit(-1);
	}
	fclose(fp);
	return buffer;
}

int main(int argc, char** argv)
{
	struct sockaddr_in addr;
	char * sgffile;
	char * sgf;
	if(argc < 2)
		return -1;
	sgffile = argv[1];

	ts.tv_sec = 0;
	ts.tv_nsec = 500*1000*1000;

	/* socket stuff */
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s < 0) {
		perror("socket");
		exit(-1);
	}
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(5000);

	if(0 > connect(s,(struct sockaddr*)&addr,sizeof(addr))) {
		perror("connect");
		return -1;
	}

	sgf = read_file(sgffile);

	sgf_init(&scb);
	sgf_parse_fast(sgf);

	mysend();
	fclose(stdout);
	close(s);
	free(sgf);

	return 0;
}
