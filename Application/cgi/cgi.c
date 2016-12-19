#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *data;
	long m;
	printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
	printf("<TITLE>Bart Kneepkens & Martin Donkersloot</TITLE>\n");
	printf("<H3>Hi!</H3>\n");
	
	data = getenv("QUERY_STRING");
	
	if(data == NULL){
		printf("<P>Error! Error in passing data from form to script.");
	}
	else{
		printf("<P>Your name  is "); 
		printf("%s", data+5);
	}
	
	return 0;
}
