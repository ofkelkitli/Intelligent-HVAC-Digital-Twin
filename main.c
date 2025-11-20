#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

//DURUMLARIMIZ
typedef enum{
	IDLE,
	HEATING,
	COOLING,
	ALARM
} Types;

//MEVCUT DURUM YAPISI
typedef struct{
	float currentTemp;
	Types currentType;
} Current;

/* *now ifadesi bu fonksiyondaki deðiþkenlerin kopyasal deðiþimi deðil gerçek deðer deðiþimini gösterir.
Kullanýmý now->deðiþken þeklinde ve main fonksiyonunda çaðýrýrken &now olaraktýr. */

//KARAR VERME MEKANÝZMAMIZ
void Decision(Current *now){	

	int random_cycles;
	float random_temp;
	
	
	//Rastgele Sýcaklýk Deðeri Atamasý Normalde Sensöre Baðlý Verilerden Sýcaklýk Çekeceðiz
	now->currentTemp =((float)rand() / (float)RAND_MAX)*60; //rand()/RAND_MAX 0.0 ile 1.0 arasýnda deðer verir     
	printf("Su Anki Sicaklik Degeri: %.2f\n",now->currentTemp);
	
	//ALARM DURUMU
	if(now->currentTemp>50||now->currentTemp<0){ //Acil durumda direk kapanmasý için önce ALARM tanýmlandý
		now->currentType=ALARM;
		printf("SU ANDA ALARM DURUMUNDA! SISTEM KENDINI KAPATIYOR\n");
		Sleep(500);
		exit(1);
	}
	
	//DÜÞÜK SICAKLIK DURUMU
	else if(now->currentTemp<15){
		now->currentType=HEATING;
		printf("SU ANDA ISINMA MODUNA GECILIYOR!\n");
		Sleep(500);
		
		//-----------------ISITMA MODU-----------------//
		
		random_cycles = rand()%6+1;
		printf("DENENECEK ISINMA SAYISI: %d\n",random_cycles);//1-6 ARASI RASTGELE ISITMA ÝÞLEMÝ SAYISI (BÝLEREK RASTGELE ATANDI! FANLARDA SORUN DURUMU VEYA FAZLA ENERJÝ HARCAMAMASI ÝÇÝN MEVCUT ENERJÝNÝN ÝSTENEN DERECEDE KULLANILMASI DURUMU GÖZ ÖNÜNE ALINDI!)
		
		Sleep(500);
		
		for(int i=0; i<random_cycles;i++){
			random_temp=((float)rand()/(float)RAND_MAX)*5;//SICAKLIÐI RASTGELE ARTTIRMA... ÝSTENEN ÇALIÞMA HIZINA GÖRE AYARLANABÝLÝR.
			now->currentTemp+=random_temp;
			printf("DENEME:%d ISINMA DEVAM EDIYOR! MEVCUT SICAKLIK DEGERI: %.2f\n",i+1,now->currentTemp);
			Sleep(500);
			
			if(now->currentTemp<35&&now->currentTemp>15){ //NORMAL DURUMA GELDÝYSE
				now->currentType=IDLE;
				printf("%d.DENEMEDE NORMAL MODA DONDU. DENEMLER DURUYOR!\n",i+1);
				Sleep(500);
				printf("\nNORMAL MODA DONDU SORUNSUZ CALISMAYA DEVAM EDIYOR!\n");
				return;
			}	
			if(now->currentTemp<0||now->currentTemp>50){ //YANLIÞLIKLA FAZLA ISINIRSA VEYA SOÐURSA DURUMU KONTROLÜ (SÝSTEM HATALARI VS. YÜZÜNDEN)
				now->currentType=ALARM;
				printf("ALARM MODUNA GECILDI!\n");
				Sleep(500);
				exit(1);
			}
		}
		
		if(now->currentTemp<15){ //ISITMA YETMEDÝYSE
			now->currentType=ALARM;
			printf("ISINMA YETERSIZ GELDI! SISTEM KENDINI KAPATIYOR!");
			Sleep(500);
			exit(1);
		}

		
		
	}
	
	//YÜKSEK SICAKLIK DURUMU
	else if(now->currentTemp>35){
		now->currentType=COOLING;
		printf("SU ANDA SOGUTMA MODUNA GECILIYOR!\n");
		Sleep(500);
		
		//-----------------SOGUTMA MODU-----------------//
		
		random_cycles = rand()%6+1;
		printf("DENENECEK SOGUTMA SAYISI: %d\n",random_cycles);//1-6 ARASI RASTGELE SOÐUTMA ÝÞLEMÝ SAYISI (BÝLEREK RASTGELE ATANDI! FANLARDA SORUN DURUMU VEYA FAZLA ENERJÝ HARCAMAMASI ÝÇÝN MEVCUT ENERJÝNÝN ÝSTENEN DERECEDE KULLANILMASI DURUMU GÖZ ÖNÜNE ALINDI!
		Sleep(500);

		
		for(int i=0; i<random_cycles;i++){
			random_temp=((float)rand()/(float)RAND_MAX)*5;//SICAKLIÐI RASTGELE AZALTMA... ÝSTENEN ÇALIÞMA HIZINA GÖRE AYARLANABÝLÝR.
			now->currentTemp-=random_temp;
			printf("DENEME:%d SOGUTMA DEVAM EDIYOR! MEVCUT SICAKLIK DEGERI: %.2f\n",i+1,now->currentTemp);
			Sleep(500);
			
			if(now->currentTemp<35&&now->currentTemp>15){
				now->currentType=IDLE;
				printf("%d.DENEMEDE NORMAL MODA DONDU. DENEMLER DURUYOR!\n",i+1);
				Sleep(500);
				printf("\nNORMAL MODA DONDU SORUNSUZ CALISMAYA DEVAM EDIYOR!\n");
				return;
			}	
			if(now->currentTemp>50||now->currentTemp<0){ //YANLIÞLIKLA FAZLA ISINIRSA VEYA SOÐURSA DURUMU KONTROLÜ (SÝSTEM HATALARI VS. YÜZÜNDEN)
				now->currentType=ALARM;
				printf("ALARM MODUNA GECILDI!\n");
				Sleep(500);
				exit(1);
			}
		}
		
		if(now->currentTemp>35){ //SOÐUTMA YETMEDÝYSE
			now->currentType=ALARM;
			printf("SOGUTMA YETERSIZ GELDI! SISTEM KENDINI KAPATIYOR!");
			Sleep(500);
			exit(1);
		}
	}		

	
	else if(now->currentTemp<35&&now->currentTemp>15){ //NORMAL MOD
		now->currentType=IDLE;
		printf("SU ANDA NORMAL MODDA CALISMAYA DEVAM EDIYOR!\n ");
	}
}

int main(void) {	
	srand(GetTickCount()); //En güvenilir rastgele atama her milisaniyede çalýþtýrsam bile yine farklý deðerler verecektir

	Current now;
	
	Decision(&now); // & Sebebi Decision fonksiyonundaki deðerlerin pseudo olarak deðil gerçekten deðiþmesini saðlamaktýr
	now.currentType = IDLE;
	return 0;
}
