//  05100000099- SEFIKA EFEOGLU 16/03/2012
#include <stdio.h>// kutuphane tanimlari
#include <stdlib.h>
#include<string.h>
#define MAX_KELIME_UZUNLUGU 21
#define MAX_UCUS 1000
void ciz();
void baslik();// baslyk yazdyrma fonksiyonu
int ucus_no_kontrol();// kullanicidan ucus no alip kontrol eder
int secim(void);// menu seceneklerini yazar
void bir_ucus_bilgi_ve_iliskin_bilet_liste(FILE *,FILE*);// dat tan ucus bilgilerini txt den bilet listesiini yazar
void koltuk_doluluk_orani_listesi(FILE *);// %50 nin altynda ki ucuslaryn doluluk oranini artana sirada yazar
void bir_yolcu_bilet_listesi(FILE *,FILE *);// bir yolcuya ait bilet listesini yazar
int ucus_noya_gore_ara(FILE *,int);// numaraya gore arama islemi yapar
void kalkis_yerine_gore_arama(FILE *);// kalkis yerine gore arama islemi yapar
struct dat_degisken_ucus
{
    int ucusNo;
    char kalkisYeri[MAX_KELIME_UZUNLUGU];
    char varisYeri[MAX_KELIME_UZUNLUGU];
    int kalkisSaati;
    int kalkisDakikasi;
    int koltukKapasitesi;
    int bosKoltukSayisi;
    float baslangic_BiletFiyati;
};
typedef struct dat_degisken_ucus ucus_type;
void yazdir(FILE *Dfptr,int *dizi,int a,ucus_type bir_ucus)// fonksiyonlarda hesaplanan degerleriyazdirir
{
    int i=0;
    float doluluk;
    baslik();
    for(i=0;  i<a ;   i++)
    {
        fseek(Dfptr, (dizi[i]-1) * sizeof(ucus_type),SEEK_SET);//dizideki  degerin oldugu kayıda gider
        fread(&bir_ucus, sizeof(ucus_type),1,Dfptr);// okur ve hesapladıktan sonra asagıda yazar
        doluluk=(float)(bir_ucus.koltukKapasitesi-bir_ucus.bosKoltukSayisi)*100/bir_ucus.koltukKapasitesi;
        printf("%3d %13s %13s %6.2d:%.2d %10d %10d%15.2f\n",bir_ucus.ucusNo,bir_ucus.kalkisYeri,bir_ucus.varisYeri,bir_ucus.kalkisSaati,bir_ucus.kalkisDakikasi,bir_ucus.koltukKapasitesi,bir_ucus.bosKoltukSayisi,doluluk);
    }
}
int main()//HAVA ALANI UCUS SORGULAMA PROGRAMI
{

    FILE *Dfptr,*Tfptr;
    int secenek, girilen_ucus_no;
    int bulundu=1;

    if((Dfptr=fopen("ucus.dat","rb+"))==NULL||(Tfptr=fopen("bilet.txt","r+"))==NULL)// dosyaların acılmasını kontrol eder
    {
        if(Dfptr==NULL)
            printf("\t\tUcus bilgilerinin oldugu dosya acilamadi\a\n");// dat acilmazsa
        else
            printf("\t\tBilet bilgilerinin oldugu dosya acilamadi\a\n");//txt acilmazsa
        return 0;// dosya acilmazsa program bitirilir
    }
    do// dosyaların ikiside acilirsa islemler yapilir
    {
        secenek=secim();
        switch(secenek)
        {
        case 1:
            girilen_ucus_no=ucus_no_kontrol();// no kontrolden ucus no alinir ve girilen noya atanir
            bulundu=ucus_noya_gore_ara(Dfptr,girilen_ucus_no);// ucus noya gore ara fonksiyonuna dat ve girilen no gonderilir
            if(bulundu==1)
                printf("\t\tDosyada bu ucus numarasina \n\t\tait bir kayit bulunamadi\n");
            break;
        case 2:
            bir_ucus_bilgi_ve_iliskin_bilet_liste(Dfptr,Tfptr);//bir ucusa ait bilgileri ve kimlerin aldıgını listeler
            break;
        case 3:
            kalkis_yerine_gore_arama(Dfptr);//kalkıs yerine gore arama yapar
            break;
        case 4:
            koltuk_doluluk_orani_listesi(Dfptr);//artan sekilde koltuk doluluk oarani hesaplar
            break;
        case 5:
            bir_yolcu_bilet_listesi(Dfptr,Tfptr);// bir yolcuya ait biletleri listeler
            break;
        }
    }
    while(secenek!=6);
    fclose(Dfptr);
    fclose(Tfptr);
    printf("Sistemden ciktiniz\n");
    return 0;
}// dosyalar kapatılır hayat devam eder...:D
int secim(void)// menu secenekleri ana fonksiyona deger dondurur.
{
    int secenek;
    printf("\t\t\t*****MENU SECENEKLERI*****\n\n");
    printf("\t\t1.Bir Ucusun Bilgilerinin Listelenmesi\n");
    printf("\t\t2.Bir Ucusun Bilgilerinin ve\n\t\t o Ucusa Iliskin Satilan Biletlerin Listelenmesi\n");
    printf("\t\t3.Bir Yerden Kalkan Ucuslarin Bilgilerinin Listelenmesi\n");
    printf("\t\t4.Koltuk Doluluk Orani %% 50 nin Altinda Olan Ucuslarin Listelenmesi\n");
    printf("\t\t5.Bir Yolcunun Biletlerinin Listelenmesi\n");
    printf("\t\t6.Cikis\n");
    do
    {
        printf("\t\tMenudeki secimlerden birini giriniz(1-6 arasi):");
        scanf("%d",&secenek);// secim alir
        if(secenek<1||secenek>6)
            printf("\t\tSecilen secenek 1 ile 6 arasinda olmalidir\a\n");// yanlis secimde mesaj verir
    }
    while(secenek<1||secenek>6);// dogru secim oluncaya kadar doner
    ciz();
    return secenek;// secenegi  ana fonksiyona gonderir
}
int ucus_no_kontrol()// kullanicinin girdigi ucus numarasini kontrol eder
{
    int girilen_ucusNo;
    do
    {
        printf("\t\tBilgilerini Gormek Istediginiz \n\t\tUcusun Numarasini Giriniz(1-1000):");
        scanf("%d",&girilen_ucusNo);// kullanicidan ucus no alir
        if(girilen_ucusNo<1||girilen_ucusNo>MAX_UCUS)
            printf("\t\tUcus Numarasi 1 ile 1000 arasinda olmaliydi\a\n");// yanlis ise mesaj verir
    }
    while(girilen_ucusNo<1||girilen_ucusNo>MAX_UCUS);// dogru giris oluncaya kadar doner
    return girilen_ucusNo;// ANA VE YARDIMCI FONKSİYONA GIRILEN UCUS NO DONDURULUR
}

void baslik()//Baslik yazdirma fonksiyonu
{
    printf("\n\nUcus No   Kalkis Yeri   Varis Yeri  Zamani    Kapasite  Bos Koltuk   Doluluk(%%)\n");
    printf("-------   -----------   ----------  ------    --------  ----------  ---------\n");
    return;
}
void bir_ucus_bilgi_ve_iliskin_bilet_liste(FILE *Dfptr,FILE *dosyat)//girilen ucus no ya gore dat dan bilgi okur txt den bilet listesi verir
{
    int toplam_bilet_sayisi=0,ucus_no_T,bulundu=1,girilen_ucus_no;
    float toplam_kazanc=0,bilet_fiyati;
    char tc[12];
    rewind(dosyat);
    girilen_ucus_no=ucus_no_kontrol();
    if(bulundu==ucus_noya_gore_ara(Dfptr,girilen_ucus_no))// ucus noya ait kayit yoksa datta mesaj verir
        printf("\t\tGirilen ucus numarasina ait \n\t\tbilgi bulunamamaktadir\a\n");
    else// eger kayit varsa
    {
        fscanf(dosyat,"%d%s%f",&ucus_no_T,tc,&bilet_fiyati);// txt den ucus noya gore alynan bilettler yazdyrylyr
        printf("\t\tTC KIMLIK NO. BILET FIYATI\n");
        printf("\t\t------------- ------------\n");
        while(!feof(dosyat))
        {
            if(girilen_ucus_no==ucus_no_T)// girilen ucus no ile txt den ucus nolar karsilasttirilir ve varsa yazdyrylyr
            {
                printf("\t\t%s    %.2f\n",tc,bilet_fiyati);//biletlerin TC kimlik no ve bilet fiy.ti yazimi
                toplam_bilet_sayisi++;
                toplam_kazanc+=bilet_fiyati;
            }
            fscanf(dosyat,"%d%s%f",&ucus_no_T,tc,&bilet_fiyati);
        }
        printf("\n\t\tToplam Bilet Sayisi  : %d\n",toplam_bilet_sayisi);// bu ucusa ait alinan bilet sayisi yazimi
        printf("\n\t\tToplam Kazanc        : %.2fTL\n",toplam_kazanc);//toplam bilet fiyati yazimi
        printf("\n\t\tOrtalama Bilet Fiyati: %.2fTL\n",(float)toplam_kazanc/toplam_bilet_sayisi);// ortalama bilet fiyati yazimi
    }

}
int ucus_noya_gore_ara(FILE *Dfptr,int girilen_ucus_no)//1. ve 2. secenekler icin kullanilir
{
    int bulundu=1;
    ucus_type bir_ucus;
    float doluluk;
    fseek(Dfptr, (girilen_ucus_no - 1) * sizeof(ucus_type), SEEK_SET);// ucus noya gore o satira gider
    fread(&bir_ucus,sizeof(ucus_type),1,Dfptr);
    if(bir_ucus.ucusNo!=0)// eger dolu  kayit ise
    {
        bulundu=0;// sifir dondurur
        baslik();// baslik yazar alttaki doluluk hesaplar bir sonrakide yazar
        doluluk=(float)(bir_ucus.koltukKapasitesi-bir_ucus.bosKoltukSayisi)*100/bir_ucus.koltukKapasitesi;
        printf("%3d %13s %13s %6.2d:%.2d %10d %10d%15.2f\n",bir_ucus.ucusNo,bir_ucus.kalkisYeri,bir_ucus.varisYeri,bir_ucus.kalkisSaati,bir_ucus.kalkisDakikasi,bir_ucus.koltukKapasitesi,bir_ucus.bosKoltukSayisi,doluluk);
    }
    return bulundu;
}
void kalkis_yerine_gore_arama(FILE *Dfptr)// kullanicinin bilgilerini gormek istedigi kalkis yerinibulur
{
    char girilen_kalkis[21];
    ucus_type bir_ucus;
    int i ,bulundu=0, dizi[MAX_UCUS],a=0;
    printf("\n\t\tBilgilerini gormek istediginiz\n\t\t kalkis yerini giriniz:");
    fflush(stdin);
    gets(girilen_kalkis);// kullanici kalkis yeri girer
    for(i=0; i<1000; i++)
    {
        fseek(Dfptr,i * sizeof(ucus_type),SEEK_SET);//girilen kalkis dat da var my
        fread(&bir_ucus, sizeof(ucus_type), 1,Dfptr);// okunur syra ile
        if(strcmp(bir_ucus.kalkisYeri,girilen_kalkis)==0)// girilen iile dosyadaki karsilastirilir
        {
            bulundu=1;// varsa bulundu 1 olur
            dizi[a]=bir_ucus.ucusNo;// diziye ucus no atilmistir
            a++;
        }
    }
    if(bulundu==0)// eger datta varsa
    {
        printf("\t\t\nBu yere ait kalkis dosyada bulunmamaktadir\a\n");// mesaj
    }
    yazdir(Dfptr,dizi,a,bir_ucus);
    return;
}
void bir_yolcu_bilet_listesi(FILE *Dfptr,FILE *Tfptr)//bir yolcunun bilet listesini yapar Kullnici TC Kimlik no girer
{
    int ucus_no_t,bulundu=0;
    ucus_type bir_ucus;
    char tc[12],tc_kul[12];
    float bilet_fiyati_t;
    rewind(Tfptr);
    printf("\t\tBiletlerini listelemek istediginiz\n\t\t kullanicinin TC Kimlik Numarasini giriniz:");
    fflush(Tfptr);
    scanf("%s",tc_kul);// kullanici TC Kimlik no girer
    fscanf(Tfptr,"%d %s %f",&ucus_no_t,tc,&bilet_fiyati_t);// txt den tc okunur
    printf("\n\nUCUS NO    KALKIS YERI   VARIS YERI ZAMANI     FIYATI\n");
    printf("-------    -----------   ----------  ------    --------\n");
    while(!feof(Tfptr))// txt sonu na kadar arama yapar
    {
        if(strcmp(tc,tc_kul)==0)// girilen ile okunan ayni ise
        {
            fseek(Dfptr, (ucus_no_t-1)*sizeof(ucus_type),SEEK_SET);
            fread(&bir_ucus,sizeof(ucus_type),1,Dfptr);
            bulundu=1;// kayit varsa 1 atar
            printf("%3d %13s %13s %6.2d:%.2d %13.2fTL\n",bir_ucus.ucusNo,bir_ucus.kalkisYeri,bir_ucus.varisYeri,bir_ucus.kalkisSaati,bir_ucus.kalkisDakikasi,bilet_fiyati_t);
        }
        fscanf(Tfptr,"%d %s %f",&ucus_no_t,tc,&bilet_fiyati_t);
    }
    if(bulundu==0)// Kayit yoksa
    {
        printf("%3d %13s %13s %6.2d:%.2d %13.2fTL\n",0,"-","-",00,00,0.00);
        printf("\n\n\t\tBu TC kimlik numarasina iliskin kayit bulunamadi\a\n");
    }
}
void koltuk_doluluk_orani_listesi(FILE *Dfptr)// koltuk doluluk oranini hesaplar % 50 nin altinda olanlari artan syraya gore yazar gunlerimi yedi:D
{
    ucus_type bir_ucus;
    float doluluk2,doluluk;
    int sayac,sayac1,bos1,bos2,a=0,i,gecici;
    int dizi[MAX_UCUS]= {};
    for(i = 0; i < MAX_UCUS; i++)
    {
        fseek(Dfptr, i * sizeof(ucus_type), SEEK_SET);
        fread(&bir_ucus, sizeof(ucus_type), 1, Dfptr);
        doluluk=(float)(bir_ucus.koltukKapasitesi-bir_ucus.bosKoltukSayisi)*100/bir_ucus.koltukKapasitesi;
        if(bir_ucus.ucusNo!=0 && doluluk<50.00)//%50.00 altında olanları diziye atar
        {
            dizi[a]=bir_ucus.ucusNo;
            a++;
        }
    }
    for(sayac=0; sayac<a; sayac++) // 1. for dongusu
    {
        fseek(Dfptr, (dizi[sayac]-1) * sizeof(ucus_type),SEEK_SET);// sayac ca gore uygun kayıda gider
        fread(&bir_ucus, sizeof(ucus_type),1,Dfptr);// okur
        doluluk=(float)(bir_ucus.koltukKapasitesi-bir_ucus.bosKoltukSayisi)*100/bir_ucus.koltukKapasitesi;
        bos1=bir_ucus.bosKoltukSayisi;
        for(sayac1=sayac+1; sayac1<a; sayac1++)
        {
            fseek(Dfptr, (dizi[sayac1]-1) * sizeof(ucus_type),SEEK_SET);// sayac ca gore uygun kayıda gider
            fread(&bir_ucus, sizeof(ucus_type),1,Dfptr);// okur
            doluluk2=(float)(bir_ucus.koltukKapasitesi-bir_ucus.bosKoltukSayisi)*100/bir_ucus.koltukKapasitesi;
            bos2=bir_ucus.bosKoltukSayisi;
            if(doluluk>doluluk2)// büyük olanın yerini degistirir
            {
                gecici=dizi[sayac];// SWAP YAPIYORUZ
                dizi[sayac]=dizi[sayac1];
                dizi[sayac1]=gecici;
                doluluk=doluluk2;
                bos1=bos2;
            }
            if(doluluk==doluluk2 && bos2>bos1)// esit ise bos oranına gore yer degistirilir
            {
                gecici=dizi[sayac];
                dizi[sayac]=dizi[sayac1];
                dizi[sayac1]=gecici;
                bos1=bos2;
            }
        }
    }
    yazdir(Dfptr,dizi,a,bir_ucus);
}
void ciz()
{
    printf("*****************************************************************************\n");
}
