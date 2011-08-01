#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


//struct addrinfo {
	//int              ai_flags;
	//int              ai_family;
	//int              ai_socktype;
	//int              ai_protocol;
	//size_t           ai_addrlen;
	//struct sockaddr *ai_addr;
	//char            *ai_canonname;
	//struct addrinfo *ai_next;
//};

char *strdup(const char *s);

//int getaddrinfo(const char *node, const char *service,
		//const struct addrinfo *hints,
		//struct addrinfo **res);



// get sockaddr, IPv4 or IPv6:
//void *get_in_addr(struct sockaddr *sa)
//{
    //if (sa->sa_family == AF_INET) {
        //return &(((struct sockaddr_in*)sa)->sin_addr);
    //}

    //return &(((struct sockaddr_in6*)sa)->sin6_addr);
//}



int hexdump(char *data,int len,int size){
	char *hex="0123456789ABCDEF";
	for(int n=0;n<len;++n){
		printf("%c%c ",hex[(unsigned char)data[n]>>4],hex[(unsigned char)data[n]&15]);
	}
	for(int n=len;n<size;++n){
		printf("   ");
	}
	printf(" |");
	for(int n=0;n<len;++n){
		printf("%c",isprint(data[n])?data[n]:'.');
	}
	printf("|");
	for(int n=len;n<size;++n){
		printf(" ");
	}
	printf("\n");

	return 0;
}













int main(int argc,char **argv){

	if(argc<5){
		//fail
		return 1;
	}

	char *laddr=strdup(argv[1]);
	char *lport=strdup(argv[2]);
	char *raddr=strdup(argv[3]);
	char *rport=strdup(argv[4]);

	printf("local:  [%s:%s]\n",laddr,lport);
	printf("remote: [%s:%s]\n",raddr,rport);



	int fdlsocket=-1;
	int fdrsocket=-1;




	struct addrinfo hints, *servinfo, *p;
	//int rv,sockfd;
	//char s[INET6_ADDRSTRLEN];


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags=2;//AI_CANONNAME;


	if((getaddrinfo(laddr,lport,&hints,&servinfo))!=0){
		//fprintf(stderr, "getaddrinfo: %d\n", gai_strerror(rv));
		puts("[-] getaddrinfo");
		return 1;
	}
	puts("[+] getaddrinfo");

	int lsocket=0;
	struct sockaddr_in *sa=NULL;
	for(p=servinfo;p!=NULL;p=p->ai_next){
		//printf("ai_flags: [%d]\n",p->ai_flags);
		//printf("ai_family: [%d]\n",p->ai_family);
		//printf("ai_socktype: [%d]\n",p->ai_socktype);
		//printf("ai_protocol: [%d]\n",p->ai_protocol);
		//printf("ai_addrlen: [%u]\n",p->ai_addrlen);
		//printf("ai_canonname: [%s]\n",p->ai_canonname);
		////printf("ai_flags: [%d]\n",p->ai_flags);
		//sa=p->ai_addr;
		////if(sa->sin_family==AF_INET){
		////struct in_addr *temp=&(sa->sin_addr);
		//printf("port: [%d]\n",sa->sin_port);
		//char s[INET6_ADDRSTRLEN];
		////((struct in_addr)temp)->s_addr=INADDR_ANY;
		////temp->s_addr=INADDR_ANY;
		//inet_ntop(p->ai_family,&sa->sin_addr,s,sizeof(s));
		//printf("client: connecting to %s\n", s);

		if((lsocket=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0){
			perror("[-] socket");
			continue;
		}else{
			puts("[+] socket");
		}

		int flags=1;
		if(setsockopt(lsocket,SOL_SOCKET,SO_REUSEADDR,&flags,sizeof(flags))!=0){
			perror("[-] setsockopt");
			return 1;
		}else{
			puts("[+] setsockopt");
		}


		if(bind(lsocket,p->ai_addr,p->ai_addrlen)<0){
			perror("[-] bind");
			continue;
		}else{
			puts("[+] bind");
			break;
		}

		printf("----------------\n");
	}


	if(p==NULL){
		fprintf(stderr, "selectserver: failed to bind\n");
		return 1;
	}



	//bind check

	if(listen(lsocket,3)!=0){
		perror("[-] listen");
		return 1;
	}else{
		puts("[+] listen");
	}




	socklen_t addrlen;
	addrlen = sizeof(struct sockaddr_in);

	if((fdlsocket=accept(lsocket, (struct sockaddr *)sa,&addrlen))<0){
		puts("[-] accept");
		return 1;
	}else{
		puts("[+] accept");
	}



	//char *buffer="helloworld";//calloc(sbuffer,1);
	////recv(fdrsocket,buffer,sbuffer,0);
		////printf("[<] rrecv [%s]\n",buffer);
	//if(send(fdlsocket,buffer,strlen(buffer),0)!=-1){
		//printf("[>] lsent [%s]\n",buffer);
	//}

	//return 3;

	puts("[!] server done...");




	if((getaddrinfo(raddr,rport,&hints,&servinfo))!=0){
		//fprintf(stderr, "getaddrinfo: %d\n", gai_strerror(rv));
		puts("[-] getaddrinfo");
		return 1;
	}
	puts("[+] getaddrinfo");

	//int rsocket=0;
	//struct sockaddr_in *sa=NULL;
	for(p=servinfo;p!=NULL;p=p->ai_next){
		//printf("ai_flags: [%d]\n",p->ai_flags);
		//printf("ai_family: [%d]\n",p->ai_family);
		//printf("ai_socktype: [%d]\n",p->ai_socktype);
		//printf("ai_protocol: [%d]\n",p->ai_protocol);
		//printf("ai_addrlen: [%u]\n",p->ai_addrlen);
		//printf("ai_canonname: [%s]\n",p->ai_canonname);
		////printf("ai_flags: [%d]\n",p->ai_flags);
		//sa=p->ai_addr;
		////if(sa->sin_family==AF_INET){
		////struct in_addr *temp=&(sa->sin_addr);
		//printf("port: [%d]\n",sa->sin_port);
		//char s[INET6_ADDRSTRLEN];
		////((struct in_addr)temp)->s_addr=INADDR_ANY;
		////temp->s_addr=INADDR_ANY;
		//inet_ntop(p->ai_family,&sa->sin_addr,s,sizeof(s));
		//printf("client: connecting to %s\n", s);

		if((fdrsocket=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0){
			perror("[-] socket");
			continue;
		}else{
			puts("[+] socket");
		}


		if (connect(fdrsocket,p->ai_addr,p->ai_addrlen)==-1) {
			//close(sockfd);
			puts("[-] connect");
			continue;
		}else{
			puts("[+] connect");
		}
		//dup2(rsocket,fdrsocket);
		break;

	}



	//char *buffer="helloworld";//calloc(sbuffer,1);
	////recv(fdrsocket,buffer,sbuffer,0);
		////printf("[<] rrecv [%s]\n",buffer);
	//if(send(fdrsocket,buffer,strlen(buffer),0)!=-1){
		//printf("[>] rsent [%s]\n",buffer);
	//}else{
		//perror("[>] rsent");
	//}

	//return 3;





	pid_t child=-1;
	if((child=fork())==-1){
		puts("[-] fork");
		return 2;
	}
	printf("[+] fork %d\n",child);

	if(child){//we are the server

		puts("[!] server start");
		int sbuffer=16;
		char *buffer=calloc(sbuffer,1);
		int count=0;
		while((count=recv(fdlsocket,buffer,sbuffer,0))>0){
//			printf("[<] lrecv [%s]\n",buffer);
			printf("\e[32m[>] ");
			hexdump(buffer,count,sbuffer);
			printf("\e[0m");
//			printf("%s",buffer);
			fflush(stdout);
			if(send(fdrsocket,buffer,count,0)!=-1){
//				printf("[>] rsent [%s]\n",buffer);
			}
			memset(buffer,'\0',sbuffer);
		}

		free(buffer);
		buffer=NULL;
		puts("[!] server done");
		puts("[!] remote shutdown");
		if(shutdown(fdrsocket,2)!=0){
			perror("[-] shutdown");
		}else{
			perror("[+] shutdown");
		}

	}else{//we are the client

		puts("[!] client start");
		int sbuffer=16;
		char *buffer=calloc(sbuffer,1);
		int count=0;
		while((count=recv(fdrsocket,buffer,sbuffer,0))>0){
			//printf("[<] rrecv [%s]\n",buffer);
			printf("\e[34m[<] ");
			hexdump(buffer,count,sbuffer);
			printf("\e[0m");
			//printf("%s",buffer);
			fflush(stdout);
			if(send(fdlsocket,buffer,count,0)!=-1){
				//printf("[>] lsent [%s]\n",buffer);
			}
			memset(buffer,'\0',sbuffer);
		}

		free(buffer);
		buffer=NULL;
		puts("[!] client done");
		puts("[!] local shutdown");
		if(shutdown(fdlsocket,2)!=0){
			perror("[-] shutdown");
		}else{
			perror("[+] shutdown");
		}

	}






	return 0;
}
