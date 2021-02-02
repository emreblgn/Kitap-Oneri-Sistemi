# Kitap Öneri Sistemi

Kullanıcının puan verdiği diğer kitaplara ve puan veren diğer kullanıcılara bakarak kullanıcıya kitap önerisinde bulunan sistem.

**Sistem 3 alt bölümden oluşmaktadır.**

1. Okuyucunun diğer okuyuculara olan benzerliğinin hesaplanması.

Okuyucuların benzerliği pearson katsayısı kullanılarak hesaplanmaktadır. Benzerlik değeri [-1,1] aralığındadır.

Benzerlik fonksiyonu:

![](/images/sim-function.png)

2. Okuyucunun en çok benzediği k kişinin belirlenmesi.

3. Okuyucuya yeni kitap önerilmesi.

Okuyucunun okumadığı bir kitaba verebileceği puan değeri aşağıdaki tahmin fonksiyonu ile hesaplanır. Değeri en yüksek çıkan kitap okuyucuya önerilir.

![](/images/pred-function.png)

**Örnek Girdi ve Çıktılar**

![](/images/input-1.png)

![](/images/input-2.png)

![](/images/input-3.png)

![](/images/input-4.png)
