#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sgf/sgf.h"

static struct timespec ts;

static void sgf_node_new(void)
{
	nanosleep(&ts,0);
	fflush(stdout);
	printf("\n;");
}

static void sgf_sz(int s)
{
	printf("SZ[%d]",s);
}

static void sgf_b(char cx, char cy)
{
	printf("B[%c%c]",cx,cy);
}

static void sgf_w(char cx, char cy)
{
	printf("W[%c%c]",cx,cy);
}

static void sgf_add(char col, char cx, char cy)
{
	printf("A%c[%c%c]",col,cx,cy);
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
	printf("PW[%s]",prop);
}

static void sgf_pb(const char *prop, int size)
{
	printf("PW[%s]",prop);
}

static void sgf_cr(char cx, char cy)
{
}

static void sgf_prop_unknown(const char *prop, int size)
{
	//printf("%s",prop);
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
		exit(1);
	}
	fseek(fp,0L,SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	buffer = (char*)calloc(1,lSize+1);
	if(!buffer){
		fclose(fp);
		fputs("mem alloc failed",stderr);
		exit(1);
	}
	if(1!=fread(buffer,lSize,1,fp)) {
		fclose(fp);
		free(buffer);
		fputs("read failed",stderr);
		exit(1);
	}
	fclose(fp);
	return buffer;
}

int main(int argc, char** argv)
{
	char * sgffile;
	char * sgf;
	if(argc < 2)
		return -1;
	sgffile = argv[1];

	ts.tv_sec = 0;
	ts.tv_nsec = 500*1000*1000;

	sgf = read_file(sgffile);

	sgf_init(&scb);
	sgf_parse_fast(sgf);

	free(sgf);

	return 0;
}
