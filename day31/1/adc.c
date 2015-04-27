#define 	ADCTS_BASE			0xE1700000
#define 	TSADCCON0			( *((volatile unsigned long *)(ADCTS_BASE+0x0)) )
#define    	TSCON0				( *((volatile unsigned long *)(ADCTS_BASE+0x4)) )
#define    	TSDLY0				( *((volatile unsigned long *)(ADCTS_BASE+0x8)) )
#define    	TSDATX0				( *((volatile unsigned long *)(ADCTS_BASE+0xc)) )
#define    	TSDATY0				( *((volatile unsigned long *)(ADCTS_BASE+0x10)) )
#define    	TSPENSTAT0			( *((volatile unsigned long *)(ADCTS_BASE+0x14)) )
#define    	CLRINTADC0			( *((volatile unsigned long *)(ADCTS_BASE+0x18)) )
#define  	ADCMUX				( *((volatile unsigned long *)(ADCTS_BASE+0x1c)) )
#define		CLRINTPEN0			( *((volatile unsigned long *)(ADCTS_BASE+0x20)) )

void uart_init(void);
char uart_recv_byte(void);
void uart_send_byte(char c);
void uart_send_string(const char const *str);
void uart_send_num(unsigned int num);

// ��ʱ����
void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--)
		;
}

// ʹ�ò�ѯ��ʽ��ȡA/Dת��ֵ
int read_adc(int ch)
{
    // ʹ��Ԥ��Ƶ���ܣ�����A/Dת������ʱ�� = PCLK/(65+1)
	TSADCCON0 = (1<<16)|(1 << 14) | (65 << 6);

    // ���λ[2]����Ϊ��ͨת��ģʽ,��ֹread start
	TSADCCON0 &= ~((1<<2)|(1<<1));

	// ѡ��ͨ��
	ADCMUX = 0;

    // ����λ[0]Ϊ1������A/Dת��
    TSADCCON0 |= (1 << 0);

    // ��A/Dת��������ʼʱ��λ[0]���Զ���0
    while (TSADCCON0 & (1 << 0));

    // ���λ[15]������Ϊ1ʱ��ʾת������
    while (!(TSADCCON0 &  (1 << 15)) );

    // ��ȡ����
    return (TSDATX0 & 0xfff);
}

void adc_test(void)
{
	uart_send_string("\r\n##################adc test################\r\n");
	while(1)
	{	
		int R = read_adc(0);
		uart_send_string("R=");
		uart_send_num(R);
		delay(0x500000);
	}
}