using TMPro;
using UnityEngine;
using System.Collections;
using UnityEditor;

public class LastSystemController : MonoBehaviour
{
    //DURUMLARIMIZ
    public enum Statement
    {
        IDLE,
        HEATING,
        COOLING,
        ALARM
    }

    //MEVCUT DURUM YAPISI
    [System.Serializable]
    public struct CurrentStatus
    {
        public float currentTemp;
        public Statement currentStatement;
    }

    //DEÐÝÞKENLER
    public TMP_Text tempText;
    public TMP_Text statusText;
    public TMP_InputField tempInput;
    public TMP_Text currentOperationText;
    public GameObject Cube;
    Renderer cubeR;

    //BEKLEME SÜREMÝZ
    WaitForSeconds wait = new WaitForSeconds(1f);
    WaitForSeconds longWait = new WaitForSeconds(2.5f);

    public CurrentStatus now = new CurrentStatus();

    float timer;
    float activeInterval = 10.0f;

    int random_cycles;
    float random_temp;


    //KARAR VERME MEKANÝZMAMIZ/FONKSÝYONUMUZ
    public void Decision()
    {
        //ALARM DURUMU
        if (now.currentTemp < 0.0f || now.currentTemp > 50.0f)
        {
            now.currentStatement = Statement.ALARM;
            Debug.Log("Alarm durumuna geçildi! Sistem kendini kapatýyor!");
            currentOperationText.text = "Alarm durumuna geçildi! Sistem kendini kapatýyor! Mevcut sýcaklýk: " + now.currentTemp.ToString("F1");
            StopAllCoroutines();
            StartCoroutine(Kapatma());
            return;
        }

        //DÜÞÜK SICAKLIK ÝÇÝN ISITMA MODU
        else if (now.currentTemp < 15)
        {
            StopAllCoroutines();
            now.currentStatement = Statement.HEATING;
            Debug.Log("Isýtma modu etkinleþtirildi!");
            currentOperationText.text = "Isýtma modu etkinleþtirildi!";

            //-------ISITMA MODU-------//

            StartCoroutine(IsýtmaModu());
        }

        //YÜKSEK SICAKLIK ÝÇÝN SOÐUTMA MODU
        else if (now.currentTemp > 35)
        {
            StopAllCoroutines();
            now.currentStatement = Statement.COOLING;
            Debug.Log("Soðutma modu etkinleþtirildi!");
            currentOperationText.text = "Soðutma modu etkinleþtirildi!";

            //-------SOÐUTMA MODU-------//

            StartCoroutine(SogutmaModu());
        }

        //NORMAL SICAKLIK ARALIÐI ÝÇÝN BOÞTA MOD
        else if (now.currentTemp >= 15 && now.currentTemp <= 35)
        {
            StopAllCoroutines();
            now.currentStatement = Statement.IDLE;
            Debug.Log("Sistem Normal modda çalýþýyor!");
            currentOperationText.text = "Sistem Normal modda çalýþýyor! Mevcut sýcaklýk: " + now.currentTemp.ToString("F1");
        }       
    }


    void Start()
    {
        now.currentTemp=Random.Range(0.0f,60.0f); //Normalde Sensörlerden Gelecek Sýcaklýk Deðeri Ýçin Rastgele Baþlangýç Deðeri Atandý
        timer = 9.0f; //Baþlangýçta Karar Verme Mekanizmasýnýn Hemen Çalýþmasý Ýçin
        cubeR = Cube.GetComponent<Renderer>();
    }

    void Update()
    {
        timer += Time.deltaTime; //Her Saniye Zamanlayýcýyý Arttýrmak Ýçin

        //Kullanýcýdan Sýcaklýk Giriþi Almak Ýçin
        if (tempInput != null)
        {
            if (Input.GetKeyDown(KeyCode.Return))
            {
                float inputTemp;
                if (float.TryParse(tempInput.text, out inputTemp))
                {
                    now.currentTemp = inputTemp;
                    timer = 9.0f;
                    tempInput.text = "";
                }
            }
        }

        //Karar Verme Mekanizmasý Ýçin Zamanlayýcý Kontrolü
        if (timer >= activeInterval)
        {
            Decision();
            timer = 0.0f;
        }

        ColorChanger();//Renk Deðiþtirme Fonksiyonu
        tempText.text = "Sýcaklýk: " + now.currentTemp.ToString("F1") + " °C"; //Sýcaklýk Bilgisini Ekranda Gösterme
        statusText.text = "Durum: " + now.currentStatement.ToString(); //Durum Bilgisini Ekranda Gösterme
    }

    //RENK DEÐÝÞTÝRME FONKSÝYONU
    void ColorChanger()
    {
        switch (now.currentStatement)
        {
            case Statement.ALARM: //ALARM DURUMU VAR => RENK SÝYAH
                cubeR.material.color = Color.black;
                break;
            case Statement.HEATING: //DÜÞÜK SICAKLIK VAR => RENK MAVÝ (ALARM VERMEDÝYSE)
                if (now.currentStatement != Statement.ALARM)
                {
                    cubeR.material.color = Color.blue;
                }
                break;
            case Statement.COOLING: //FAZLA SICAKLIK VAR => RENK KIRMIZI (ALARM VERMEDÝYSE)
                if (now.currentStatement != Statement.ALARM)
                {
                    cubeR.material.color = Color.red;
                }
                    break;
            case Statement.IDLE: //NORMAL SICAKLIK VAR => RENK YEÞÝL
                cubeR.material.color = Color.green;
                break;

        }
    }
    //SÝSTEM KAPATMA FONKSÝYONU
    IEnumerator Kapatma()
    {
        Debug.Log("Sistem kendini kapatýyor...");
        currentOperationText.text = "Sistem kendini kapatýyor...";
        yield return longWait;
        if (Application.isEditor) //Editör Modunda Çalýþýyorsa
        {
            EditorApplication.isPlaying = false;
        }
        else //Build Olarak Çalýþýyorsa
        {
            Application.Quit();
        }
    }

    //ISITMA MODU FONKSÝYONU
    public IEnumerator IsýtmaModu()
    {
        random_cycles = Random.Range(1, 7); //1-6 ARASI RASTGELE ISITMA ÝÞLEMÝ SAYISI (BÝLEREK RASTGELE ATANDI! FANLARDA SORUN DURUMU VEYA FAZLA ENERJÝ HARCAMAMASI ÝÇÝN MEVCUT ENERJÝNÝN ÝSTENEN DERECEDE KULLANILMASI DURUMU GÖZ ÖNÜNE ALINDI!
        for (int i = 0; i < random_cycles; i++)
        {
            random_temp = Random.Range(0f, 5f);
            now.currentTemp += random_temp;//SICAKLIÐI RASTGELE ARTTIRMA... ÝSTENEN ÇALIÞMA HIZINA GÖRE AYARLANABÝLÝR.
            Debug.Log("Isýtma iþlemi devam ediyor... Mevcut sýcaklýk: " + now.currentTemp);
            currentOperationText.text = "Isýtma iþlemi devam ediyor... Mevcut sýcaklýk: " + now.currentTemp.ToString("F1");
            yield return wait;
        }
        if (now.currentTemp >= 15 && now.currentTemp <= 35) //NORMAL SICAKLIK ARALIÐINA GELDÝYSE
        {
            now.currentStatement = Statement.IDLE;
            Debug.Log("Sistem normal moda geçti! Mevcut sýcaklýk: " + now.currentTemp);
            currentOperationText.text = "Sistem normal moda geçti! Mevcut sýcaklýk: " + now.currentTemp.ToString("F1");
            yield break;
        }
        if (now.currentTemp > 50 || now.currentTemp < 0) //YANLIÞLIKLA ALARM DURUMUNA GELDÝYSE
        {
            now.currentStatement = Statement.ALARM;
            Debug.Log("Sýcaklýk çok yükseldi! Alarm durumuna geçiliyor!");
            currentOperationText.text = "Sýcaklýk çok yükseldi! Alarm durumuna geçiliyor!";
            StartCoroutine(Kapatma());
        }
        if (now.currentTemp< 15) //ISITMA YETERLÝ OLMADIYSA
        {
            now.currentStatement = Statement.ALARM;
            Debug.Log("Isýtma yeterli olmadý! Alarm durumuna geçiliyor!");
            currentOperationText.text = "Isýtma yeterli olmadý! Alarm durumuna geçiliyor!";
            StartCoroutine(Kapatma());
        }
    }

    //SOÐUTMA MODU FONKSÝYONU
    public IEnumerator SogutmaModu()
    {
        random_cycles = Random.Range(1, 7);//1-6 ARASI RASTGELE ISITMA ÝÞLEMÝ SAYISI (BÝLEREK RASTGELE ATANDI! FANLARDA SORUN DURUMU VEYA FAZLA ENERJÝ HARCAMAMASI ÝÇÝN MEVCUT ENERJÝNÝN ÝSTENEN DERECEDE KULLANILMASI DURUMU GÖZ ÖNÜNE ALINDI!
        for (int i = 0; i < random_cycles; i++)
        {
            random_temp = Random.Range(0f, 5f);
            now.currentTemp -= random_temp;//SICAKLIÐI RASTGELE AZALTMA... ÝSTENEN ÇALIÞMA HIZINA GÖRE AYARLANABÝLÝR.
            Debug.Log("Soðutma iþlemi devam ediyor... Mevcut sýcaklýk: " + now.currentTemp);
            currentOperationText.text = "Soðutma iþlemi devam ediyor... Mevcut sýcaklýk: " + now.currentTemp.ToString("F1");
            yield return wait;
        }
        if (now.currentTemp >= 15 && now.currentTemp <= 35)//NORMAL SICAKLIK ARALIÐINA GELDÝYSE
        {
            now.currentStatement = Statement.IDLE;
            Debug.Log("Sistem normal moda geçti! Mevcut sýcaklýk: " + now.currentTemp);
            currentOperationText.text = "Sistem normal moda geçti! Mevcut sýcaklýk: " + now.currentTemp.ToString("F1");
            yield break;
        }
        if (now.currentTemp > 50 || now.currentTemp < 0)//YANLIÞLIKLA ALARM DURUMUNA GELDÝYSE
        {
            now.currentStatement = Statement.ALARM;
            Debug.Log("Sýcaklýk çok düþtü! Alarm durumuna geçiliyor!");
            currentOperationText.text = "Sýcaklýk çok düþtü! Alarm durumuna geçiliyor!";
            StartCoroutine(Kapatma());
        }
        if (now.currentTemp > 35)//SOÐUTMA YETERLÝ OLMADIYSA
        {
            now.currentStatement = Statement.ALARM;
            Debug.Log("Soðutma yeterli olmadý! Alarm durumuna geçiliyor!");
            currentOperationText.text = "Soðutma yeterli olmadý! Alarm durumuna geçiliyor!";
            StartCoroutine(Kapatma());
        }
    }
}
