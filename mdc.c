 #include<stdio.h>
 #include<stdlib.h>
 #include<string.h>
 #include<fcntl.h>
 int code_length(int);
 int open_file(char *);
 char *creat_master_array(int );
 int compress(int,char *,int);
 int compress5(int,char *,int);
 char location(char,char *);
 int decompress(int,int,char * );

int main(int argc, char *argv[])
{
	int fd,wfd,efd,cl,ndc;
	char *ma;
	if(argc<2)
	{
		perror("File not entered");
		goto out;
	}
	fd=file_open(argv[1]);
	  fd=("text",O_RDONLY);



	printf("fd=%d\n",fd);
	printf("%s opend in reading mode\n",argv[1]);
        ma=creat_master_array(fd);
	ndc=strlen(ma);
	printf("ma : %s\n",ma);
	printf("Length of ma is:%d\n",ndc);
	efd=open("encrypt_key",O_WRONLY|O_CREAT);
	if(efd<0)
	{
		perror("OPEN EFD");
		goto out;
	}
	printf("File opened successfully with efd : %d\n",efd);
	write(efd,ma,ndc);
	cl=code_length(ndc);
	printf("code length is:%d\n",cl);
	compress(cl,ma,fd);
	exit(EXIT_SUCCESS);
out:
	exit(EXIT_FAILURE);
}
char * creat_master_array(int fd)
{
	char ch;
	char *buff;
	int i=0,j,flag=0;
	buff=malloc(sizeof(char)*1);
	read(fd,&ch,1);
	*(buff+i)=ch;
	i++;
	while(read(fd,&ch,1))
	{
		if(ch==10)
			break;
		else
		{
			for(j=0;j<i;j++)
			{
				//if(ch==*(buff+j))
				if(*(buff+j)==ch)
				{
					flag=1;
					break;
				}
			}
			if(flag==0)
			{
				*(buff+i)=ch;
				i++;
				buff=realloc(buff,i);

			}
			flag=0;
		}

	}
	printf("i=%d\n",i);
	*(buff+i)=10;
	return buff;
	
}
int code_length(int ndc)
{
	int cl;
	if(ndc<=4)
		cl=2;
	else if(ndc<=8)
		cl=3;
	else if(ndc<=16)
		cl=4;
	else if(ndc<=32)
		cl=5;
	else if(ndc<=64)
		cl=6;
	else if(ndc<=128)
		cl=7;
	return cl;

}
int file_open(char *file)
{
	int fd;
	fd=open(file,O_RDONLY);
	if(fd<0)
	{
		perror("OPEN");
		exit(EXIT_FAILURE);
		
	}
	return fd;

}
int compress(int cl,char *ma,int fd)
{
	int wfd;
	unsigned char ch;
	switch(cl)
	{
		case 5:
			wfd=compress5(cl,ma,fd);
			lseek(wfd,0,SEEK_SET);
			//decompress(wfd,cl,ma);
			break;
		default:
			printf("error\n");
	}
}
int compress5(int cl,char *ma,int fd)
{
	unsigned char cha,chb,ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch;
	unsigned char byte;
	int m,wfd,x=15;
	lseek(fd,0,SEEK_SET);
	wfd=open("compress",O_RDWR|O_CREAT);
	if(wfd<0)
	{
		perror("OPEN wfd");
		exit(1);
	}
	printf("file opened successfully with wfd :%d\n",wfd);
	while(read(fd,&cha,1))
	{
		
		printf("%c\n",cha);
		if(cha==10)
			break;
		else
		{
			ch1=location(cha,ma);
			ch1=ch1<<3;
			read(fd,&cha,1);
			ch=ch2=location(cha,ma);
			ch2=ch2<<3;
			ch2=ch2>>5;
			byte=ch1|ch2;
			write(wfd,&byte,1);
			ch2=ch<<6;

		}
		
	}
	return wfd;

}
char location(char ch,char *ma)
{
	int i;
	for(i=0;i<strlen(ma);i++)
	{
		if(*(ma+i)==ch)
		return i;
	}
}
int decompress(int wfd,int cl,char *ma)
{
	unsigned char ch1,ch2,ch;
	int wfd2;
	printf("wfd in decompress is:%d\n",wfd);
	wfd2=open("decompress",O_RDWR|O_CREAT);
	if(wfd2<0)
	{
		perror("open wfd2");
		exit(1);
	}
	printf("File opened successfully with wfd2:%d\n",wfd2);
	while(read(wfd,&ch,1))
	{
		
		
			
			printf("ch=%d\n",ch);
			ch1=ch>>cl;
			//printf("%c\n",ma[ch1]);
			write(wfd2,&ma[ch1],1);
			ch2=ch<<cl;
			ch2=ch2>>cl;
			//printf("%d\n",ch2);
			write(wfd2,&ma[ch2],1);
		
	
	}
	
}
