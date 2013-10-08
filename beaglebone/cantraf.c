#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>


#define DEFALUT_TIMEOUT 5


const char *can_ifname = "can0";
int can_sock = -1;
struct can_frame can_frame;

long get_current_time()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000L + tv.tv_usec / 1000L;
}

int open_can(){
	struct sockaddr_can addr;
	struct can_frame frame;
	struct ifreq ifr;

	printf("open can bus(%s)...\n", can_ifname);

	if((can_sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("create listening socket error");
		return 0;
	}

	strcpy(ifr.ifr_name, can_ifname);
	ioctl(can_sock, SIOCGIFINDEX, &ifr);

	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex; 

	printf("%s at index %d\n", can_ifname, ifr.ifr_ifindex);

	if(bind(can_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind error");
		return 0;
	}
	printf("open can bus success\n");
	return 1;
}


int main_loop(){
	struct timeval tv;
	fd_set fdsr;
	int ret;
	long recvbytecount = 0;
	long recvallcount = 0;
	long starttime = -1;
	long preshowtime = -1;
	long lasttime = 0;
	for(;;){
		tv.tv_sec = DEFALUT_TIMEOUT;
		tv.tv_usec = 0;
		FD_ZERO(&fdsr);

		FD_SET(can_sock, &fdsr);


		ret = select(can_sock+1, &fdsr, NULL, NULL, &tv);

		if (ret < 0)
		{
			perror("select error!");
			return -1;
		}
		else if (ret == 0 && starttime > 0)
		{
			printf("time: %ldms RX: %ldbytes RXAll:%ldbytes\n", lasttime - starttime, recvbytecount, recvallcount);
			printf("Rate: %.2lfbyte/s  Rate of All: %.2lfbyte/s  %.2lfbps\n", (double)recvbytecount/(double)(lasttime - starttime)*1000.0, (double)recvallcount/(double)(lasttime - starttime)*1000.0, (double)recvallcount/(double)(lasttime - starttime)*8000.0);
			printf("\ntime out, waitting for new statistics\n");
			starttime = -1;
		}
		else if(FD_ISSET(can_sock, &fdsr)){
			if(starttime < 0){
				starttime = get_current_time();
				preshowtime = starttime;
				recvbytecount = 0;
				recvallcount = 0;
				printf("start new statistics\n");
			}

			ret = read(can_sock, &can_frame, sizeof(struct can_frame));
			if(ret<0){
				printf("read can bus fail\n");
			}

			recvbytecount += can_frame.can_dlc;
			recvallcount += ret;
			lasttime = get_current_time();
			if((lasttime - preshowtime)>1000){
				printf("time: %ldms RX: %ldbytes RXAll:%ldbytes\n", lasttime - starttime, recvbytecount, recvallcount);
				preshowtime = lasttime;
			}
		}
	}
}

void sendtest(int count)
{
	long starttime, lasttime, preshowtime;
	long sendcount = 0;
	long errorcount = 0;
	int i, ret;
	lasttime = preshowtime = starttime = get_current_time();
	for(i=0; i<8; ++i){
		can_frame.data[i] = i;
	}
	printf("start test send, count=%d\n", count); 
	for(i=1; i<=count; ++i){
		can_frame.can_dlc = 8;
		can_frame.can_id  = 0x00;
		ret = write(can_sock, &can_frame, sizeof(struct can_frame));
		if(ret>0){
			sendcount += ret;
		}
		else{
			++errorcount;
			printf("errno: %d %s \n", errno, strerror(errno));
		}
		lasttime = get_current_time();
		if((lasttime - preshowtime)>1000){
			printf("time: %ldms TX: %ldbytes TXAll:%ldbytes Error:%ld\n", lasttime - starttime, sendcount/2, sendcount, errorcount);
			preshowtime = lasttime;
		}	
	}
	printf("time: %ldms TX: %ldbytes TXAll:%ldbytes Error:%ld\n", lasttime - starttime, sendcount/2, sendcount, errorcount);
}

int main(int argc, char ** argv)
{
	if(open_can())
	{
		if(argc == 1){
			main_loop();
		}
		else if(argc == 2){
			sendtest(atoi(argv[1]));
		}
	}
	return 0;
}


