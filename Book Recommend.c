#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int **books;			// Sutunlarin kitaplari, satirlarinda kisileri gosterdigi puan matrisi.
int *similarPeople;		// Kisiye en cok benzeyen k kisinin id numaralarinin tutuldugu dizi.
float *similarities;	// Butun kisilerin secilen kisiyle olan benzerlik oranini tutan dizi.
float *predictedBooks;	// Kisinin puan vermedigi kitaplara kac puan verebilecegini tutan dizi.

int bookCount;			// Toplam kitap sayisi.
int peopleCount;		// Toplam kisi sayisi.
int k;					// Kullanicidan alinacak k degeri.
int simIndex = 0;		// SimilarPeople dizisinin en son bos indisini tutan degisken.

float averagePoint(int);		// Kisinin oy verdigi butun kitaplar icin puan ortalamasini donen fonksiyon.
float averagePoint2(int, int);	// Kisinin 2. argumandaki kisi ile ortak oy verdigi kitaplarin puan ortalamasini donen fonksiyon.
float sim(int,int);				// Verilmis olan benzerlik fonksiyonu.
float pred(int,int);			// Verilmis olan tahmin fonksiyonu.
int person;						// Kullanicidan ismi alinan kisinin indeksi.

int main(){
	int i, j;					// Dongulerde kullanilacak olan i ve j degiskenleri.
	char fileName[32];			// Kullanicidan alinacak olan csv dosyasinin isminin tutuldugu degisken.
	
	printf("Kitap sayisini girin: ");	// Kullanicidan kitap sayisi alinir.
	scanf("%d",&bookCount);
	
	printf("Kisi sayisini girin: ");	// Kullanicidan kisi sayisi alinir.
	scanf("%d",&peopleCount);
	
	books = (int **)malloc(peopleCount * sizeof(int*));			// Kitap matrisi icin kullanicidan alinan degerlere gore bellekte yer ayrilir.
	for(i = 0; i < peopleCount; i++) books[i] = (int *)malloc(bookCount * sizeof(int));
	
	if(books == NULL){											// books matrisinin olusturulup olusturulamadiginin kontrolu.
		printf("Bellek ayrilamadi.");
		return 1;
	}
	
	predictedBooks = (float*)malloc(bookCount * sizeof(float));	// kitap puan tahminlerinin tutuldugu dizi icin bellekte yer ayrilir.
	
	if(predictedBooks == NULL){									// predictedBooks dizisinin olusturulup olusturulamadiginin kontrolu.
		printf("Bellek ayrilamadi.");
		return 1;
	}
	
	char mat[peopleCount+1][bookCount+1][64];					// Csv dosyasinin okunup yazilacagi 3 boyutlu mat dizisi.(1. boyut kisilerin satirlari, 2. boyut kitaplarin sutunlari, 3. boyut ise kelimelerin uzunluklaridir)
	
	printf("csv dosyasinin adini girin: ");						// Csv dosyasinin adi kullanicidan alinir.
	scanf("%s", &fileName);
	
	FILE *fstream = fopen(fileName,"r");						// Csv dosyasi okuma formatinda acilir.
	
	if(fstream == NULL){										// Dosyanin acilip acilamadiginin kontrolu.
	    printf("\n file opening failed ");			
	    return 1;
	}
	
	// 3 boyutlu dizinin indexleri.
	int c = 0;		// Gecerli kolon.
	int r = 0;		// Gecerli satir.
	int s = 0;		// Gecerli karakter.
	
	char buffer[1024];		// Okunan satirlarin yazilacagi buffer ve line stringleri.
	char *line;
   	
	while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL){		// Her satir icin doner.
		c = 0;		// Gecerli kolon sifirlanir.
		i = 0;		// Line stringinin gecerli indisi sifirlanir.
		while(c < bookCount+1){										// Her sutun icin doner.
			s = 0;	// Gecerli string indisi sifirlanir.
			while(line[i] != ','){									// ',' karakteri yok ise gecerli karakteri diziye ekle.
				mat[r][c][s] = line[i];
				s++;
				i++;
			}
			i++;
			c++;
		}
		r++;
	}
	
	int validPeopleCount = 0;		// Ismi 'U' ile baslayan ve uzerinde karsilastirma islemleri yapilacak kisilerin sayisi.
	int endWhile = 0;				// Donguden cikma bayragi.
	while(validPeopleCount <= peopleCount && !endWhile){
		if(mat[validPeopleCount + 1][0][0] == 'U'){			// Kisi ismi 'U' ile basliyorsa validPeopleCount degiskeni bir arttirilir.
			validPeopleCount++;
		}else{												// Kisi ismi 'U' ile baslamiyorsa bu kisiden sonraki kisiler sorgusu yapilacak kisiler orn: NU1, NU2... oldugu icin donguden cikilir.
			endWhile = 1;
		}
	}
	
	similarities = (float*)malloc(validPeopleCount * sizeof(float));		// Benzerlik dizisi icin bellekte yer ayrilir.	
	
	if(similarities == NULL){												// similarities dizisinin olusturulup olusturulamadiginin kontrolu.
		printf("Bellek ayrilamadi.");
		return 1;
	}	
	
	for(i=1;i<peopleCount+1;i++){											// books dizisi sayi tamsayi degerleri alacagi icin mat dizisinin 1. sutun ve 1. sutun indislerindeki sayi karakterlerinin ascii degerlerinden 48 cikartilarak sayi degerleri elde edilir.
		for(j=1;j<bookCount+1;j++){
			books[i - 1][j - 1] = mat[i][j][0] - 48;
		}
	}

	printf("okunan books matrisi:\n");										// Okunan puan matrisi ekrana yazdirilir.
	for(i=0;i<peopleCount;i++){
		for(j=0;j<bookCount;j++){
			printf("%d ",books[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
	
	char personId[8];										// Sorgulanacak kullanicinin ismini tutan string.
	printf("Kitap Onerisi yapilacak kullanici adi: ");		// Kullanici adi alinir.
	scanf("%s", &personId);
	
	i = validPeopleCount;									// While dongusunde kullanilacak i degeri dosyanin sonunda bulunan kullanicilardan secileni bulmak icin validPeopleCount degerine esitlenir.
	endWhile = 0;											// Donguden cikma bayragi.
	
	while(i < peopleCount && !endWhile){
		if(strcmp(personId, mat[i+1][0]) == 0){				// Kullanicidan alinan isim mat dizisindeki kullanici isimleriyle karsilastirilir bulunursa donguden cikilir.
			person = i;
			endWhile = 1;
		}
		i++;
	}
	if(!endWhile){											// Kisi bulunamadiysa program sonlandirilir.
		printf("Kisi bulunamadi.");
		return 1;
	}
	
	printf("Kisi index: %d\n", person);						// Bulunan kisinin indisi ekrana yazdirilir.
	
	printf("Benzer kullanici sayisi(k): ");					// K degeri kullanicidan alinir.
	scanf("%d", &k);
	
	similarPeople = (int*)malloc(k * sizeof(int));			// similarPeople dizisi icin bellekte k degerine gore yer ayrilir.
	
	if(similarPeople == NULL){								// similarPeople dizisinin olusturulup olusturulamadiginin kontrolu.
		printf("Bellek ayrilamadi.");
		return 1;
	}
	
	for(i=0;i<validPeopleCount;i++){						// Kisinin validPeople kategorisine giren kisilerle benzerlikleri bulunup similarities dizisine yazilir.
		similarities[i] = sim(person ,i);
	}
	
	for(i=0; i<k; i++){										// similarities dizisinin en buyuk k elemanini bulmak icin k defa dizi kontrol edilir.
		
		int mostSimilar = 0;								// Gecerli dongunun en buyuk indisini tutan degisken.
		
		for(j=0;j<validPeopleCount;j++){					// Dizinin en buyuk elemani bulunur, bu eleman similarPeople dizisinin en basina koyulur ve bu elemanin similarities degeri bir sonraki dongude karsilastirmalara eklenmemesi icin -1 yapilir.
			if(similarities[j] > similarities[mostSimilar]){
				mostSimilar = j;
			}
		}
		
		similarPeople[simIndex] = mostSimilar;
		simIndex++;
		similarities[mostSimilar] = -1;
	}
	
	// Kullaniciya en benzer k kullanici ve benzerlik oranlari ekrana yazdirilir.
	printf("%s kullanicisina en yakin kullanicilar(k=%d) ve hesaplanan pearson benzerlikleri sirasiyla, <kullanici adi, pearsons coefficient>: \n", personId, k);
	for(i=0;i<k;i++){
		printf("%d. %s, %f\n", i, mat[similarPeople[i] + 1][0], sim(person ,similarPeople[i]));
	}

	// Kisinin puan vermedigi kitaplar icin puan tahminleri ekrana yazdirilir.
	printf("\n%s kullanicisinda okunmamis olan kitaplar icin hesaplanan tahmini begenme degerleri:\n", personId);
	j = 1;
	for(i=0;i<bookCount;i++){
		if(books[person][i] == 0){		// Gecerli kitabin puan degeri 0 ise kisi bu kitaba puan vermemis kabul edilir ve bu kitap icin puan tahmini hesaplanir.
			predictedBooks[i] = pred(person, i);
			printf("%d. %s, %f\n", j, mat[0][i+1], predictedBooks[i]);
			j++;
		}else{							// Gecerli kitaba zaten bir puan vermisse bu degerlerler ugrasmamak icin bu indeks -1 yapilir.
			predictedBooks[i] = -1;
		}
	}
	
	int maxPointBook = 0;			// Kisinin en cok puan verebilecegi kitabi bulmak icin tutulan degisken.
	
	for(i=0;i<bookCount;i++){		// Kisinin puan vermedigi kitaplarin puan tahminleri karsilastirilip en buyuk olan kitabin indisi bulunur.
		if(predictedBooks[i] > predictedBooks[maxPointBook]){
			maxPointBook = i;
		}
	}
	
	
	printf("\nSonuc olarak onerilen kitap: %s", mat[0][maxPointBook+1]);			// Tahmin edilen kitabin ismi ekrana yazdirilir.
	
	return 0;
}

float sim(int a, int b){
	
	float avgA = averagePoint2(a, b);		// A ve B kisininin ortak kitaplarina A kisisinin verdigi ortalama puan.
	float avgB = averagePoint2(b, a);		// A ve B kisininin ortak kitaplarina B kisisinin verdigi ortalama puan.
	
	// Denklemdeki degiskenler (sim(a,b) = x / (sqrt(y) * sqrt(z)))
	float x = 0;
	float y = 0;
	float z = 0;
	
	int i;
	// Denklemler verilen sekilde hesaplanir.
	for(i=0;i<bookCount; i++){
		if(books[a][i] != 0 && books[b][i] != 0){
			x += (books[a][i] - avgA) * (books[b][i] - avgB);
		}
	}

	for(i=0;i<bookCount;i++){
		if(books[a][i] != 0 && books[b][i] != 0){
			y += pow(books[a][i] - avgA, 2);
		}
	}
	
	for(i=0;i<bookCount;i++){
		if(books[a][i] != 0 && books[b][i] != 0){
			z += pow(books[b][i] - avgB, 2);
		}
	}
	
	return x / (sqrt(y) * sqrt(z)); 		// Denklem ustte yazilan sekilde donulur.
}

float averagePoint(int index){		
	
	int i;
	int sum = 0;			// Puan toplaminin tutulacagi degisken.
	int validBooks = 0;		// Kisinin oy vermis oldugu kitap sayisinin tutulacagi degisken.
	
	for(i=0;i<bookCount;i++){
		if(books[index][i] != 0){			// Puan matrisindeki deger 0 degil ise bu puan isleme dahil edilir.
			sum += books[index][i];
			validBooks++;
		}
	}
	
	return (float)sum / (float)validBooks;	// Ortalama puan donulur.
}

float averagePoint2(int a, int b){
	
	int i;
	int sum = 0;
	int validBooks = 0;
	
	for(i=0;i<bookCount;i++){
		if(books[a][i] != 0 && books[b][i] != 0){		// averagePoint fonksiyonundan farkli olarak iki kisininde puan degeri 0 olmayan kitaplar isleme dahil edilir.
			sum+= books[a][i];
			validBooks++;
		}
	}
	
	return (float)sum / (float)validBooks;
}

float pred(int person, int book){
	
	// Denklemdeki degiskenler (pred(a,p) = ra' + top/bottom)
	float top = 0;
	float bottom = 0;
	int i;
	
	// Denklemler verilen sekilde hesaplanir.
	for(i=0; i<k; i++){
		top += sim(person, similarPeople[i]) * (books[similarPeople[i]][book] - averagePoint(similarPeople[i]));
	}
	
	for(i=0; i<k; i++){
		bottom += sim(person, similarPeople[i]);
	}
	
	return averagePoint(person) + (top/bottom);		// Denklem ustte yazilan sekilde donulur.
}
