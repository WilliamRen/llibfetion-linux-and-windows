AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=libfetion_test
libfetion_test_SOURCES=commdef.h fxsocket.c initial.h main.c process.c sipc.h xml.c config.c fxsocket.h log.c mem.c process.h thread.c xml.h config.h helper.c log.h mem.h protocol.c thread.h crypto.c helper.h login.c mutex.c protocol.h utf8.c crypto.h initial.c login.h mutex.h sipc.c utf8.h
libfetion_test_LDADD=$(top_srcdir)/libsipc4/src/libsipc4.a
INCLUDES=-I$(top_srcdir)/libsipc4/include -I/usr/include/libxml2
libfetion_test_LDFLAGS=-lpthread -lssl -lcurl -lxml2
