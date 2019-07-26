#include <stdio.h>
#include <stdlib.h>
void ucusun_zamaninin_degistirilmesi(FILE *);//zaman degistirme fonksiyonu,
void ucus_no_al_ekle(FILE *);//ucus ekleme fonksiyonu
int secim();//menu secimleri icin
int ucus_no_kontrol();// girilen ucus numarasini kontrol eder
void baslik();//baslik yazdirir
void  bir_ucusun_kaydinin_silinmesi(FILE *, FILE *);//dosyada olan bir ucusu siler
void bir_ucusa_iliskin_bilet_satimi(FILE *,FILE *);
void ciz();
struct ucus //Dfptr dosyasýnýn sahalarini okumak icin kullanilir
{
    int ucus_no;
    char kalkisYeri[21];
    char VarisYeri[21];
    int KalkisSaati;
    int KalkisDakikasi;
    int KoltukKapasitesi;
    int BosKoltukSayisi;
    float bas_bilet_fiy;
};
typedef struct ucus ucus_type;// struct ucus un adini degistirdim
int main()
{
    FILE * Dfptr, *Tfptr;
    int secenek;

    Dfptr=fopen("ucus.dat","rb+");//dat uzantili dosyanin acilmasi
    Tfptr=fopen("bilet.txt","a+");//txt uzantili dosyani acilmasi
    if(Dfptr==NULL||Tfptr==NULL)//dosya acildimi kontrolu
    {

        printf("Dosya Acilamadi\n");//acilmadi mesaji
        return 0;
    }
    else //dosya acilirsa
    {
        do
        {
            secenek=secim();//menu seceneklerinin secilmesi
            switch(secenek)
            {
            case 1:
                ucus_no_al_ekle(Dfptr);//ucus ekelme fonksiyonunun cagirilmasi
                break;
            case 2:
                bir_ucusun_kaydinin_silinmesi(Dfptr,Tfptr);// ucus sileme fonksiyonun cagirilmasi
                break;
            case 3:
                ucusun_zamaninin_degistirilmesi(Dfptr);//ucus zamani degistirme fonksiyonunun cagirilmasi
                break;
            case 4:
                bir_ucusa_iliskin_bilet_satimi(Dfptr,Tfptr);
                break;
            }

        }
        while(secenek!=5);
    }
    fclose(Dfptr);//cikis islemi sonrasi dosyalarin kapatilmasi
    fclose(Tfptr);//cikis islemi sonrasi dosyalarin kapatilmasi
    printf("Sistem kapatilmistir\n");
    return 0;
}
int secim()//menuden secim yapmak için menu yazimi ve secim alimi
{
    int secenek;
    printf("\t\t******MENU******\n");
    printf("\t\t1.Yeni Ucus Numarasi Eklenmesi\n");
    printf("\t\t2.Kaydin Silinmesi\n");
    printf("\t\t3.Bir Ucusun Kalkis Zamaninin Degistirilmesi\n");
    printf("\t\t4.Bir Ucusa Iliskin Bilet Alimi\n");
    printf("\t\t5.Cikis\n");
    do
    {
        printf("\t\tSeciminizi giriniz:");
        fflush(stdin);
        scanf("%d",&secenek);//secim alinir
        if(secenek<1 ||secenek>5)//yanlis secimde uyari
            printf("\n\tYanlis Bir Secimde Bulundunuz\a\n");
    }
    while(secenek<1 ||secenek>5); //dogru secim yapincaya kadar calis
    ciz();
    return secenek;
}
int ucus_no_kontrol()//islem yapilacak ucus noyu alir
{
    int giris;
    do
    {
        printf("\n\t\tIslem yapmak istediginiz ucus numarasini giriniz:");
        scanf("%d",&giris);//ucus no alimi
        if(giris<1||giris>1000)//sinirlar disinda numara girldi ise mesaj ver
            printf("\t\tYanlis bir ucus numarasi girdiniz \nSecenekler 1 ile 1000 Arasindadir\n");
    }
    while( giris<1||giris>1000); //dogru no girinceye kadar doner

    return giris;
}
void ucus_no_al_ekle(FILE *Dfptr)//ucus numarasi alip ekeleme fonksiyonu
{
    ucus_type ucus_kayit;
    int giris;
    giris=ucus_no_kontrol();//kullaniciden ucus no alip dondurur
    fseek(Dfptr,(giris-1) *sizeof(ucus_type),SEEK_SET);//dosyadan o kayit bulunur
    fread(&ucus_kayit,sizeof(ucus_type),1,Dfptr);//dosyadan o kayit okunur
    if(ucus_kayit.ucus_no==0)//ucus nonun oldugu kayit bo ise bilgileri alip ekler
    {
        ucus_kayit.ucus_no=giris;//kullanicinin girdigi degeri ucus no olarak ata
        printf("\t\tKalkis yerini giriniz:           ");
        fflush(stdin);
        gets(ucus_kayit.kalkisYeri);
        printf("\t\tVaris yerini giriniz:            ");
        fflush(stdin);
        gets(ucus_kayit.VarisYeri);
        printf("\t\tKalkis saatini giriniz:          ");
        scanf("%d",&ucus_kayit.KalkisSaati);
        printf("\t\tKalkis Dakikasini giriniz:       ");
        scanf("%d",&ucus_kayit.KalkisDakikasi);
        printf("\t\tKoltuk Kapasitesini Giriniz:     ");
        scanf("%d",&ucus_kayit.KoltukKapasitesi);
        ucus_kayit.BosKoltukSayisi=ucus_kayit.KoltukKapasitesi;
        printf("\t\tBilet Baslangic Fiyatini Giriniz: ");
        scanf("%f",&ucus_kayit.bas_bilet_fiy);
        fseek(Dfptr,(giris-1) * sizeof(ucus_type),SEEK_SET);//ucus nonu oldugu kayida gidilir
        fwrite(&ucus_kayit,sizeof(ucus_type),1,Dfptr);//alinan bilgiler yazilir
        printf("\t\t******Ekleme islemi tamamlanmistir******\n\n");
        printf("\t\t\n********EKLENEN UCUS BILGILERI******\n\n");
        baslik();//nelerin eklendigini gormek amaciyla ekrana yazar
        printf("%3d %13s %13s %6.2d:%.2d %10d %10d %15.2f\n",ucus_kayit.ucus_no,ucus_kayit.kalkisYeri,ucus_kayit.VarisYeri,ucus_kayit.KalkisSaati,ucus_kayit.KalkisDakikasi,ucus_kayit.KoltukKapasitesi,ucus_kayit.BosKoltukSayisi,ucus_kayit.bas_bilet_fiy);
    }
    else
        printf("\n\n\t\tBu numaraya ait ucus bulunmaktadir!!!\a\n\n");
}
void baslik()//Baslik yazdirma fonksiyonu
{
    printf("Ucus No   Kalkis Yeri   Varis Yeri  Zamani    Kapasite  Bos Koltuk  Bilet Fiyati\n");
    printf("-------   -----------   ----------  ------    --------  ----------  ------------\n");
    return;
}
void  bir_ucusun_kaydinin_silinmesi(FILE *Dfptr, FILE *Tfptr)//2secenek ucus kaydinin silinmesi
{
    ucus_type bir_ucus;
    FILE *dosya;
    int giris,ucus_no;
    char tc[21];
    float bilet_fiyat;
    giris=ucus_no_kontrol();//kullanici ucus no girer
    fseek(Dfptr,(giris-1) * sizeof(ucus_type),SEEK_SET);//dosyadan o kayda gidilir
    fread(&bir_ucus,sizeof(ucus_type),1,Dfptr);//dosyadan o kayit okunur
    if(bir_ucus.ucus_no!=0)//bu ucusa ait kayit var ise
    {
        dosya=fopen("gecici.txt","w");// txt den ucusu silmek icin gecici dosya olustur
        bir_ucus.ucus_no=0;//ucus numarasina 0 atanir
        fseek(Dfptr, (giris-1) * sizeof(ucus_type), SEEK_SET);//tekrar dosyadan o kayda gidilir
        fwrite(&bir_ucus,sizeof(ucus_type),1,Dfptr);//yeni bilgiler dosyaya yazilir
        while(!feof(Tfptr))
        {
            fscanf(Tfptr,"%d %s %f",&ucus_no,tc,&bilet_fiyat);//bilet dosyasindan okuma yapilir
            if(ucus_no!=giris)
            {
                fflush(dosya);
                fprintf(dosya,"%d %s %.2f\n",ucus_no,tc,bilet_fiyat);//geciciye yazar
            }
        }
        printf("\n\n\t\tSilme islemi tamamlanmistir\n\n\n");//mesaj verilir
        fclose(dosya);
        fclose(Tfptr);
        remove("bilet.txt");
        rename("gecici.txt","bilet.txt");
        Tfptr=fopen("bilet.txt","a+");
    }
    else//eger kayit yok ise
        printf("\n\n\t\tBu Numaraya Ait Ucus Kayitlarda Bulunmamaktadir\a\n\n\n");
}
void ucusun_zamaninin_degistirilmesi(FILE *Dfptr)//3 secenek bir ucusun zamaninin degistirilmesi
{
    ucus_type bir_ucus;
    int giris;
    giris=ucus_no_kontrol();//kullanici ucus no girer
    fseek(Dfptr,(giris-1) * sizeof(ucus_type),SEEK_SET);//dosyadan o kayda gidilir
    fread(&bir_ucus,sizeof(ucus_type),1,Dfptr);//dosyadan o kayit okunur
    if(bir_ucus.ucus_no!=0)//bu ucusa ait kayit var ise
    {
        // yeni saat ve dakika girilir
        printf("\n\n\t\tUcusun yeni saatini giriniz:");
        scanf("%d",&bir_ucus.KalkisSaati);
        printf("\n\n\t\tUcusun yeni dakikasini giriniz:");
        scanf("%d",&bir_ucus.KalkisDakikasi);
        fseek(Dfptr,(giris-1) * sizeof(ucus_type),SEEK_SET);
        fwrite(&bir_ucus,sizeof(ucus_type),1,Dfptr);
        printf("\n\n\t\tUcusun kalkis zamani degistirildi\n");
        printf("\n\t\t***YENI BILGILER***\n");//guncelleme sonrasi ekrana yeni saatin yazilimi
        baslik();
        printf("%3d %13s %13s %6d:%d %10d %10d %15.2f\n",bir_ucus.ucus_no,bir_ucus.kalkisYeri,bir_ucus.VarisYeri,bir_ucus.KalkisSaati,bir_ucus.KalkisDakikasi,bir_ucus.KoltukKapasitesi,bir_ucus.BosKoltukSayisi,bir_ucus.bas_bilet_fiy);
    }
    else//eger kayit yok ise
        printf("\n\n\t\tBu Numaraya Ait Bir Ucus Bulunmamaktadir\n");//mesaj ver
}
void bir_ucusa_iliskin_bilet_satimi(FILE *Dfptr,FILE *Tfptr)//bilet satilma fonksiyonu
{
    ucus_type bir_ucus;
    char tc[12], cevap;
    int bilet_adet,i=0, giris,carpan ;
    float adet_bilet_fiyati[100],bos_kolutuk_orani,toplam_fiyat=0;

    giris=ucus_no_kontrol();
    fseek(Dfptr,(giris-1) * sizeof(ucus_type),SEEK_SET);
    fread(&bir_ucus,sizeof(ucus_type),1,Dfptr);
    if(bir_ucus.ucus_no!=0)
    {
        printf("\t\t\t\n\nSatin almak istediginiz bilet sayisini giriniz: ");
        scanf("%d",&bilet_adet);
        if(bir_ucus.BosKoltukSayisi>=bilet_adet)//bilet adeti kadar bos koltuk varsa satılma islemi baslar
        {
            for(i=0; i<bilet_adet; i++)
            {
                bos_kolutuk_orani=(bir_ucus.BosKoltukSayisi-i)*100/bir_ucus.KoltukKapasitesi;//bos koltuk orani bulur
                carpan=(100-bos_kolutuk_orani)/10;// orana göre carpan hesaplar carpan:int old. tam bolme yapar
                adet_bilet_fiyati[i]=bir_ucus.bas_bilet_fiy*((float)carpan/10+1);//carpanın float iler sınırlarını genislet
                toplam_fiyat+=adet_bilet_fiyati[i];//toplam fiyat hesaplar
                printf("\t\t%d. Koltuk fiyati:%.2f\n",i+1,adet_bilet_fiyati[i]);
            }
            printf("\t\tToplam billet adeti:%d\n",bilet_adet);
            printf("\t\tToplam Fiyat:%.2f\n",toplam_fiyat);
            printf("\t\tOrtalama bilet fiyati:%.2f\n",toplam_fiyat/bilet_adet);
            printf("\t\tBiletleri almak istiyorsaniz evet icin:e/E Hayir icin:h /H\n");
            fflush(stdin);
            scanf("%c",&cevap);//almak için secim yapılır
            if(cevap=='e'|| cevap=='E')//secime gore alım yapılır
            {
                bir_ucus.BosKoltukSayisi=bir_ucus.BosKoltukSayisi-bilet_adet;
                fseek(Dfptr,(giris-1) * sizeof(ucus_type),SEEK_SET);
                fwrite(&bir_ucus,sizeof(ucus_type),1,Dfptr);
                for(i=0; i<bilet_adet; i++)
                {
                    printf("\t\t%d.yolcunun TC kimlik Nosunu Giriniz:",i+1);
                    fflush(stdin);
                    scanf("%s",tc);//kullanici TC kim. no girer
                    fprintf(Tfptr,"%d %s %.2f\n",giris,tc,adet_bilet_fiyati[i]);
                }
                printf("\t\tBilet alim isleminiz gerceklesmistir\n");
            }
            else
                printf("\t\tBilet Satin Alma islemi yapilmadi\n");// satin alma biter
        }
        else
            printf("\t\tYeterli miktarda bilet bulunmamaktadir\a\n");// mesajlar verilir
    }
    else
        printf("\t\tBu Numaraya Ait Ucus Bulunmamaktadir\a\n");// mesajlar verilir
}
void ciz()
{
    printf("\t\t***************************************************\n");
}
