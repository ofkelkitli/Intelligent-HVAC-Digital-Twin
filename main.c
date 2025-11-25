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

//KARAR VERME MEKANİZMAMIZ
void Decision(Current *now){	

	int random_cycles;
	float random_temp;
	
	
	//Rastgele Sıcaklık Değeri Ataması Normalde Sensöre Bağlı Verilerden Sıcaklık Çekeceğiz
	now->currentTemp =((float)rand() / (float)RAND_MAX)*60; //rand()/RAND_MAX 0.0 ile 1.0 arasında değer verir     
	printf("Su Anki Sicaklik Degeri: %.2f\n",now->currentTemp);
	
	//ALARM DURUMU
	if(now->currentTemp>50||now->currentTemp<0){ //Acil durumda direk kapanması için önce ALARM tanımlandı
		now->currentType=ALARM;
		printf("SU ANDA ALARM DURUMUNDA! SISTEM KENDINI KAPATIYOR\n");
		Sleep(500);
		exit(1);
	}
	
	//DÜŞÜK SICAKLIK DURUMU
	else if(now->currentTemp<15){
		now->currentType=HEATING;
		printf("SU ANDA ISINMA MODUNA GECILIYOR!\n");
		Sleep(500);
		
		//-----------------ISITMA MODU-----------------//
		
		random_cycles = rand()%6+1;
		printf("DENENECEK ISINMA SAYISI: %d\n",random_cycles);//1-6 ARASI RASTGELE ISITMA İŞLEMİ SAYISI (BİLEREK RASTGELE ATANDI! FANLARDA SORUN DURUMU VEYA FAZLA ENERJİ HARCAMAMASI İÇİN MEVCUT ENERJİNİN İSTENEN DERECEDE KULLANILMASI DURUMU GÖZ ÖNÜNE ALINDI!)
		
		Sleep(500);
		
		for(int i=0; i<random_cycles;i++){
			random_temp=((float)rand()/(float)RAND_MAX)*5;//SICAKLIĞI RASTGELE ARTTIRMA... İSTENEN ÇALIŞMA HIZINA GÖRE AYARLANABİLİR.
			now->currentTemp+=random_temp;
			printf("DENEME:%d ISINMA DEVAM EDIYOR! MEVCUT SICAKLIK DEGERI: %.2f\n",i+1,now->currentTemp);
			Sleep(500);
			
			if(now->currentTemp<35&&now->currentTemp>15){ //NORMAL DURUMA GELDİYSE
				now->currentType=IDLE;
				printf("%d.DENEMEDE NORMAL MODA DONDU. DENEMLER DURUYOR!\n",i+1);
				Sleep(500);
				printf("\nNORMAL MODA DONDU SORUNSUZ CALISMAYA DEVAM EDIYOR!\n");
				return;
			}	
			if(now->currentTemp<0||now->currentTemp>50){ //YANLIŞLIKLA FAZLA ISINIRSA VEYA SOĞURSA DURUMU KONTROLÜ (SİSTEM HATALARI VS. YÜZÜNDEN)
				now->currentType=ALARM;
				printf("ALARM MODUNA GECILDI!\n");
				Sleep(500);
				exit(1);
			}
		}
		
		if(now->currentTemp<15){ //ISITMA YETMEDİYSE
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
		printf("DENENECEK SOGUTMA SAYISI: %d\n",random_cycles);//1-6 ARASI RASTGELE SOĞUTMA İŞLEMİ SAYISI (BİLEREK RASTGELE ATANDI! FANLARDA SORUN DURUMU VEYA FAZLA ENERJİ HARCAMAMASI İÇİN MEVCUT ENERJİNİN İSTENEN DERECEDE KULLANILMASI DURUMU GÖZ ÖNÜNE ALINDI!
		Sleep(500);

		
		for(int i=0; i<random_cycles;i++){
			random_temp=((float)rand()/(float)RAND_MAX)*5;//SICAKLIĞI RASTGELE AZALTMA... İSTENEN ÇALIŞMA HIZINA GÖRE AYARLANABİLİR.
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
			if(now->currentTemp>50||now->currentTemp<0){ //YANLIŞLIKLA FAZLA ISINIRSA VEYA SOĞURSA DURUMU KONTROLÜ (SİSTEM HATALARI VS. YÜZÜNDEN)
				now->currentType=ALARM;
				printf("ALARM MODUNA GECILDI!\n");
				Sleep(500);
				exit(1);
			}
		}
		
		if(now->currentTemp>35){ //SOĞUTMA YETMEDİYSE
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
	srand(GetTickCount());

	Current now;
	
	Decision(&now);
	now.currentType = IDLE;
	return 0;
}

