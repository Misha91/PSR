#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sockLib.h>
#include <string.h>

#define SERVER_PORT     80 /* Port 80 is reserved for HTTP protocol */
#define SERVER_MAX_CONNECTIONS  20



int pM[1024];
int pS[1024];
int pE[1024];

#define HTMLs "<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.01 Transitional//EN' 'http://www.w3.org/TR/html4/loose.dtd'>\
<html>\
<head>\
<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />\
<title>Motor Status Example</title>\
</head>\
<body onload='setTimeout(function(){location.reload()}, 100);'>\
<h1>Motor Status Example</h1>\
<object type='image/svg+xml' data='graph.svg'>\
</body> </html>"

#define HTMLg "<?xml version='1.0'?>\
		<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.0//EN' 'http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd'>\
<svg width='1000' height='300' xmlns='http://www.w3.org/2000/svg'>\
  <g transform='translate(50,130) scale(1)'>\
    <!-- Now Draw the main X and Y axis -->\
    <g style='stroke-width:2; stroke:black'>\
      <!-- X Axis -->\
      <path d='M 0 0 L 1250 0 Z'/>\
      <!-- Y Axis -->\
      <path d='M 0 -300 L 0 300 Z'/>\
    </g>\
    <g style='fill:none; stroke:#B0B0B0; stroke-width:1; stroke-dasharray:2 4;text-anchor:end; font-size:30'>\
      <text style='fill:black; stroke:none' x='-1' y='100' >-100</text>\
      <text style='fill:black; stroke:none' x='-1' y='200' >-200</text>\
      <text style='fill:black; stroke:none' x='-1' y='0' >0</text>\
      <text style='fill:black; stroke:none' x='-1' y='-200' >200</text>\
      <text style='fill:black; stroke:none' x='-1' y='-100' >100</text>\
      <g style='text-anchor:middle'>\
	<text style='fill:black; stroke:none' x='100' y='20' >100</text>\
	<text style='fill:black; stroke:none' x='200' y='20' >200</text>\
	<text style='fill:black; stroke:none' x='300' y='20' >300</text>\
	<text style='fill:black; stroke:none' x='400' y='20' >400</text>\
	<text style='fill:black; stroke:none' x='500' y='20' >500</text>\
	<text style='fill:black; stroke:none' x='600' y='20' >600</text>\
	<text style='fill:black; stroke:none' x='700' y='20' >700</text>\
	<text style='fill:black; stroke:none' x='800' y='20' >800</text>\
	<text style='fill:black; stroke:none' x='900' y='20' >900</text>\
	<text style='fill:black; stroke:none' x='1000' y='20' >1000</text>\
	<text style='fill:black; stroke:none' x='1100' y='20' >1100</text>\
      </g>\
    </g>\
    <polyline\
	      points='"
    
#define HTMLge "'\
	style='stroke:blue; stroke-width: 1; fill : none;'/>\
   <polyline\
  	      points='"

#define HTMLge2 "'\
	style='stroke:red; stroke-width: 1; fill : none;'/>\
   <polyline\
  	      points='"
		
#define HTMLgend "'\
	style='stroke:green; stroke-width: 1; fill : none;'/>\
  </g>\
</svg>"
	
void newRequest(int newFd, int *h, int *dataN);

void www(int* s, int *h, int *dataN, int argc, char *argv[])
{
  
  int newFd;
  struct sockaddr_in serverAddr;
  struct sockaddr_in clientAddr;
  int sockAddrSize;

  sockAddrSize = sizeof(struct sockaddr_in);
  bzero((char *) &serverAddr, sizeof(struct sockaddr_in));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(SERVER_PORT);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  *s = socket(AF_INET, SOCK_STREAM, 0);
  printf("%d\n",*s);
  if (*s<0)
  {
    printf("Error: www: socket(%d)\n", *s);
    return;
  }


  if (bind(*s, (struct sockaddr *) &serverAddr, sockAddrSize) == ERROR)
  {
    printf("Error: www: bind\n");
    return;
  }

  if (listen(*s, SERVER_MAX_CONNECTIONS) == ERROR)
  {
    perror("www listen");
    close(*s);
    return;
  }

  printf("www server running, s:%d\n", *s);

  while(1)
  {
    /* accept waits for somebody to connect and the returns a new file descriptor */
    if ((newFd = accept(*s, (struct sockaddr *) &clientAddr, &sockAddrSize)) == ERROR)
    {
      perror("www accept");
      close(*s);
      return;
    }
    else{
    	printf("new connection on www\n");
    	taskSafe();
    	newRequest(newFd, &(*h), &(*dataN));
    }
    taskUnsafe();
    //FILE *f = fdopen(newFd, "w");
    //fprintf(f, "HTTP/1.0 200 OK\r\n\r\n");
    //fprintf(f, "Current time is %ld.\n", time(NULL));
    //fclose(f);
    /* The client connected from IP address inet_ntoa(clientAddr.sin_addr)
       and port ntohs(clientAddr.sin_port).

       Start a new task forHTMLge each request. The task will parse the request
       and sends back the response.

       Don't forget to close newFd at the end */
  }
}

void newRequest(int newFd, int *h, int *dataN){
	(*h)++;
	FILE *f = fdopen(newFd, "r+");	
	char str1[10], str2[10], str3[10];
	fscanf(f, "%s %s %s", str1,str2,str3);
	//str1[9]='\0';
	//str2[9]='\0';
	//str3[9]='\0';
	//printf("Read String1 |%s|\n", str1 );
	//printf("Read String2 |%s|\n", str2 );
	//printf("Read String3 |%s|\n", str3 );
	//fprintf(f, "HTTP/1.0 200 OK\r\nContent-Type: image/svg+html\r\n\r\n"); //Content-Type: image/svg+html\n
	if (strcmp(str2, "/graph.svg") < 0){
	//printf("b");
	//printf("\n%s", str2);
	fprintf(f, "HTTP/1.0 200 OK\r\n\r\n"); //Content-Type: image/svg+html\n
	//fprintf(f, "Current time is %ld.\n", time(NULL));
	fprintf(f, "%s", HTMLs);
	}
	if (strcmp(str2, "/graph.svg") >= 0){
		//printf("a");
		int a = 0;
		int x = 0;
		fprintf(f, "HTTP/1.0 200 OK\r\nContent-Type: image/svg+xml\r\n\r\n");
		fprintf(f, "%s", HTMLg); // HTMLge
		for (a = *dataN; a < 1024; a++){
			fprintf(f, "%d,    %d\n", x, pM[a]*(-1));
			x++;
		}
		for (a = 0; a < *dataN - 1; a++){
			fprintf(f, "%d,    %d\n", x, pM[a]*(-1));
			x++;
		}
		fprintf(f, "%d,    %d", x, pM[*dataN]*(-1));
		fprintf(f, "%s", HTMLge);
		a = 0;
		x = 0;
				
//		for (a = *dataN; a < 1024; a++){
//			fprintf(f, "%d,    %d\n", x, pE[a]);
//			x++;
//		}
//		for (a = 0; a < *dataN - 1; a++){
//			fprintf(f, "%d,    %d\n", x, pE[a]);
//			x++;
//		}
//		fprintf(f, "%d,    %d", x, pE[*dataN]);
//		printf(f, "%s", HTMLge2);
//		a = 0;
//		x = 0;
		
		
		for (a = *dataN; a < 1024; a++){
			fprintf(f, "%d,    %d\n", x, pS[a]*(-1));
			x++;
		}
		for (a = 0; a < *dataN - 1; a++){
			fprintf(f, "%d,    %d\n", x, pS[a]*(-1));
			x++;
		}
		fprintf(f, "%d,    %d", x, pS[*dataN]*(-1));
		fprintf(f, "%s", HTMLgend);
	}
	
	fclose(f);	
}


