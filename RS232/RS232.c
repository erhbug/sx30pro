void serialTx(unsigned char *s){
    while(*s){
        SBUF=*s++;
        while(TI == 0);
        TI =0;
    }
}

void SerialEnable(){
    SCON = 0x50;
    TMOD = 0x20;
    TH1 = TL1 = 0xfd;
    TR1= 1;
}