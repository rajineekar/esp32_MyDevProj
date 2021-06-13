
#include "global.h"
#include "Connection_Pub.h"


/***********************************************************************************************************************
* Function Name:  esp_err_t  Open_UDP_Server(unsigned int Port_Number, char Mode)
* Description  : This function is used to open the UDP server using Linux network commands .
* Arguments    : Port number of server to be run, Mode( server to be run as blocking mode or non blocking mode)(MODE_NONBLOCK,MODE_BLOCK)
* Return Value : Error(fail or success)
***********************************************************************************************************************/
#if 0
 esp_err_t  Open_UDP_Server(unsigned int Port_Number, char Mode)
{
	int flags;
	int sock_fd=0;
	struct sockaddr_in serverAddr;


	printf("Open_UDP_Server:opening\n");
	/*
		Create UDP socket
	*/
	sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock_fd < 0)
	{
		printf("Open_UDP_Server:socket failed on port_number:(%d)\r\n",Port_Number);
		return ;
	}

	printf("Open_UDP_Server:socket SUCCESS on port_number:(%d),sockfd(%d)\r\n",Port_Number,sock_fd);
	/*
		Configure settings in address struct
	*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(Port_Number);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	/*
		Bind socket with address struct
	*/
	if (bind(sock_fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr))<0)
	{
		printf("Open_UDP_Server:BINDING failed\r\n");
		close(sock_fd);
		return ;
	}

    	if(Mode ==MODE_NONBLOCK)
    	{
		/*
			Get the socket's access mode and status flags
			arg3 is ignored by fcntl during "get"
		*/
		if((flags = fcntl(sock_fd, F_GETFL, 0)) == -1)
		{
			printf("Open_UDP_Server:utils_set_socket_non_blocking: get flags error\n");
			close(sock_fd);
			return ESP_FAIL;

		}

		/*
			Update flag to make the file descriptor non-blocking
		*/
		flags |= O_NONBLOCK;

		/*
			Set the updated flag
		*/
		if(fcntl(sock_fd, F_SETFL, flags) == -1)
		{
			printf("Open_UDP_Server:utils_set_socket_non_blocking: set flags error\n");
			close(sock_fd);
			return ;
		}
    	}
	UDP_Server_Sockfd=sock_fd;
#if 0
	/*
		Initialize size variable to be used later on
	*/
	 while(1)
	 {
		/*
			Try to receive any incoming UDP datagram. Address and port of
			requesting client will be stored on Udp_Client_details variable
		*/
	   	nBytes = recvfrom(UDP_Server_Sockfd,buffer,UDP_BUFFER_LEN,0,(struct sockaddr *)&Udp_Client_details, &addr_size);

		printf("data received from client:%s\n\n",buffer);
		/*
			Send uppercase message back to client, using Udp_Client_details as the address
		*/
		  sendto(UDP_Server_Sockfd,buffer,nBytes,0,(struct sockaddr *)&Udp_Client_details,addr_size);
	 }
#endif
  return ESP_OK;
}




/***********************************************************************************************************************
* Function Name:  esp_err_t  Open_Tcp_Client(char *hostname,unsigned int Port_Number,unsigned char *data,unsigned char len)
* Description  : This function is used to open the TCP client using Linux network commands and send the data through connected fd.
				retry connections is MAX_TCP_CON_RETRY_CONT.
* Arguments    : IP address of the server,Port number of server , data to be send, length of the data to be send
* Return Value : Error(fail or success)
***********************************************************************************************************************/


esp_err_t  Open_Tcp_Client(char *hostname,unsigned int Port_Number,unsigned char *data,unsigned char len)
{
	int  n,i;
	esp_err_t Error=ESP_OK;
	struct sockaddr_in serveraddr;



	printf("Open_Tcp_Client:Creating\n" );

	/*
		socket: create the socket
	*/
	for(i=0;i<MAX_TCP_CON_RETRY_CONT;i++)
	{
		TCP_Client_Sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (TCP_Client_Sockfd < 0)
		{
			printf("Open_Tcp_Client:Creating the socket failed ip is(%s),port(%d)\r\n",hostname,Port_Number);
			Error= ESP_FAIL;
		}

		printf("Open_Tcp_Client:Creating the socket success ip is(%s),port(%d),sockfd(%d)\r\n",hostname,Port_Number,TCP_Client_Sockfd);

		memset(&serveraddr, '0', sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = htons(Port_Number);

		/*
			Convert IPv4 and IPv6 addresses from text to binary form
		*/
		if(inet_pton(AF_INET, hostname , &serveraddr.sin_addr)<=0)
		{
			printf("Open_Tcp_Client:Invalid address/ Address not supported\r\n");
			return ESP_FAIL;
		}

		/*
			connect: create a connection with the server
		*/
		if (connect(TCP_Client_Sockfd, &serveraddr, sizeof(serveraddr)) < 0)
		{
			printf("Open_Tcp_Client:connect failed\n");
			Error = ESP_FAIL;
		}

		if(Error == ESP_FAIL)
		{
			close(TCP_Client_Sockfd);
			TCP_Client_Sockfd = ESP_FAIL;
			vTaskDelay(pdMS_TO_TICKS(2000));
		}
		else
		{
			break;
		}

	}


	if(i == MAX_TCP_CON_RETRY_CONT)
		return ESP_FAIL;
	/*
		send the message line to the server
	*/
	n = write(TCP_Client_Sockfd, data, len);
	if (n < 0)
	{
		printf("Open_Tcp_Client:writing to server failed\r\n");
		return ESP_FAIL;
	}
	printf("Open_Tcp_Client:writing to server Success with data is (");
	for(i=0;i<n;i++)
	{
		printf("%x ",data[i]);
	}
	printf(")\r\n");
	return ESP_OK;
}
#endif
